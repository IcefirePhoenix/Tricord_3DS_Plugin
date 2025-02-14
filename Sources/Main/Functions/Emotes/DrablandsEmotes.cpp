#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // Driver code for emote swapper in Drablands
    void Emotes::drablandEmoteSwapper(MenuEntry *entry)
    {
        // when entering new area, it's not possible to override current bottom-screen emote menu graphics...
        // restore default emote set to avoid button-graphic mix-ups...
        if (GeneralHelpers::isLoadingScreen(false))
        {
            initEmoteValueLayout(AddressList::getAddress("GameplayEmotes"), 0x03020100, 0x07060504, false, {0, 1, 2, 7, 8, 3, 4, 9});
            Emotes::forceDefaultEmotes(false);
            return;
        }

        // hotkey behavior for toggling between emote sets...
        if (entry->Hotkeys[0].IsPressed())
        {
            initEmoteValueLayout(AddressList::getAddress("GameplayEmotes"), 0x03020100, 0x07060504, false, {0, 1, 2, 7, 8, 3, 4, 9});
            Emotes::forceDefaultEmotes(true);
        }
        else if (entry->Hotkeys[1].IsPressed())
        {
            initEmoteValueLayout(AddressList::getAddress("GameplayEmotes"), 0x09080A0B, 0x0C060504, true, {11, 6, 5, 10, 8, 3, 4});
            Emotes::forceDefaultEmotes(true);
        }
    }
}