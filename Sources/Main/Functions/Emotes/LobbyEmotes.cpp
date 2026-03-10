#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // Driver code for emote swapper in multiplayer lobby
    void Emotes::lobbyEmoteSwapper(MenuEntry *entry)
    {
        if (GeneralHelpers::isLoadingScreen(true))
            restoreDefaultEmotes();

        // hotkey behavior for toggling between emote sets...
        if (Level::getCurrLevel() == Level::levelIDFromName("Hytopia Castle"))
        {
            if (entry->Hotkeys[0].IsPressed())
            {
                initEmoteValueLayout(AddressList::getAddress("GameplayEmotes"), 0x05080A0B, 0x00000706, true, {11, 6, 5, 3, 4, 9, 0});
                toggleDefaultEmotes(true);
                replicateEditsForLobby();
            }
            else if (entry->Hotkeys[1].IsPressed())
            {
                initEmoteValueLayout(AddressList::getAddress("GameplayEmotes"), 0x04090100, 0x00000302, true, {0, 1, 10, 8, 2, 7, 0});
                toggleDefaultEmotes(true);
                replicateEditsForLobby();
            }
            else if (entry->Hotkeys[2].IsPressed())
            {
                Process::Write32(Emotes::graphicsAddresses[5], Emotes::blankEmotePtr);
                Process::Write8(AddressList::getAddress("GameplayEmotes") + 0x5, 0xC);
                Process::Write8(AddressList::getAddress("LobbyEmotes") + 0x5, 0xC);
                toggleDefaultEmotes(true);
            }
        }
    }
}