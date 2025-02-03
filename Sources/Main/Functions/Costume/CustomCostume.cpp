#include "Helpers.hpp"
#include "Main/Costume.hpp"

namespace CTRPluginFramework
{
    /**
     * TODO: finish complete overhaul to ONLY use custom, imported costume models
     * Previous implementation allowing existing costumes to be placed in unused DLC slots was unnecessary...
     */

    MenuEntry *menuCostumeSlotA;
    MenuEntry *menuCostumeSlotB;
    MenuEntry *menuCostumeSlotC;
    MenuEntry *menuCostumeSlotD;
    MenuEntry *triggerCostumeSlots;
    MenuEntry *restoreGreatFairy;
    MenuEntry *manageCatalogSize;
    MenuEntry *initCustomCostumes;
    MenuEntry *writeCostumeIDToSlot;

    bool showSlots = false;
    bool customCostumeFilesLoaded = false;

    u32 catalogStartOffset = 0xE8;
    u32 CostumeCatalogLocation;
    u8 costumeCatalogSize, catalogIncSize, openedCustomSlots;

    const int Costume::unusedDLC_slotIDs[4] = { 0x26, 0x27, 0x28, 0x29 };

    const StringVector Costume::BCH_fileNames =
    {
        "costume_customA.bch",
        "costume_customB.bch",
        "costume_customC.bch",
        "costume_customD.bch"
    };

    int fileToSlotMapping[4] = {0x26, 0x27, 0x28, 0x29}; // default values; expected to change during runtime
    bool slotStatus[4] = {false};

    /* ------------------ */

    // TODO: Set default plugin menu entry names = custom costume names from user-created txt
    // TODO: Custom desc strings -> see below

    // Enables and disables up to four custom costume inventory slots
    void Costume::openCustomCostumeSlots(MenuEntry *entry)
    {
        if (showSlots)
        {
            disableCustomCostumeMenuOpt();
            entry->SetName("Open Custom Costume Slots");
        }
        else
        {
            openedCustomSlots = chooseSlotAddNum();

            addCustomSlotsToMenu(openedCustomSlots);
            checkCustomCostumeConflict();
            enableCustomCostumeOpt();

            entry->SetName("Close Custom Costume Slots");
        }
        showSlots = !showSlots;
    }

    // Forces priority of Great Fairy -- unused costume -- if previously in-use
    void checkCustomCostumeConflict()
    {
        std::string errMsg = ("Custom Costume Slot A is currently in-use by Restore Great Fairy Costume. Please disable Restore Great Fairy Costume in order to use this costume slot.\n\nNote: The option to configure Custom Costume Slot A has automatically been disabled in the menu.");

        if (restoreGreatFairy->IsActivated() && openedCustomSlots >= 0)
        {
            menuCostumeSlotA->CanBeSelected(false);
            MessageBox(Color::Gainsboro << "Error", errMsg)();
        }
    }

    // Disables custom costume slots
    void disableCustomCostumeMenuOpt(void)
    {
        menuCostumeSlotA->Hide();
        menuCostumeSlotB->Hide();
        menuCostumeSlotC->Hide();
        menuCostumeSlotD->Hide();

        initCustomCostumes->Disable();
        writeCostumeIDToSlot->Disable();

        openedCustomSlots = 0;
    }

    // Inits helper functions for enabling custom costume slots
    void enableCustomCostumeOpt(void)
    {
        manageCatalogSize->Enable();
        writeCostumeIDToSlot->Enable();

        if (!customCostumeFilesLoaded)
            initCustomCostumes->Enable();
    }

