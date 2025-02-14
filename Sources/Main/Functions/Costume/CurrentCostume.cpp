#include "Helpers.hpp"
#include "Main/Costume.hpp"

namespace CTRPluginFramework
{
    MenuEntry *doppelCostumeStageResetManager;

    /* ------------------ */

    // TODO: edit plugin menu layout to have current costumes as separate menu entries with dynamic titles

    // Driver code for changing the current costume for a specific player | TODO: add support for custom costumes IF they are enabled...
    void Costume::changeLinkCostume(MenuEntry *entry)
    {
        int linkChoice = GeneralHelpers::chooseLink();

        if (linkChoice >= 0)
        {
            Keyboard costumeList("Choose a costume:\n\nBe sure to load into a new area for changes to fully\ntake effect.");
            costumeList.Populate(GameData::universalCostumeList);

            Costume::setPlayerCostume(linkChoice, costumeList.Open());
        }
    }

    // Helper method for setting current costume; by default, this accesses both primary + alternate costume IDs
    void Costume::setPlayerCostume(int player, int costumeID, bool useAltOnly)
    {
        if (costumeID >= 0)
        {
            if (!useAltOnly)
                Process::Write8((AddressList::getAddress("CurrCostume") + player * PLAYER_OFFSET), costumeID);

            // note: loading screen updates require both addresses to be set if cosmetic costumes are enabled...
            Process::Write8((AddressList::getAddress("CurrCostumeAlt") + player * PLAYER_OFFSET), costumeID);
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
                result = costumeList.Open() + GameData::maxCostumeCount; // get 0x26-0x29 range
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
                Process::Patch(AddressList::getAddress("BearMinMaxCheckA"), 0xA3A00001);
                Process::Patch(AddressList::getAddress("BearMinMaxCheckB"), 0xB3A00000);
                break;
            case 1: // force-disable upgrade...
                Process::Patch(AddressList::getAddress("BearMinMaxCheckA"), 0xA3A00000);
                Process::Patch(AddressList::getAddress("BearMinMaxCheckB"), 0xB3A00000);
                break;
            case 2: // force-enable upgrade..
                Process::Patch(AddressList::getAddress("BearMinMaxCheckA"), 0xA3A00001);
                Process::Patch(AddressList::getAddress("BearMinMaxCheckB"), 0xB3A00001);
                break;
            default:
                break;
        }
    }

    // Allows edits made to Doppel costumes to persist after entering the single-player lobby
    void Costume::preventDoppelLobbyReset(MenuEntry *entry)
    {
        u32 forceDisableReset = 0xEA000007;
        u32 defaultResetBehavior = 0x1A000007;

        if (entry->Name() == "Prevent Doppel Costume resets")
        {
            Process::Patch(AddressList::getAddress("DoppelLobbyReset"), forceDisableReset);
            entry->SetName("Allow Doppel Costume resets");
        }
        else
        {
            Process::Patch(AddressList::getAddress("DoppelLobbyReset"), defaultResetBehavior);
            entry->SetName("Prevent Doppel Costume resets");
        }
    }

    void Costume::preventDoppelStageReset(MenuEntry *entry)
    {
        // TODO: impl using Process::Patch w/buffer ptr + CopyMem to reset
        if (!entry->IsActivated())
        {
            Process::Patch(AddressList::getAddress("DoppelStageResetA"), 0xE3500000);
            Process::Patch(AddressList::getAddress("DoppelStageResetB"), 0x13A00018);
            Process::Patch(AddressList::getAddress("DoppelStageResetC"), 0x15C40064);
        }

        if (entry->WasJustActivated())
        {
            Process::Patch(AddressList::getAddress("DoppelStageResetA"), 0xE320F000);
            Process::Patch(AddressList::getAddress("DoppelStageResetB"), 0xE320F000);
            Process::Patch(AddressList::getAddress("DoppelStageResetC"), 0xE320F000);
        }
    }
}