#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

Field name(std::size_t at) {
    return structure("mName", at, {string("mUnredacted", 0x0), optional("mFilteredName", 0x20, 0x20, string("value", 0))});
}
Object block(int x, int y, int z, int mode, bool redstone, bool conditional) {
    return Object{{"mBlockPos", Object{{"x", x}, {"y", y}, {"z", z}}}, {"mMode", mode}, {"mRedstoneMode", redstone},
                  {"mIsConditional", conditional}};
}
}

BPX_PACKET("CommandBlockUpdatePacket") {
    p.shape("1.16.201", {
        blockPos("mBlockPos", 0x0),
        u16("mMode", 0xC),
        boolean("mRedstoneMode", 0xE),
        boolean("mIsConditional", 0xF),
        string("mCommand", 0x18),
        string("mLastOutput", 0x38),
        string("mNameText", 0x58),
        u32("mTickDelay", 0x78),
    });
    p.shape("1.21.60", {
        blockPos("mBlockPos", 0x0),
        u16("mMode", 0xC),
        boolean("mRedstoneMode", 0xE),
        boolean("mIsConditional", 0xF),
        u64("mEntityId", 0x10),
        string("mCommand", 0x18),
        string("mLastOutput", 0x38),
        name(0x58),
        u32("mTickDelay", 0xA0),
        boolean("mTrackOutput", 0xA4),
        boolean("mExecuteOnFirstTick", 0xA5),
        boolean("mIsBlock", 0xA6),
    });
    p.shape("1.26.30", {
        variant("mTarget", 0x0, 0x10,
                {structure("EntityCommandTarget", 0, {u64("mEntityId", 0x0)}),
                 structure("BlockCommandData", 0, {blockPos("mBlockPos", 0x0), u16("mMode", 0xC), boolean("mRedstoneMode", 0xE),
                                                   boolean("mIsConditional", 0xF)})}),
        string("mCommand", 0x18),
        string("mLastOutput", 0x38),
        name(0x58),
        boolean("mTrackOutput", 0xA0),
        u32("mTickDelay", 0xA4),
        boolean("mExecuteOnFirstTick", 0xA8),
    });

    p.values("mBlockPos", {Object{{"x", 0}, {"y", 64}, {"z", 0}}, Object{{"x", -123}, {"y", -64}, {"z", 456}},
                           Object{{"x", 30000}, {"y", 319}, {"z", -30000}}});
    p.values("mMode", {0, 1, 2}); // CommandBlockMode: Normal, Repeating, Chain
    p.values("mRedstoneMode", {false, true});
    p.values("mIsConditional", {false, true});
    p.values("mEntityId", {1, 123456});
    p.values("mIsBlock", {false, true});
    p.values("mTarget", {
        Object{{"index", 0}, {"value", Object{{"mEntityId", 1}}}},
        Object{{"index", 0}, {"value", Object{{"mEntityId", 123456}}}},
        Object{{"index", 1}, {"value", block(0, 64, 0, 0, false, false)}},
        Object{{"index", 1}, {"value", block(-123, -64, 456, 1, true, false)}},
        Object{{"index", 1}, {"value", block(30000, 319, -30000, 2, false, true)}},
    });
    p.values("mCommand", {"", "say hello", "/give @p minecraft:diamond 64", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mLastOutput", {"", "commands.say.success", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mNameText", {"", "Command Block", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mName", {Object{{"mUnredacted", ""}}, Object{{"mUnredacted", "Command Block"}},
                       Object{{"mUnredacted", "bad word"}, {"mFilteredName", "*** word"}}});
    p.values("mTickDelay", {0, 1, 20, 1200});
    p.values("mTrackOutput", {false, true});
    p.values("mExecuteOnFirstTick", {false, true});
}
