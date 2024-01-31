#ifndef EMOTES_HPP
#define EMOTES_HPP

#include "CTRPluginFramework.hpp"
#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Unicode.h"

namespace CTRPluginFramework {
    void refreshEmoteGraphics(const u32 color[][4], std::vector<u8>& layout, bool useBlank);

    class Emotes {
    public:
        static void lobbyEmoteSwapper(MenuEntry* entry);
        static void drablandEmoteSwapper(MenuEntry* entry);
        static void customEmotes(MenuEntry* entry);
    };
}
#endif