#include "packets/Packet.h"

namespace {
using namespace bpx::schema;

Field page() { return structure("ReplacePage", 0, {i32("mPageIndex", 0x0), string("mPageText", 0x8), string("mPhotoName", 0x28)}); }

Object text(int i, char const* text, char const* photo) {
    return Object{{"mPageIndex", i}, {"mPageText", text}, {"mPhotoName", photo}};
}
}

BPX_PACKET("BookEditPacket") {
    p.shape("1.16.201", {u8("mAction", 0x0), i32("mBookSlot", 0x4), i32("mPageIndex1", 0x8), i32("mPageIndex2", 0xC),
                         string("mText1", 0x10), string("mText2", 0x30), string("mText3", 0x50)});
    {
        Field add = page();
        add.name  = "AddPage";
        p.shape("1.26.0", {i32("mBookSlot", 0x0),
                           variant("mOperation", 0x8, 0x60,
                                   {page(), add, structure("DeletePage", 0, {i32("mPageIndex", 0x0)}),
                                    structure("SwapPages", 0, {i32("mPageIndex", 0x0), i32("mSwapWithIndex", 0x4)}),
                                    structure("Finalize", 0, {string("mTitle", 0x0), string("mAuthor", 0x20), string("mXuid", 0x40)})})});
    }

    p.values("mBookSlot", {0, 1, 8, 35});
    p.values("mAction", {0, 1, 2, 3, 4}); // ReplacePage, AddPage, DeletePage, SwapPages, Finalize
    p.values("mPageIndex1", {0, 1, 49});
    p.values("mText1", {"", "Page text", "\xc2\xa7" "aunicode \xe2\x9c\x93\nline 2"});
    p.values("mText2", {"", "photo_1"});
    p.scenario("AddPage", {{"mAction", 1}, {"mPageIndex1", 3}, {"mText1", "new page"}, {"mText2", "photo_2"}});
    p.scenario("DeletePage", {{"mAction", 2}, {"mPageIndex1", 7}});
    p.scenario("SwapPages", {{"mAction", 3}, {"mPageIndex1", 2}, {"mPageIndex2", 5}});
    p.scenario("Finalize", {{"mAction", 4}, {"mText1", "My Book"}, {"mText2", "Steve"}, {"mText3", "2535412345678901"}});
    p.values("mOperation", {Object{{"index", 0}, {"value", text(0, "", "")}},
                            Object{{"index", 0}, {"value", text(49, "\xc2\xa7" "aunicode \xe2\x9c\x93\nline 2", "photo_1")}},
                            Object{{"index", 1}, {"value", text(3, "new page", "photo_2")}},
                            Object{{"index", 2}, {"value", Object{{"mPageIndex", 7}}}},
                            Object{{"index", 3}, {"value", Object{{"mPageIndex", 2}, {"mSwapWithIndex", 5}}}},
                            Object{{"index", 4}, {"value", Object{{"mTitle", "My Book"}, {"mAuthor", "Steve"}, {"mXuid", "2535412345678901"}}}},
                            Object{{"index", 4}, {"value", Object{{"mTitle", ""}, {"mAuthor", ""}, {"mXuid", ""}}}}});
}
