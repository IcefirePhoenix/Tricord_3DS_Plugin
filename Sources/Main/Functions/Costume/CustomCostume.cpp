#include "Helpers.hpp"
#include "Main/Costume.hpp"

namespace CTRPluginFramework
{
    MenuEntry *DLC_SlotWriterA, *TricordUseDLC_SlotA;

    u8 unusedCostumeSlotID_A = 0x26, bitstringEdit = 0x7F;
    std::string fairyName = "Great Fairy Costume", customName = "Custom Costume";

    /* ------------------ */

    // Checks if the user has imported a custom costume
    bool getCustomCostumeLoaderStatus(void)
    {
        std::string path = Utils::Format("/luma/titles/%s/romfs/Common/Costume/costume_highfairy.bch", Hex(Process::GetTitleID()).c_str());
        return File::Exists(path);
    }

    // Updates plugin's costume reference data
    void updatePluginCostumeData(bool addCostume)
    {
        std::string workingName = getCustomCostumeLoaderStatus() ? customName : fairyName;

        if (addCostume)
        {
            GameData::maxCostumeCount = 39;
            if (GameData::universalCostumeList.back() == "Fierce Deity Armor")
                GameData::universalCostumeList.push_back(workingName);
        }
        else
        {
            GameData::maxCostumeCount = 38;
            if (GameData::universalCostumeList.back() == workingName)
                GameData::universalCostumeList.pop_back();
        }
    }

    // Restores reference to costume_highfairy, to be placed in the first unused DLC slot
    bool restoreGreatFairyPtrRef(void)
    {
        std::string greatFairyFileName = "costume_highfairy";

        // create pointer reference to Great Fairy filename...
        bool stringWriteClear = Process::WriteString(AddressList::getAddress("HighfairyReference"), greatFairyFileName, StringFormat::Utf8);
        bool ptrWriteClear = Process::Write32((AddressList::getAddress("UnusedCostumeDataPointers")), AddressList::getAddress("HighfairyReference"));

        return stringWriteClear && ptrWriteClear;
    }

    // Sets the owned status for first unused DLC costume
    bool forceOwnDLC_SlotA(void)
    {
        u32 finalAddr = AddressList::getAddress("CostumeObtainStatus") + unusedCostumeSlotID_A;
        return Process::Write8(finalAddr, true);
    }

    // Increases maximum available costume slot count by 1
    bool increaseCatalogSize(void)
    {
        return Process::Write8(AddressList::getAddress("CostumeCatalogMaxSlot"), bitstringEdit);
    }

    // Restores catalog data
    void restoreData(void)
    {
        u8 restoreBitstringEdit = 0x3F;

        Process::Write8(AddressList::getAddress("CostumeCatalogMaxSlot"), restoreBitstringEdit);
        Process::Write8(AddressList::getAddress("CostumeObtainStatus") + unusedCostumeSlotID_A, false);
    }

    // Force-enables an additional slot in the custom selection menu using the first unused DLC slot
    void Costume::overrideDLC_CostumeSlotA(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
        {
            if (!restoreGreatFairyPtrRef() || !forceOwnDLC_SlotA() || !increaseCatalogSize())
            {
                OSD::Notify("[ERROR] Cannot init additional costume slot. Cancelling...");
                entry->Disable();
            }
            else
            {
                updatePluginCostumeData(TricordUseDLC_SlotA->IsActivated());
            }
        }
        else if (!entry->IsActivated())
        {
            updatePluginCostumeData(false);
            restoreData();
        }
    }

    // Determines whether to update costume data
    void Costume::toggleTricordCustomCostumeUsage(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
        {
            updatePluginCostumeData(true);
        }
        else if (!entry->IsActivated())
        {
            updatePluginCostumeData(false);
        }
    }

    // Updates Custom Costume-related MenuEntry titles
    void Costume::setDLCEntryTitles(void)
    {
        std::string useToggleTitle = "Allow Tricord to use the ";
        std::string workingName = getCustomCostumeLoaderStatus() ? customName : fairyName;

        std::string &writerNote = DLC_SlotWriterA->Note();
        std::string &useToggleNote = TricordUseDLC_SlotA->Note();

        DLC_SlotWriterA->SetName(getCustomCostumeLoaderStatus() ? Utils::Format("Restore %s", workingName.c_str()) : Utils::Format("Enable %s Slot", workingName.c_str()));
        TricordUseDLC_SlotA->SetName(useToggleTitle + workingName);

        writerNote = getCustomCostumeLoaderStatus() ? DescUtils::getDesc("custom_costume_note") : DescUtils::getDesc("restore_fairy_note");
        useToggleNote = getCustomCostumeLoaderStatus() ? DescUtils::getDesc("allow_Tricord_custom_note") : DescUtils::getDesc("allow_Tricord_fairy_note");

        DLC_SlotWriterA->RefreshNote();
        TricordUseDLC_SlotA->RefreshNote();
    }

    /**
     * TODO: Locate MSBT pointers -> overwrite/add references to custom desc strings
     *
     * Costume Name: Custom Costume #1/2/3/4 OR custom names defined in a user-created txt?
     *
     * Costume Description:
     * "A custom costume imported in-game via the Tricord plugin!
     * This otherworldly garb comes with no powers, but it looks quite dapper!"
     *
     * void Costume::initCustomDescriptions(MenuEntry *entry){}
     */
}