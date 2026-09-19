#pragma once

#include "core/Version.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace bpx::server {

using PacketId = std::uint32_t;

class GamePacket {
public:
    GamePacket() = default;
    GamePacket(std::shared_ptr<void> object, PacketId id) : mObject(std::move(object)), mId(id) {}

    [[nodiscard]] PacketId  id() const { return mId; }
    [[nodiscard]] std::byte* data() const { return static_cast<std::byte*>(mObject.get()); }
    [[nodiscard]] explicit operator bool() const { return mObject != nullptr; }

    template <class T>
    [[nodiscard]] T& at(std::size_t offset) const {
        return *reinterpret_cast<T*>(data() + offset);
    }

    // MSVC shared_ptr's control-block pointer: the make_shared allocation holding the object.
    [[nodiscard]] void const* block() const { return reinterpret_cast<void* const*>(&mObject)[1]; }

private:
    std::shared_ptr<void> mObject;
    PacketId              mId = 0;
};

struct ReadResult {
    bool        ok   = false;
    std::size_t size = 0;
    std::optional<std::size_t> consumed; // unset when the build's read position could not be located
};

struct WriteEvent {
    std::string doc;
    std::string notes;
    std::size_t begin;
    int         slot;
};

struct WriteTrace {
    std::string             body;
    std::vector<WriteEvent> events;
};

class Server {
public:
    virtual ~Server() = default;

    [[nodiscard]] virtual Version const& version() const = 0;

    [[nodiscard]] virtual PacketId maxPacketId() const = 0;

    // sizeof(Packet) in this build: where a packet's own fields start.
    [[nodiscard]] virtual std::size_t payloadOffset() const = 0;

    virtual GamePacket create(PacketId id) = 0;

    virtual std::string name(GamePacket const& packet) = 0;

    // nullopt when the game faulted.
    virtual std::optional<std::string> write(GamePacket const& packet) = 0;

    virtual std::optional<ReadResult> read(GamePacket& packet, std::string_view body) = 0;

    // Before 1.21.60 BinaryStream writers are not virtual, so no events are recorded.
    virtual std::optional<WriteTrace> traceWrite(GamePacket const& packet) = 0;

    // 0 when unknown.
    virtual std::size_t objectSize(GamePacket const& packet) = 0;
};

std::unique_ptr<Server> connect(Version version, std::string& error);

}
