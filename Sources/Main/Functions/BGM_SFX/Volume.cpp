#include "Helpers.hpp"
#include "Main/BGM_SFX.hpp"

namespace CTRPluginFramework
{
    MenuEntry *masterVol;
    MenuEntry *BGMVol;
    MenuEntry *voiceVol;
    MenuEntry *lowHPVol;
    MenuEntry *emoteVol;

    StringVector percentages =
    {
        "125%",
        "100%",
        "75%",
        "50%",
        "25%",
        "10%",
        "Mute"
    };

    /* ------------------ */

    // Force-sets the current BGM track
    void BGM_SFX::bgmSet(MenuEntry *entry)
    {
        // TODO: not enough info known for this
    }

    // Helper function that returns the desired volume address
    // TODO: locate remainder addresses
    u32 getVolumeAddress(int entryID)
    {
        switch (entryID)
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

    // Returns volume selection as string title
    std::string getEntryShortName(int entryID)
    {
        switch (entryID)
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

    // Driver code for setting desired volume
    void BGM_SFX::volSet(MenuEntry *entry)
    {
        int sel = selVolPreset();
        if (sel >= 0)
        {
            // multiple MenuEntries call this function... to differentiate between them, use MenuEntry Arg value...
            int entryID = reinterpret_cast<int>(entry->GetArg());

            // using Arg value, get volume title and address...
            std::string originalName = getEntryShortName(entryID);
            u32 address = getVolumeAddress(entryID);

            // reflect edits in menu as confirmation...
            Process::WriteFloat(address, getFloatFromPercentSel(sel));
            entry->SetName(originalName << ": " << getPercentAsStr(sel));
        }
    }

    // Menu interface for selecting the desired volume percentage level
    int BGM_SFX::selVolPreset(void)
    {
        Keyboard vol("Select your desired volume level.\n\nChanges will be reverted upon game restart.");
        vol.Populate(percentages);
        return vol.Open();
    }

    // Returns user selection as string (for menu display confirmation)
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

    // Returns user selection as float (for memory writes)
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
}