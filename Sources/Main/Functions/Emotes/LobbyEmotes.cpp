#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // Driver code for emote swapper in multiplayer lobby
    void Emotes::lobbyEmoteSwapper(MenuEntry *entry)
    {
        // when previous location is NOT Drablands, emote menu data defaults to Drablands emote set for some reason...
        u32 finalAddress = (Level::getPrevLevel() < Level::levelIDFromName("Deku Forest")) ? AddressList::getAddress("GameplayEmotes") : AddressList::getAddress("LobbyEmotes");

        // when entering new area, it's not possible to override current bottom-screen emote menu graphics...
        // restore default emote set to avoid button-graphic mix-ups...
        if (GeneralHelpers::isLoadingScreen(false))
        {
            initEmoteValueLayout(finalAddress, 0x05080A0B, 0x00000706, true, {11, 6, 5, 3, 4, 9, 0});
            forceDefaultEmotes(false);
            return;
        }

        // hotkey behavior for toggling between emote sets...
        if (Level::getCurrLevel() == Level::levelIDFromName("Hytopia Castle"))
        {
            if (entry->Hotkeys[0].IsPressed())
            {
                initEmoteValueLayout(finalAddress, 0x05080A0B, 0x00000706, true, {11, 6, 5, 3, 4, 9, 0});
                forceDefaultEmotes(true);
            }
            else if (entry->Hotkeys[1].IsPressed())
            {
                initEmoteValueLayout(finalAddress, 0x04090100, 0x00000302, true, {0, 1, 10, 8, 2, 7, 0});
                forceDefaultEmotes(true);
            }
            else if (entry->Hotkeys[2].IsPressed())
            {
                Process::Write32(Emotes::graphicsAddresses[5], blankEmotePointer);
                Process::Write8(finalAddress + 0x5, 0xC);
                forceDefaultEmotes(true);
            }
        }
    }
}