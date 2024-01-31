#include "Helpers.hpp"
#include "Main/BGM_SFX.hpp"

namespace CTRPluginFramework
{
     MenuEntry* lobbyBallAuto;

    u32 _lobbySong = 0xFFFFFFFF;

    StringVector percentages = {
        "125%",
        "100%",
        "75%",
        "50%",
        "25%",
        "10%",
        "Mute"
    };

    void BGM_SFX::bgmSet(MenuEntry* entry) {
        // not enough info known for this
    }

    void BGM_SFX::masterVolSet(MenuEntry* entry) {
        int sel = selVolPreset();

        Process::WriteFloat(AddressList::MasterVolume.addr, getFloatFromPercentSel(sel));
        entry->SetName("Master volume: " << getPercentAsStr(sel));
    }

    void BGM_SFX::bgmVolSet(MenuEntry* entry) {
        int sel = selVolPreset();

        Process::WriteFloat(AddressList::BGMVolume.addr, getFloatFromPercentSel(sel));
        entry->SetName("BGM volume: " << getPercentAsStr(sel));
    }

    void BGM_SFX::lobbyBallSong(MenuEntry* entry) {
        if (getSelSong() == 0xFFFFFFFF) { 
            int selectedSong = selLobbyBallSong();
            if (selectedSong >= 0) {
                u32 songEdit = 0xFFFFFF00 | selectedSong;

                storeSelSong(songEdit);
                entry->SetName("Disable Lobby Ball song selection");
                lobbyBallAuto->Enable();
            }
        }
        else {
            storeSelSong(0xFFFFFFFF);
            entry->SetName("Choose Lobby Ball song");
            lobbyBallAuto->Disable();
        }
    }

    void BGM_SFX::storeSelSong(u32 song) {
        _lobbySong = song;
    }

    u32 BGM_SFX::getSelSong(void) {
        return _lobbySong;
    }

    void BGM_SFX::writeLobbyBallSel(MenuEntry* entry) {
        Process::Write32(AddressList::LobbyBallSong.addr, getSelSong());
    }

    int BGM_SFX::selLobbyBallSong(void) {
        Keyboard songSel("Select a song.\n\nNote that these changes are client-side only,\nand will not be observable by other players\nonline.");
        songSel.Populate(GameData::lobbyBallSongs);

        return songSel.Open();
    }

    void BGM_SFX::voiceVol(MenuEntry* entry) {
        // not enough info known for this
    }
    
    void BGM_SFX::lowHPVol(MenuEntry* entry) {
        // not enough info known for this
    }

    std::string BGM_SFX::getPercentAsStr(int selection) {
        switch (selection) {
        case 0:
            return "125%";
        case 1:
            return "100%";
        case 2:
            return "75%";
        case 3:
            return "50%";
        case 4:
            return "25%";
        case 5:
            return "10%";
        case 6:
            return "Muted";
        default:
            return "100%";
        }
    }

    int BGM_SFX::selVolPreset(void) {
        Keyboard vol("Select your desired volume level.\n\nChanges will be reverted upon game restart.");
        vol.Populate(percentages);

        return vol.Open();
    }

    float BGM_SFX::getFloatFromPercentSel(int selection) {
        switch (selection) {
        case 0:
            return 1.25;
        case 1:
            return 1.0;
        case 2:
            return 0.75;
        case 3:
            return 0.5;
        case 4:
            return 0.25;
        case 5:
            return 0.1;
        case 6:
            return 0;
        default:
            return 1.0;
        }
    }
}