#include "Helpers.hpp"
#include "Main/BGM_SFX.hpp"

namespace CTRPluginFramework
{
    MenuEntry *lobbyBallAuto;

    u32 noSong = 0xFFFFFFFF;
    u32 lobbySong = noSong;

    /* ------------------ */

    // Force-changes the current Lobby Ball song
    void BGM_SFX::lobbyBallSong(MenuEntry *entry)
    {
        if (lobbySong == noSong)
        {
            int selectedSong = selLobbyBallSong();
            if (selectedSong >= 0)
            {
                u32 songEdit = 0x00000000 | selectedSong;
                lobbySong = songEdit;

                entry->SetName("Disable Lobby Ball song selection");
                lobbyBallAuto->Enable();
            }
        }
        else
        {
            entry->SetName("Choose Lobby Ball song");
            lobbyBallAuto->Disable();
            lobbySong = noSong;
        }
    }

    // Helper function to write song changes to memory
    void BGM_SFX::writeLobbyBallSel(MenuEntry *entry)
    {
        u32 lobbyBallSongOffset = 0x6CC;
        u32 dynamicAddr, finalSongAddress;

        Process::Read32(AddressList::LobbyBallPointer.addr, dynamicAddr);
        finalSongAddress = dynamicAddr + lobbyBallSongOffset;

        if (!GeneralHelpers::isNullPointer(finalSongAddress))
            Process::Write32(finalSongAddress, lobbySong);
    }

    // Helper function to open song selection interface
    int BGM_SFX::selLobbyBallSong(void)
    {
        Keyboard songSel("Select a song.\n\nNote that these changes are client-side only,\nand will not be observable by other players\nonline.");
        songSel.Populate(GameData::lobbyBallSongs);
        return songSel.Open();
    }
}