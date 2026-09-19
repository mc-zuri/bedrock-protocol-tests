#include "packets/Packet.h"

#include <string>
#include <vector>

namespace {
using namespace bpx::schema;

Field params(std::size_t at) { return vector("mParams", at, 0x20, string("value", 0)); }

Layout older(bool filtered) {
    Layout l{u8("mType", 0x0), string("mAuthor", 0x8), string("mMessage", 0x28)};
    if (!filtered) {
        for (Field f : {params(0x48), boolean("mLocalize", 0x60), string("mXuid", 0x68), string("mPlatformId", 0x88)}) l.push_back(std::move(f));
    } else {
        for (Field f : {optional("mFilteredMessage", 0x48, 0x20, string("value", 0)), params(0x70), boolean("mLocalize", 0x88),
                        string("mXuid", 0x90), string("mPlatformId", 0xB0)})
            l.push_back(std::move(f));
    }
    return l;
}

Object alt(int i, Object v) { return Object{{"index", i}, {"value", std::move(v)}}; }

std::vector<int> const kMessageOnly{0, 5, 6, 9, 10}; // Raw, Tip, SystemMessage, TextObjectWhisper, TextObject
std::vector<int> const kAuthor{1, 7, 8};             // Chat, Whisper, Announcement
std::vector<int> const kParams{2, 3, 4};             // Translate, Popup, JukeboxPopup
char const* const kMessage = "\xc2\xa7" "eHello \xe2\x9c\x93";
}

BPX_PACKET("TextPacket") {
    p.shape("1.16.201", older(false));
    p.shape("1.21.0", older(true));
    p.shape("1.21.130", {boolean("mLocalize", 0x0), string("mXuid", 0x8), string("mPlatformId", 0x28),
                         optional("mFilteredMessage", 0x48, 0x20, string("value", 0)),
                         variant("mBody", 0x70, 0x48,
                                 {structure("MessageOnly", 0, {u8("mType", 0x0), string("mMessage", 0x8)}),
                                  structure("AuthorAndMessage", 0, {u8("mType", 0x0), string("mAuthor", 0x8), string("mMessage", 0x28)}),
                                  structure("MessageAndParams", 0, {u8("mType", 0x0), string("mMessage", 0x8), params(0x28)})})});

    p.values("mLocalize", {false, true});
    p.values("mXuid", {"", "2535412345678901"});
    p.values("mPlatformId", {"", "platform-chat-id"});
    // to 1.21.124: a Raw message unless the scenario picks another type
    p.values("mMessage", {"", kMessage, "chat.type.text"});
    p.values("mFilteredMessage", {Value(), "H****"});
    for (int t : kMessageOnly) p.scenario("message only", {{"mType", t}, {"mMessage", kMessage}});
    p.scenario("1.19.40", "message only", {{"mType", 11}, {"mMessage", kMessage}}); // TextObjectAnnouncement
    for (int t : kAuthor) p.scenario("author and message", {{"mType", t}, {"mAuthor", "Steve"}, {"mMessage", kMessage}});
    for (int t : kParams)
        p.scenario("message and parameters", {{"mType", t}, {"mMessage", "%s joined"}, {"mParams", List{"Steve", "\xe2\x9c\x93"}}});
    p.scenario("translate, no parameters", {{"mType", 2}, {"mMessage", "commands.generic.unknown"}, {"mParams", List{}}});
    List body;
    for (int t : {0, 5, 6, 9, 10, 11}) body.push_back(alt(0, Object{{"mType", t}, {"mMessage", kMessage}}));
    for (int t : kAuthor) body.push_back(alt(1, Object{{"mType", t}, {"mAuthor", "Steve"}, {"mMessage", kMessage}}));
    for (int t : kParams) body.push_back(alt(2, Object{{"mType", t}, {"mMessage", "%s joined"}, {"mParams", List{"Steve", "\xe2\x9c\x93"}}}));
    p.values("mBody", body);
    p.baseline("1.21.130", "mBody", body.front()); // the reader rejects an empty message
    p.values("1.21.130", "mFilteredMessage", {Value(), "", "H****"});
}
