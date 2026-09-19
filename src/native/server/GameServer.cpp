#include "server/Server.h"

#include "bindings/Bindings.h"
#include "bindings/Names.h"
#include "memory/Guard.h"
#include "server/Recorder.h"

#include <malloc.h>

#include <cstring>
#include <format>
#include <memory>
#include <new>

namespace bpx::server {

namespace n = bindings::names;

namespace {

// Holds the BinaryStream object (0x68 in 1.21.0), not the packet: the packet's bytes are in a std::string it points to.
struct alignas(16) StreamStorage {
    std::byte bytes[0x400]{};
    void*     get() { return bytes; }
};

using CreatePacketFn     = std::shared_ptr<void>* (*)(std::shared_ptr<void>* result, int id);
using BinaryStreamCtorFn = void* (*)(void* self, std::string* buffer, bool copyBuffer);
using GetNameStrFn       = std::string* (*)(void const* self, std::string* result);         // member fns return via hidden rdx
using GetNameViewFn      = std::string_view* (*)(void const* self, std::string_view* result);
using WriteFn            = void (*)(void const* self, void* stream);
using ReadFn             = void* (*)(void* self, void* result, void* stream);
using ReadEnumFn         = int (*)(void* self, void* stream);
using DeletingDtorFn     = void* (*)(void* self, unsigned flags);
using WriteCtxFn         = void (*)(void const* self, void* stream, void* reflectionCtx);

template <class Fn>
Fn virtualAt(void const* object, int slot) {
    return (*static_cast<Fn* const*>(object))[slot];
}

// 1.21.111 .. 1.21.124 SerializedPayloadPackets: write/read are asserting stubs (`mov [0], 0xdeadc0de`).
// The real writer is the slot before write (plus a ReflectionCtx; zeroed works), the reader the slot after read.
bool isAssertStub(void const* fn) {
    static constexpr unsigned char kStore[] = {0xC7, 0x04, 0x25, 0, 0, 0, 0, 0xDE, 0xC0, 0xAD, 0xDE};
    auto const*                    code     = static_cast<unsigned char const*>(fn);
    for (int i = 0; i < 0x80; ++i)
        if (std::memcmp(code + i, kStore, sizeof kStore) == 0) return true;
    return false;
}

void callWrite(void const* packet, void* stream, int slot) {
    auto const fn = virtualAt<WriteFn>(packet, slot);
    if (slot > 0 && isAssertStub(reinterpret_cast<void const*>(fn))) {
        alignas(16) static std::byte reflectionCtx[0x400]{};
        virtualAt<WriteCtxFn>(packet, slot - 1)(packet, stream, reflectionCtx);
        return;
    }
    fn(packet, stream);
}

int readSlot(void const* packet, int slot) {
    return isAssertStub(reinterpret_cast<void const*>(virtualAt<ReadFn>(packet, slot))) ? slot + 1 : slot;
}

void destroyStream(void* stream) {
    // slot 0: scalar deleting destructor; flags 0 = destroy without freeing (storage is ours)
    memory::guard([&] { virtualAt<DeletingDtorFn>(stream, 0)(stream, 0); });
}

class GameServer final : public Server {
public:
    GameServer(Version version, bindings::Registry const& r) : mVersion(version) {
        mCreate        = reinterpret_cast<CreatePacketFn>(r.address(n::CreatePacket));
        mStreamCtor    = reinterpret_cast<BinaryStreamCtorFn>(r.address(n::BinaryStreamCtor));
        mStreamKind    = r.variant(n::StreamConstruction);
        mStreamVtable  = r.address(n::BinaryStreamVtable);
        mOwned         = r.offset(n::StreamOwnedBuffer, 0x8);
        mView          = r.offset(n::StreamView, 0x28);
        mReadPointer   = r.offset(n::StreamReadPointer, 0x38);
        mOverflowed    = r.offset(n::StreamHasOverflowed, 0x40);
        mBufferRef     = r.offset(n::StreamBuffer, 0x48);
        mGetNameKind   = r.variant(n::GetNameKind);
        mSlotName      = r.slot(n::PacketGetName);
        mSlotWrite     = r.slot(n::PacketWrite);
        mSlotRead      = r.slot(n::PacketRead);
        mPayload       = static_cast<std::size_t>(r.offset(n::PacketSize, 0x30));
        mResultHasValue = r.offset(n::ResultHasValue, 0x40);
        mReadKind      = r.variant(n::ReadKind);
        mMaxId         = static_cast<PacketId>(r.offset(n::MaxPacketId, 1023));
    }

    Version const& version() const override { return mVersion; }
    PacketId       maxPacketId() const override { return mMaxId; }
    std::size_t    payloadOffset() const override { return mPayload; }

    GamePacket create(PacketId id) override {
        std::shared_ptr<void> object;
        memory::guard([&] { mCreate(&object, static_cast<int>(id)); });
        return object ? GamePacket(std::move(object), id) : GamePacket();
    }

    std::string name(GamePacket const& packet) override {
        std::string out;
        if (mSlotName < 0) return out;
        memory::guard([&] {
            if (mGetNameKind == n::GetNameStringView) {
                std::string_view view;
                virtualAt<GetNameViewFn>(packet.data(), mSlotName)(packet.data(), &view);
                out.assign(view);
            } else {
                virtualAt<GetNameStrFn>(packet.data(), mSlotName)(packet.data(), &out);
            }
        });
        return out;
    }