    // Updates plugin menu to display additional entries for managing any added costume slots
    void addCustomSlotsToMenu(int openSlots)
    {
        menuCostumeSlotA->Show();
        menuCostumeSlotA->CanBeSelected(true);

        // redundant; using a switch statement jumbles the entire menu's entry order...
        if (openedCustomSlots == 1)
            menuCostumeSlotB->Show();

        else if (openedCustomSlots == 2)
        {
            menuCostumeSlotB->Show();
            menuCostumeSlotC->Show();
        }

        else if (openedCustomSlots == 3)
        {
            menuCostumeSlotB->Show();
            menuCostumeSlotC->Show();
            menuCostumeSlotD->Show();
        }

        /*
        switch (openSlots)
        {
            case 3:
                menuCostumeSlotD->Show();
            case 2:
                menuCostumeSlotC->Show();
            case 1:
                menuCostumeSlotB->Show();
            default:
                menuCostumeSlotA->Show();
                menuCostumeSlotA->CanBeSelected(true);
                break;
        }
        */
    }

    // Menu interface for choosing number of custom costume slots to enable
    int chooseSlotAddNum(void)
    {
        const StringVector slotCount =
        {
            "1 slot",
            "2 slots",
            "3 slots",
            "4 slots"
        };

        Keyboard addSlots("Choose the number of costume slots to open:");
        addSlots.Populate(slotCount);

        return addSlots.Open();
    }

    // Menu interface for changing default mappings from slots A-D -> costume_custom A-D
    void Costume::changeSlotMappings(MenuEntry *entry)
    {
        const StringVector slotNames =
        {
            "Slot A",
            "Slot B",
            "Slot C",
            "Slot D"
        };

        Keyboard chooseSlot("Choose the slot to edit:");
        Keyboard chooseCostume("Choose the costume to use:");

        chooseSlot.Populate(slotNames);
        chooseCostume.Populate(Costume::BCH_fileNames);

        int chosenSlot = chooseSlot.Open();
        if (chosenSlot >= 0)
        {
            int costumeSelection = chooseCostume.Open();

            // TODO: not sure if this works... check?
            if (costumeSelection >= 0)
                fileToSlotMapping[chosenSlot] = Costume::unusedDLC_slotIDs[costumeSelection];
        }
    }

    // Dynamically updates the costume inventory size depending on number of enabled custom costume slots
    void Costume::manageCatalogSizeAuto(MenuEntry *entry)
    {
        u8 isObtained;
        u8 vanillaCostumeCount = 0x25;
        u32 catalogSizeOffset = 0xE4;

        // get catalog dynamic location from pointer...
        Process::Read32(AddressList::getAddress("CostumeCatalogPointer"), CostumeCatalogLocation);

        // determine total number of costume slots needed (vanilla and custom)...
        if (!GeneralHelpers::isNullPointer(CostumeCatalogLocation))
        {
            costumeCatalogSize = catalogIncSize = 0;
            for (u8 slotCount = 0x0; slotCount <= vanillaCostumeCount; slotCount++)
            {
                isObtained = 0;
                Process::Read8((AddressList::getAddress("CostumeObtainStatus") + slotCount), isObtained);

                if (isObtained)
                    costumeCatalogSize++;
            }

            // increase catalog size to accommodate custom costume slots B-D ONLY...
            for (int customSlots = 3; customSlots > 0; customSlots--)
            {
                if (slotStatus[customSlots] != false)
                    catalogIncSize++;
            }

            // check if slot A is in-use...
            if (slotStatus[0] != false || restoreGreatFairy->IsActivated() == true)
                catalogIncSize++;

            Process::Write8((CostumeCatalogLocation + catalogSizeOffset), (costumeCatalogSize + catalogIncSize));
        }
    }

    // Forces custom costume selection edits to reflect in costume inventory
    void Costume::writeToCostumeSlot(MenuEntry *entry)
    {
        if (!GeneralHelpers::isNullPointer(CostumeCatalogLocation))
        {
            u32 DLCAddressStart = CostumeCatalogLocation + catalogStartOffset + costumeCatalogSize;
            for (int iterator = 0; iterator < 4; iterator++)
            {
                if (slotStatus[iterator] != false)
                    Process::Write8(DLCAddressStart + (u8)iterator, fileToSlotMapping[iterator]);
            }
        }
    }

