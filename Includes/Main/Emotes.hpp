#ifndef EMOTES_HPP
#define EMOTES_HPP

#include "CTRPF.hpp"
#include "Cheats.hpp"
#include "Helpers.hpp"
#include "Unicode.h"

namespace CTRPluginFramework
{
    /**
     * TODO: look into possible pointers to get rid of these hardcoded arrays...
     * Also, values may break if custom files are used...
     * Also also, other regions need support for blank emote...
     */

    void refreshEmoteGraphics(const u32 color[][4], std::vector<u8>& layout, bool useBlank);
    void initGraphicsRefresh(bool useBlankEmote);
	void initEmoteValueLayout(u32 address, u32 buttonID_A, u32 buttonID_B, bool isBlankEmote, std::vector<u8> pointerVector);
    void initEmoteAddresses(void);

    class Emotes
    {
    public:
        static void lobbyEmoteSwapper(MenuEntry* entry);
        static void drablandEmoteSwapper(MenuEntry* entry);
        static void forceDefaultEmotes(bool shouldDisable);

        // column order: NA, EU, JP
        static const u32 greenEmotePointers[12][3];
        static const u32 blueEmotePointers[12][3];
        static const u32 redEmotePointers[12][3];

        static const u32 blankEmotePointer = 0x2070A780;
        static u32 graphicsAddresses[8];
    };
}
#endif