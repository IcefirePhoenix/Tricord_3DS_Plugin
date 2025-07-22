#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // Driver code for emote swapper in Drablands
    void Emotes::drablandEmoteSwapper(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
            initEmoteAddresses();

        if (GeneralHelpers::isLoadingScreen(true))
            restoreDefaultEmotes();

        // hotkey behavior for toggling between emote sets...
        if (Level::getCurrLevel() != Level::levelIDFromName("Hytopia Castle"))
        {
            if (entry->Hotkeys[0].IsPressed())
            {
                initEmoteValueLayout(AddressList::getAddress("GameplayEmotes"), 0x03020100, 0x07060504, false, {0, 1, 2, 7, 8, 3, 4, 9});
                toggleDefaultEmotes(true);
            }
            else if (entry->Hotkeys[1].IsPressed())
            {
                initEmoteValueLayout(AddressList::getAddress("GameplayEmotes"), 0x09080A0B, 0x0C060504, true, {11, 6, 5, 10, 8, 3, 4});
                toggleDefaultEmotes(true);
            }
        }
    }
}