    // Init custom directory metadata for custom costume models
    void Costume::initCustomCostumesAuto(MenuEntry *entry)
    {
        std::string customCostumeDirectory = "/Tricord/Custom/Costumes/";
        std::string fileNames = "costume_customA\0\0\0costume_customB\0\0\0costume_customC\0\0\0costume_customD";
        std::string errMsgPrefix = "Missing: ";
        StringVector missingFiles;

        u32 RODataPaddingOffset = 0x14;
        u32 offsetBetweenFileNames = 0x12;

        bool showErrMsg = false;

        // define filenames in padding space...
        Process::WriteString(AddressList::getAddress("TextToRodata") + RODataPaddingOffset, fileNames, StringFormat::Utf8);

        // restore Slot A pointer if Great Fairy is not currently used -> directs to "costume_customA" rather than "costume_highfairy"...
        if (!restoreGreatFairy->IsActivated())
        {
            Process::Write32((AddressList::getAddress("UnusedCostumeDataPointers")), (AddressList::getAddress("TextToRodata") + 0x14));
            menuCostumeSlotA->CanBeSelected(true);
        }

        // create pointer references to all custom filenames...
        for (int customCostumeFileIndex = 0; customCostumeFileIndex < openedCustomSlots; customCostumeFileIndex++)
        {
            u32 filenamePtrOffset = RODataPaddingOffset + (offsetBetweenFileNames * customCostumeFileIndex);

            if (File::Exists(customCostumeDirectory + Costume::BCH_fileNames[customCostumeFileIndex]))
                Process::Write32((AddressList::getAddress("UnusedCostumeDataPointers")), (AddressList::getAddress("TextToRodata") + filenamePtrOffset));
            else
            {
                missingFiles.push_back(Costume::BCH_fileNames[customCostumeFileIndex]);
                showErrMsg = true;
            }
        }

        // notify user of any missing custom files...
        if (showErrMsg)
        {
            OSD::Notify(Color::Red << "ERROR");
            OSD::Notify(Color::Red << "Custom Costume files cannot be found or do");
            OSD::Notify(Color::Red << "not exist in the Tricord directory.");
            OSD::Notify(Color::Gainsboro << "Custom Costume loading will now be CANCELLED.");

            for (int iterator = 0; iterator < missingFiles.size(); iterator++)
            {
                OSD::Notify(Color::Yellow << missingFiles[iterator]);
            }

            entry->Disable();
            customCostumeFilesLoaded = false;
        }
        else
            customCostumeFilesLoaded = true;
    }

    // Force-restores the Great Fairy Costume, an unused costume intact within the game files
    void Costume::greatFairyEnable(MenuEntry *entry)
    {
        u8 greatFairySlotID = Costume::unusedDLC_slotIDs[0];

        std::string greatFairyFileName = "costume_highfairy";
        std::string errMsg = "Tricord cannot place Great Fairy Costume into Custom Costume Slot A since it is currently configured to display a custom costume.\n\nWould you like to overwrite the custom costume in Custom Costume Slot A with Great Fairy?";

        // create pointer reference to Great Fairy filename...
        Process::WriteString(AddressList::getAddress("TextToRodata"), greatFairyFileName, StringFormat::Utf8);

        manageCatalogSize->Enable();

        // handle conflicts if Slot A is already in-use...
        if (slotStatus[0] != true)
        {
            if (MessageBox(Color::Gainsboro << "Error", errMsg, DialogType::DialogYesNo)())
            {
                // overwrite Slot A current custom costume with Great Fairy...
                menuCostumeSlotA->CanBeSelected(false);
                slotStatus[0] = true;
            }
            else
            {
                // cancel Great Fairy restore and keep current Slot A custom costume...
                entry->Disable();
                return;
            }
        }
        else
        {
            if (!GeneralHelpers::isNullPointer(CostumeCatalogLocation))
            {
                Process::Write32((AddressList::getAddress("UnusedCostumeDataPointers")), AddressList::getAddress("TextToRodata"));
                Process::Write8((CostumeCatalogLocation + catalogStartOffset + costumeCatalogSize), greatFairySlotID);
            }
        }
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