    std::optional<std::string> write(GamePacket const& packet) override {
        std::string   body;
        StreamStorage stream;
        if (!makeStream(stream.get(), body)) return std::nullopt;
        bool ok = memory::guard([&] { callWrite(packet.data(), stream.get(), mSlotWrite); });
        destroyStream(stream.get());
        if (!ok) return std::nullopt;
        return body;
    }

    std::optional<ReadResult> read(GamePacket& packet, std::string_view body) override {
        if (mSlotRead < 0) return std::nullopt;
        // A BinaryStream is a ReadOnlyBinaryStream, so the same constructor serves reading.
        std::string   buffer(body);
        StreamStorage stream;
        if (!makeStream(stream.get(), buffer)) return std::nullopt;
        alignas(16) std::byte result[0x100]{};
        // The read position's offset varies by build: take the qword that moves from 0 to <= body size.
        std::byte before[0x80];
        std::memcpy(before, stream.get(), sizeof before);
        int  status = 0;
        bool ok     = memory::guard([&] {
            int const slot = readSlot(packet.data(), mSlotRead);
            if (mReadKind == n::ReadEnum) status = virtualAt<ReadEnumFn>(packet.data(), slot)(packet.data(), stream.get());
            else virtualAt<ReadFn>(packet.data(), slot)(packet.data(), result, stream.get());
        });
        std::optional<std::size_t> consumed;
        for (std::size_t o = 8; o + 8 <= sizeof before && !consumed; o += 8) {
            std::uint64_t was = 0, now = 0;
            std::memcpy(&was, before + o, 8);
            std::memcpy(&now, static_cast<std::byte*>(stream.get()) + o, 8);
            if (was == 0 && now != 0 && now <= body.size()) consumed = static_cast<std::size_t>(now);
        }
        destroyStream(stream.get());
        if (!ok) return std::nullopt;
        switch (mReadKind) {
        case n::ReadEnum: return ReadResult{status == 1, body.size(), consumed};
        case n::ReadExtended: {
            std::int32_t code = 0;
            std::memcpy(&code, result, sizeof code);
            std::destroy_at(reinterpret_cast<std::string*>(result + 8));
            return ReadResult{code == 1, body.size(), consumed};
        }
        default: return ReadResult{static_cast<bool>(result[mResultHasValue]), body.size(), consumed};
        }
    }

    std::optional<WriteTrace> traceWrite(GamePacket const& packet) override {
        WriteTrace    out;
        StreamStorage stream;
        if (!makeStream(stream.get(), out.body)) return std::nullopt;
        recorder::attach(stream.get(), &out.body, &out.events);
        bool ok = memory::guard([&] { callWrite(packet.data(), stream.get(), mSlotWrite); });
        recorder::detach(stream.get());
        destroyStream(stream.get());
        if (!ok) return std::nullopt;
        return out;
    }

    std::size_t objectSize(GamePacket const& packet) override {
        // make_shared block = control block + object, allocated via malloc, so _msize knows its size
        std::size_t size  = 0;
        auto const* block = static_cast<std::byte const*>(packet.block());
        if (!block) return 0;
        memory::guard([&] { size = _msize(const_cast<std::byte*>(block)); });
        auto used = static_cast<std::size_t>(packet.data() - block);
        return size > used ? size - used : 0;
    }

private:
    bool makeStream(void* at, std::string& buffer) {
        if (mStreamKind == n::StreamByLayout) {
            if (!mStreamVtable) return false;
            auto* p = static_cast<std::byte*>(at);
            *reinterpret_cast<std::uintptr_t*>(p)            = mStreamVtable;
            new (p + mOwned) std::string();
            *reinterpret_cast<std::string_view*>(p + mView)  = buffer;
            *reinterpret_cast<std::uint64_t*>(p + mReadPointer) = 0;
            *reinterpret_cast<bool*>(p + mOverflowed)         = false;
            *reinterpret_cast<std::string**>(p + mBufferRef)  = &buffer;
            return true;
        }
        return mStreamCtor && memory::guard([&] { mStreamCtor(at, &buffer, false); });
    }

    Version            mVersion;
    CreatePacketFn     mCreate      = nullptr;
    BinaryStreamCtorFn mStreamCtor  = nullptr;
    int                mStreamKind  = 0;
    std::uintptr_t     mStreamVtable = 0;
    std::int64_t       mOwned = 0x8, mView = 0x28, mReadPointer = 0x38, mOverflowed = 0x40, mBufferRef = 0x48;
    int                mGetNameKind = 0;
    int                mSlotName = -1, mSlotWrite = -1, mSlotRead = -1;
    std::size_t        mPayload = 0x30;
    std::int64_t       mResultHasValue = 0x40;
    int                mReadKind = n::ReadResult;
    PacketId           mMaxId = 1023;
};

}

std::unique_ptr<Server> connect(Version version, std::string& error) {
    auto& r      = bindings::load(version);
    auto  failed = r.resolve();
    std::string missing;
    if (!r.address(n::CreatePacket)) missing += std::format(" {}", n::CreatePacket);
    auto streamBinding = r.variant(n::StreamConstruction) == n::StreamByLayout ? n::BinaryStreamVtable : n::BinaryStreamCtor;
    if (!r.address(streamBinding)) missing += std::format(" {}", streamBinding);
    if (r.slot(n::PacketWrite) < 0) missing += std::format(" {}", n::PacketWrite);
    if (!missing.empty()) {
        error = "unresolved:" + missing;
        for (auto const& f : failed) error += "\n  " + f;
        return nullptr;
    }
    return std::make_unique<GameServer>(version, r);
}

}
