#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/GeneralHelpers.hpp"
#include "Helpers/GameData.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
    StringVector percentages = {
        "125%",
        "100%",
        "75%",
        "50%",
        "25%",
        "10%",
        "Mute"
    };

    void bgmSet(MenuEntry* entry) {
        // not enough info known for this
    }

    void masterVolSet(MenuEntry* entry) {
        int sel = selVolPreset();

        Process::WriteFloat(AddressList::MasterVolume.addr, getFloatFromPercentSel(sel));
        entry->SetName("Master volume: " << getPercentAsStr(sel));
    }

    void bgmVolSet(MenuEntry* entry) {
        int sel = selVolPreset();

        Process::WriteFloat(AddressList::BGMVolume.addr, getFloatFromPercentSel(sel));
        entry->SetName("BGM volume: " << getPercentAsStr(sel));
    }

    u32 _lobbySong = 0xFFFFFFFF;

    void lobbyBallSong(MenuEntry* entry) {
        if (getSelSong() == 0xFFFFFFFF) { 
            int selectedSong = selLobbyBallSong();
            if (selectedSong > 0) {
                u32 songEdit = 0xFFFFFF00 | selectedSong;

                storeSelSong(songEdit);
                entry->SetName("Disable Lobby Ball song selection");
            }
        }
        else {
            storeSelSong(0xFFFFFFFF);
            entry->SetName("Choose Lobby Ball song");
        }
    }

    void storeSelSong(u32 song) {
        _lobbySong = song;
    }

    u32 getSelSong(void) {
        return _lobbySong;
    }

    void writeLobbyBallSel(MenuEntry* entry) {
        Process::Write32(AddressList::LobbyBallSong.addr, getSelSong());
    }

    int selLobbyBallSong(void) {
        Keyboard songSel("Select a song.\n\nNote that these changes are client-side only,\nand will not be observable by other players\nonline.");
        songSel.Populate(GameData::lobbyBallSongs);

        return songSel.Open();
    }

    void voiceVol(MenuEntry* entry) {
        // not enough info known for this
    }
    
    void lowHPVol(MenuEntry* entry) {
        // not enough info known for this
    }

    std::string getPercentAsStr(int selection) {
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

    int selVolPreset(void) {
        Keyboard vol("Select your desired volume level.\n\nChanges will be reverted upon game restart.");
        vol.Populate(percentages);

        return vol.Open();
    }

    float getFloatFromPercentSel(int selection) {
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