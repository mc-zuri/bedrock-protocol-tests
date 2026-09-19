#pragma once

#include "core/Version.h"
#include "schema/Schema.h"

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

// Field offsets are relative to Server::payloadOffset(), so Packet base class changes don't touch defs.
// Packets are matched by getName(), not by id.
namespace bpx::packets {

using schema::Field;
using schema::Layout;
using schema::List;
using schema::Object;
using schema::Value;

struct Scenario {
    std::string label;
    Object      values;
};

class PacketDef {
public:
    explicit PacketDef(std::string name) : mName(std::move(name)) {}

    PacketDef& shape(std::string_view since, Layout layout);
    PacketDef& values(std::string_view field, List domain);
    PacketDef& values(std::string_view since, std::string_view field, List domain);
    // For a member whose default the game's own reader rejects (SetDefaultGameTypePacket's -1 = Undefined).
    PacketDef& baseline(std::string_view field, Value value);
    PacketDef& baseline(std::string_view since, std::string_view field, Value value);
    PacketDef& scenario(std::string label, Object values);
    PacketDef& scenario(std::string_view since, std::string label, Object values);
    PacketDef& scenario(std::string_view since, std::string_view until, std::string label, Object values);
    // Raw bodies for members that cannot be set field by field (polymorphic pointers, CompoundTag).
    // Payloads apply by their own versions, even outside since() / until().
    PacketDef& payload(std::string label, std::string bytes);
    PacketDef& payload(std::string_view since, std::string_view until, std::string label, std::string bytes);
    PacketDef& captured(std::string capture, std::size_t skip = 0);
    // The game re-encodes what it reads (e.g. compresses), so a payload counts if the game's own re-write round-trips.
    PacketDef& rewrites(std::string why);
    [[nodiscard]] bool rewritesOwnWay() const { return !mRewrites.empty(); }
    // For a reader that stores into a member its writer never writes: move `size` bytes at `own` into
    // `packet` at `other` and require that packet's write to equal the capture exactly.
    struct Via {
        Version     since, until;
        std::string packet;
        std::size_t own = 0, other = 0, size = 0;
        std::string capture;
    };
    PacketDef& checkedVia(std::string_view since, std::string_view until, std::string packet, std::size_t own, std::size_t other,
                          std::size_t size, std::string capture);
    [[nodiscard]] Via const* via(Version v) const;
    PacketDef& since(std::string_view version);
    PacketDef& until(std::string_view version);
    // Id fallback for builds whose getName returns "" (1.21.0's UpdateTradePacket).
    PacketDef& unnamed(std::uint32_t id);

    [[nodiscard]] std::string const& name() const { return mName; }
    [[nodiscard]] Layout const* layout(Version v) const;
    [[nodiscard]] bool          supports(Version v) const;
    [[nodiscard]] std::uint32_t unnamedId() const { return mUnnamed; }
    [[nodiscard]] std::vector<Scenario> scenarios(Layout const& layout, Version v) const;
    struct Payload {
        Version     since, until;
        std::string label;
        std::string bytes;
        std::string capture; // used instead of bytes when set
        std::size_t skip = 0;
    };
    [[nodiscard]] std::vector<Payload> payloads(Version v) const;

private:
    std::string                                   mName;
    std::vector<std::pair<Version, Layout>>       mShapes;
    struct Domain {
        Version     since;
        std::string field;
        List        values;
    };
    struct Baseline {
        Version     since;
        std::string field;
        Value       value;
    };
    std::vector<Domain>                           mValues;
    std::vector<Baseline>                         mBaselines;
    struct Extra {
        Version  since;
        Version  until;
        Scenario scenario;
    };
    std::vector<Extra>                            mExtra;
    std::vector<Payload>                          mPayloads;
    std::string                                   mRewrites;
    std::vector<Via>                              mVia;
    Version                                       mSince{}, mUntil{9999, 0, 0, 0};
    std::uint32_t                                 mUnnamed = 0;
};

std::map<std::string, PacketDef, std::less<>>& all();

int add(std::string name, void (*define)(PacketDef&));

}

#define BPX_PACKET_CAT2(a, b) a##b
#define BPX_PACKET_CAT(a, b)  BPX_PACKET_CAT2(a, b)
#define BPX_PACKET(name)                                                                             \
    static void BPX_PACKET_CAT(bpxPacket_, __LINE__)(::bpx::packets::PacketDef & p);                \
    [[maybe_unused]] static int const BPX_PACKET_CAT(bpxPacketReg_, __LINE__) =                      \
        ::bpx::packets::add(name, &BPX_PACKET_CAT(bpxPacket_, __LINE__));                            \
    using namespace ::bpx::schema;                                                                   \
    static void BPX_PACKET_CAT(bpxPacket_, __LINE__)([[maybe_unused]] ::bpx::packets::PacketDef & p)
