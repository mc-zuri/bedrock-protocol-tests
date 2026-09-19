#include "packets/Packet.h"

#include <cstdint>
#include <string>

BPX_PACKET("PhotoTransferPacket") {
    p.shape("1.16.201", {string("mPhotoName", 0x0), string("mPhotoData", 0x20), string("mBookId", 0x40)});
    p.shape("1.17.30", {string("mPhotoName", 0x0), string("mPhotoData", 0x20), string("mBookId", 0x40), u8("mType", 0x60),
                        u8("mSourceType", 0x61), i64("mOwnerId", 0x68), string("mNewPhotoName", 0x70)});

    p.values("mPhotoName", {"", "photo_1", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mPhotoData", {"", std::string("\x89PNG\r\n\x1a\n\x00\x00", 10)});
    p.values("mBookId", {"", "book-0001"});
    p.values("mType", {0, 1, 2});       // Portfolio, PhotoItem, Book
    p.values("mSourceType", {0, 1, 2}); // Portfolio, PhotoItem, Book
    p.values("mOwnerId", {-1, 1, std::int64_t{-4294967295LL}});
    p.values("mNewPhotoName", {"", "photo_2"});
}
