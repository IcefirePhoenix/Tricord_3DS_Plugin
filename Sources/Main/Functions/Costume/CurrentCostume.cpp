#include "Helpers.hpp"
#include "Main/Costume.hpp"

namespace CTRPluginFramework
{
    // TODO: edit plugin menu layout to have current costumes as separate menu entries with dynamic titles

    // Driver code for changing the current costume for a specific player | TODO: add support for custom costumes IF they are enabled...
    void Costume::changeLinkCostume(MenuEntry *entry)
    {
        int linkChoice = GeneralHelpers::chooseLink();

        if (linkChoice >= 0)
        {
            u32 playerID = (u32)linkChoice;
            u32 memoryOffset = playerID * PLAYER_OFFSET;

            Keyboard costumeList("Choose a costume:\n\nBe sure to load into a new area for changes to fully\ntake effect.");
            costumeList.Populate(GameData::universalCostumeList);

            int result = costumeList.Open();

            if (result >= 0)
                Process::Write8((AddressList::CurrCostume.addr + memoryOffset), result);
        }
    }

    // TODO: Refactor to add support for custom costumes:
    void Costume::selectCostumeID(MenuEntry *entry)
    {
        const StringVector type =
        {
            "Original Costumes",
            "Custom Costumes"
        };

        u8 result;
        u8 unusedRange = 0x26;

        Keyboard costumeType("Choose the desired costume type:");
        Keyboard costumeList("Choose a costume:");

        costumeType.Populate(type);
        int choice = costumeType.Open();

        switch (choice)
        {
            case 0:
                costumeList.Populate(GameData::universalCostumeList);
                result = costumeList.Open();
                break;
            case 1:
                costumeList.Populate(GameData::customCostumeList);
                result = costumeList.Open() + unusedRange; // get 0x26-0x29 range
                break;
            default:
                break;
        }
    }

    // Forces Bear Minimum/Maximum upgrade status regardless of hero point count
    void Costume::forceBearMinMax(MenuEntry *entry)
    {
        const StringVector bearOptions =
        {
            "Reset",
            "Bear Minimum",
            "Bear Maximum"
        };

        Keyboard bearStatus("Force Bear Minimum or Bear Maximum, or\nReset to restore the Hero Point check.");
        bearStatus.Populate(bearOptions);

        switch (bearStatus.Open())
        {
            case 0: // default -> upgrade check for 30+ hero points...
                Process::Patch(AddressList::BearMinMaxCheckA.addr, 0xA3A00001);
                Process::Patch(AddressList::BearMinMaxCheckB.addr, 0xB3A00000);
                break;
            case 1: // force-disable upgrade...
                Process::Patch(AddressList::BearMinMaxCheckA.addr, 0xA3A00000);
                Process::Patch(AddressList::BearMinMaxCheckB.addr, 0xB3A00000);
                break;
            case 2: // force-enable upgrade..
                Process::Patch(AddressList::BearMinMaxCheckA.addr, 0xA3A00001);
                Process::Patch(AddressList::BearMinMaxCheckB.addr, 0xB3A00001);
                break;
            default:
                break;
        }
    }

    // Allows edits made to Doppel costumes to persist after entering the single-player lobby
    void Costume::preventLobbyReset(MenuEntry *entry)
    {
        u32 forceDisableReset = 0xEA000007;
        u32 defaultResetBehavior = 0x1A000007;

        if (entry->Name() == "Prevent Doppel Costume resets")
        {
            Process::Patch(AddressList::DoppelLobbyReset.addr, forceDisableReset);
            entry->SetName("Allow Doppel Costume resets");
        }
        else
        {
            Process::Patch(AddressList::DoppelLobbyReset.addr, defaultResetBehavior);
            entry->SetName("Prevent Doppel Costume resets");
        }
    }
}