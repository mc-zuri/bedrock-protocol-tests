#include "packets/Packet.h"

namespace {
char const* const kHeader    = "eyJhbGciOiJFUzM4NCIsIng1dSI6Ik1IWXdFQVlIS29aSXpqMENBUVlGSzRFRUFDSURZZ0FFIn0"; // {"alg":"ES384","x5u":...}
char const* const kPayload   = "eyJzYWx0IjoiQUFBQUFBQUFBQUFBQUFBQUFBQUFBQT09In0";                             // {"salt":"AAAA..."}
char const* const kSignature = "c2lnbmF0dXJl";
}

BPX_PACKET("ServerToClientHandshakePacket") {
    p.shape("1.16.201", {string("mToken", 0x0)});
    p.shape("1.26.40", {string("mHeader", 0x0), string("mData", 0x30), string("mSignature", 0x60)});

    p.values("mToken", {std::string(kHeader) + "." + kPayload + "." + kSignature, "eyJhbGciOiJub25lIn0.eyJzYWx0IjoiIn0."});
    p.baseline("mHeader", kHeader);
    p.baseline("mData", kPayload);
    p.baseline("mSignature", kSignature);
    p.values("mHeader", {kHeader, "eyJhbGciOiJub25lIn0"});
    p.values("mData", {kPayload, "eyJzYWx0IjoiIn0"});
    p.values("mSignature", {kSignature, "AAAA"}); // not empty: the reader rejects a token without a signature
}
