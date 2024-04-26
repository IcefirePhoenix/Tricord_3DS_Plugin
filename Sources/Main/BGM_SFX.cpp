#include "Helpers.hpp"
#include "Main/BGM_SFX.hpp"

namespace CTRPluginFramework
{
    MenuEntry* masterVol;
    MenuEntry* BGMVol;
    MenuEntry* voiceVol;
    MenuEntry* lowHPVol;
    MenuEntry* emoteVol;
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

    void BGM_SFX::bgmSet(MenuEntry* entry) 
    {
        // not enough info known for this
    }

    u32 getSoundAddress(int entryID) 
    {
        switch(entryID)
        {
        case 0:
            return AddressList::MasterVolume.addr;
        case 1:
            return AddressList::BGMVolume.addr;
        case 2:
        case 3:
        case 4:
        default:
            return 0;
        }
    }

    std::string getEntryShortName(int entryID)
    {
        switch(entryID)
        {
        case 0:
            return "Master volume";
        case 1:
            return "BGM volume";
        case 2:
            return "Link Voice volume";
        case 3:
            return "Low Health Alert volume";
        case 4:
            return "Emote volume";
        default:
            return "";
        }
    }

    void BGM_SFX::volSet(MenuEntry* entry) 
    {
        int sel = selVolPreset();
        if (sel >= 0)
        {
            int entryID = reinterpret_cast<int>(entry->GetArg());

            std::string originalName = getEntryShortName(entryID); 
            u32 address = getSoundAddress(entryID); 
        
            Process::WriteFloat(address, getFloatFromPercentSel(sel));
            entry->SetName(originalName << ": " << getPercentAsStr(sel));
        }
    }

    // TODO: bug fix
    void BGM_SFX::lobbyBallSong(MenuEntry* entry) 
    {
        if (getSelSong() == 0xFFFFFFFF) 
        { 
            int selectedSong = selLobbyBallSong();
            if (selectedSong >= 0) 
            {
                u32 songEdit = 0x00000000 | selectedSong;

                storeSelSong(songEdit);
                entry->SetName("Disable Lobby Ball song selection");
                lobbyBallAuto->Enable();
            }
        }
        else 
        {
            storeSelSong(0xFFFFFFFF);
            entry->SetName("Choose Lobby Ball song");
            lobbyBallAuto->Disable();
        }
    }

    void BGM_SFX::storeSelSong(u32 song) 
    {
        _lobbySong = song;
    }

    u32 BGM_SFX::getSelSong(void) 
    {
        return _lobbySong;
    }

    u32 BGM_SFX::getLobbyBallSongAddress(void)
    {
        u32 lobbyBallSongOffset = 0x6CC;
        return (GameData::getLobbyBallDataAddress() + lobbyBallSongOffset);
    }

    void BGM_SFX::writeLobbyBallSel(MenuEntry* entry) 
    {
        u32 finalSongAddress = getLobbyBallSongAddress();

        if (!GeneralHelpers::isNullPointer(finalSongAddress))
            Process::Write32(getLobbyBallSongAddress(), getSelSong());
    }

    int BGM_SFX::selLobbyBallSong(void) 
    {
        Keyboard songSel("Select a song.\n\nNote that these changes are client-side only,\nand will not be observable by other players\nonline.");
        songSel.Populate(GameData::lobbyBallSongs);

        return songSel.Open();
    }

    std::string BGM_SFX::getPercentAsStr(int selection) 
    {
        switch (selection) 
        {
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

    int BGM_SFX::selVolPreset(void) 
    {
        Keyboard vol("Select your desired volume level.\n\nChanges will be reverted upon game restart.");
        vol.Populate(percentages);

        return vol.Open();
    }

    float BGM_SFX::getFloatFromPercentSel(int selection) 
    {
        switch (selection) 
        {
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
    
    // Force normal or 8-bit BGMs
    void BGM_SFX::forceNormal8bit(MenuEntry* entry)
    {
        Keyboard ForceBGMType("Force Normal or 8-bit BGM, or\nReset to restore the Timeless Tunic check.");
        static const StringVector BGMTypeOptions = 
        {
            "Reset",
            "Normal BGM",
            "8-bit BGM"
        };
        ForceBGMType.Populate(BGMTypeOptions);
        switch (ForceBGMType.Open()){
            case 0:
                // Default BEQ
                Process::Patch(AddressList::BGMType.addr, 0x0A00003B);
                break;
            case 1:
                // Unconditional branch
                Process::Patch(AddressList::BGMType.addr, 0xEA00003B);
                break;
            case 2:
                // Noop
                Process::Patch(AddressList::BGMType.addr, 0xE320F000);
                break;
        }
    }

    // Disable Timeless Tunic voice filter
    void BGM_SFX::disable8bitVoice(MenuEntry* entry)
    {
        if (entry->Name() == "Disable Timeless Tunic voice filter"){
            Process::Patch(AddressList::Voice8bit.addr, 0xEA000006);
            entry->SetName("Enable Timeless Tunic voice filter");
        } else {
            Process::Patch(AddressList::Voice8bit.addr, 0x1A000006);
            entry->SetName("Disable Timeless Tunic voice filter");
        }
    }
}
