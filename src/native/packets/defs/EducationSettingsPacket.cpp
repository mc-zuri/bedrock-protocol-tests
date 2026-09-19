#include "packets/Packet.h"

namespace {
using namespace bpx::schema;

Layout head() { return {string("codeBuilderDefaultUri", 0x0), string("codeBuilderTitle", 0x20)}; }
Field agent(std::size_t at) { return optional("agentCapabilities", at, 2, optional("canModifyBlocks", 0, 1, boolean("value", 0))); }
Field link(std::size_t at) {
    return optional("externalLinkSettings", at, 0x40, structure("value", 0, {string("url", 0x0), string("displayName", 0x20)}));
}
Layout since1_17_30(bool hasQuiz) {
    Layout l = head();
    for (Field f : {boolean("canResizeCodeBuilder", 0x40), boolean("disableLegacyTitleBar", 0x41), string("postProcessFilter", 0x48),
                    string("screenshotBorderResourcePath", 0x68), optional("codeBuilderOverrideUri", 0xC8, 0x20, string("value", 0))})
        l.push_back(std::move(f));
    if (hasQuiz) {
        l.push_back(boolean("hasQuiz", 0xF0));
        l.push_back(agent(0xF8));
        l.push_back(link(0x100));
    } else {
        l.push_back(agent(0xF0));
        l.push_back(link(0xF8));
    }
    return l;
}
}

BPX_PACKET("EducationSettingsPacket") {
    p.shape("1.16.201", head());
    {
        Layout l = head();
        l.push_back(boolean("canResizeCodeBuilder", 0x40));
        l.push_back(optional("codeBuilderOverrideUri", 0x88, 0x20, string("value", 0)));
        l.push_back(boolean("hasQuiz", 0xB0));
        p.shape("1.16.210", l);
    }
    p.shape("1.17.30", since1_17_30(true));
    p.shape("1.20.61", since1_17_30(false));

    p.values("codeBuilderDefaultUri", {"", "https://education.minecraft.net/codebuilder"});
    p.values("codeBuilderTitle", {"", "Code Builder", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("canResizeCodeBuilder", {false, true});
    p.values("disableLegacyTitleBar", {false, true});
    p.values("postProcessFilter", {"", "grayscale"});
    p.values("screenshotBorderResourcePath", {"", "textures/ui/screenshot_frame"});
    p.values("codeBuilderOverrideUri", {Value(), "", "https://localhost:8080/"});
    p.values("hasQuiz", {false, true});
    p.values("agentCapabilities", {Value(), Object{{"canModifyBlocks", false}}, Object{{"canModifyBlocks", true}}});
    p.values("externalLinkSettings", {Value(), Object{{"url", ""}, {"displayName", ""}},
                                      Object{{"url", "https://example.com/lesson"}, {"displayName", "Lesson 1"}}});
}
