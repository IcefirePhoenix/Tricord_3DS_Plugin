#ifndef EMOTES_HPP
#define EMOTES_HPP

#include "CTRPluginFramework.hpp"
#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Unicode.h"

namespace CTRPluginFramework 
{
    void refreshEmoteGraphics(const u32 color[][4], std::vector<u8>& layout, bool useBlank);
    void initGraphicsRefresh(bool useBlankEmote);
	void initEmoteValueLayout(u32 address, u32 buttonID_A, u32 buttonID_B, bool isBlankEmote, std::vector<u8> pointerVector);
	void disableDefaultEmotes(void);

    class Emotes {
    public:
        static void lobbyEmoteSwapper(MenuEntry* entry);
        static void drablandEmoteSwapper(MenuEntry* entry);
    };
}
#endif