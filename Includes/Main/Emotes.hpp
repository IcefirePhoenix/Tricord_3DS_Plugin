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
	void initEmoteValueLayout(u32 address, u32 buttonID_A, u32 buttonID_B, bool isBlankEmote, std::vector<u8> pointerVector);
    void restoreDefaultEmotes(void);
    void replicateEditsForLobby(void);
    void toggleDefaultEmotes(bool shouldDisable);

    class Emotes
    {
    public:
        static void initEmoteAddresses(void);
        static void lobbyEmoteSwapper(MenuEntry* entry);
        static void drablandEmoteSwapper(MenuEntry* entry);

        static const u32 pointerIndexes[12][3];

        static const u32 blankEmotePtr = 0x21800000;
        static u32 graphicsAddresses[8];
    };
}
#endif