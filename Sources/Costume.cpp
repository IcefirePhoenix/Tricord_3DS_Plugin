#include "cheats.hpp"
#include "csvc.h"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/PlayerHelper.hpp"

#include "CTRPluginFramework/Graphics/OSD.hpp"
#include "CTRPluginFramework/Menu/MessageBox.hpp"

#include "CTRPluginFrameworkImpl/Menu.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/System/FSPath.hpp"

#include <CTRPluginFramework.hpp>
#include <array>
#include "3ds.h"

namespace CTRPluginFramework
{
    bool showSlots = false;
    bool isCostumeFail = false;

    u8 CustomSlots[4] = { 255, 255, 255, 255 };

    u32 catalogStartOffset = 0xE8;
    u32 CostumeCatalogLocation;
    u8 costumeCatalogSize, catalogIncSize;

    void openCustomCostumeSlots(MenuEntry* entry) {
        if (showSlots) {
            disableCustomCostumeMenuOpt();
            resetCostumeArray();

            entry->SetName("Open Custom Costume Slots");
        }
        else {
            int openedCustomSlots = chooseSlotAddNum();

            addCustomSlotsToMenu(openedCustomSlots);
            isCustomCostumeConflict(openedCustomSlots);      
            enableCustomCostumeOpt();

            entry->SetName("Close Custom Costume Slots");
        }
        showSlots = !showSlots;
    }

    void disableCustomCostumeMenuOpt(void) {
        menuCostumeSlotA->Hide();
        menuCostumeSlotB->Hide();
        menuCostumeSlotC->Hide();
        menuCostumeSlotD->Hide();

        initCustomCostumes->Disable();
        writeCostumeIDToSlot->Disable();
    }

    void enableCustomCostumeOpt(void) {
        manageCatalogSize->Enable();
        writeCostumeIDToSlot->Enable();

        if (!isCostumeFail)
            initCustomCostumes->Enable();
    }

    bool isCustomCostumeConflict(int openSlots) {
        if (restoreGreatFairy->IsActivated() && openSlots >= 0) {
            menuCostumeSlotA->CanBeSelected(false);

            std::string errMsg = ("Custom Costume Slot A is currently in-use by Restore Great Fairy Costume. Please disable Restore ");
            errMsg += ("Great Fairy Costume in order to use this costume slot.\n\nNote: The option to configure Custom Costume Slot A ");
            errMsg += ("has automatically been disabled in the menu.");

            MessageBox(Color::Gainsboro << "Error", errMsg)();

            return true;
        }
        return false;
    }

    void addCustomSlotsToMenu(int openSlots) {
        menuCostumeSlotA->Show();
        menuCostumeSlotA->CanBeSelected(true);

        // redundant; using a switch statement jumbles the entire menu's entry order...
        if (openSlots == 1) {
            menuCostumeSlotB->Show();
        }
        else if (openSlots == 2) {
            menuCostumeSlotB->Show();
            menuCostumeSlotC->Show();
        }
        else if (openSlots == 3) {
            menuCostumeSlotB->Show();
            menuCostumeSlotC->Show();
            menuCostumeSlotD->Show();
        }

        /*
        switch (openSlots) {
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

    void resetCostumeArray(void) {
        for (int i = 0; i < 4; i++) {
            CustomSlots[i] = 255;
        }
    }

    int chooseSlotAddNum(void) {
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

    void manageCatalogSizeAuto(MenuEntry* entry) {
        u8 isObtained;
        u32 catalogSizeOffset = 0xE4;

        // get catalog dynamic location from pointer
        Process::Read32(AddressList::CostumeCatalogPointer.addr, CostumeCatalogLocation);

        if (!isCatalogPointerNull()) {
            costumeCatalogSize = catalogIncSize = 0;

            for (u32 i = 0x0; i < 0x26; i++) {
                isObtained = 0;

                Process::Read8((AddressList::CostumeObtainStatus.addr + i), isObtained);

                if (isObtained) {
                    costumeCatalogSize++;
                }
            }

            // increase catalog size to accommodate custom costume slots B-D
            for (int i = 3; i > 0; --i) {
                if (CustomSlots[i] != 255) {
                    catalogIncSize++;
                }
            }

            // check if slot A is in-use
            if (CustomSlots[0] != 255 || restoreGreatFairy->IsActivated() == true) {
                catalogIncSize++;
            }

            Process::Write8((CostumeCatalogLocation + catalogSizeOffset), (costumeCatalogSize + catalogIncSize));
        }
    }

    void selectCostumeID(MenuEntry *entry) {
        u8 result;
        u8 unusedRange = 0x26;

        Keyboard costumeType("Choose the desired costume type:");
        Keyboard costumeList("Choose a costume:");

        const StringVector type =
        {
            "Original Costumes",
            "Custom Costumes"
        };

        costumeType.Populate(type);
        int choice = costumeType.Open();

        if (choice == 0) {
            costumeList.Populate(universalCostumeList);
            result = costumeList.Open();
        }
        else {
            costumeList.Populate(customCostumeList);
            result = costumeList.Open() + unusedRange; // get 0x26-0x29 range
        }

        if (entry->Name() == "   Set custom costume slot A") {
            CustomSlots[0] = result;
        }
        else if (entry->Name() == "   Set custom costume slot B") {
            CustomSlots[1] = result;
        }
        else if (entry->Name() == "   Set custom costume slot C") {
            CustomSlots[2] = result;
        }
        else if (entry->Name() == "   Set custom costume slot D") {
            CustomSlots[3] = result;
        }
    }

    bool isCatalogPointerNull(void) {
        return CostumeCatalogLocation == 0x00000000;
    }

    void writeCostumeSlot(MenuEntry* entry) {
        if (!isCatalogPointerNull()) {
            u32 DLCAddressStart = CostumeCatalogLocation + catalogStartOffset + costumeCatalogSize;

            for (u32 iterator = 0x0; iterator < 0x4; ++iterator) {
                if (CustomSlots[iterator] != 255) {
                    Process::Write8(DLCAddressStart + iterator, CustomSlots[iterator]);
                }
            }
        }
    }

    // TODO: costume names + descriptions? locate msbt pointers
    // Example:
    // Custom Costume #1
    // "A custom costume imported in-game via the Tricord plugin!
    // This otherworldly garb comes with no powers, but it looks quite dapper!"
    void initCustomCostumesAuto(MenuEntry* entry) {
        std::string customCostumeDirectory = "/Tricord/Custom/Costumes/";
        std::string fileNames = "costume_customA\0\0\0costume_customB\0\0\0costume_customC\0\0\0costume_customD";
        std::array <std::string, 4> errMsg = { "", "", "", "" };

        u32 RODataOffset = 0x14;

        Process::WriteString(AddressList::TextToRodata.addr + RODataOffset, fileNames, StringFormat::Utf8);

        // restore Slot A pointer -> directs to costume_customA rather than costume_highfairy
        if (!restoreGreatFairy->IsActivated()) {
            Process::Write32((AddressList::UnusedCostumeDataPointerList.addr), (AddressList::TextToRodata.addr + 0x14));
            menuCostumeSlotA->CanBeSelected(true);
        }

        // edit pointer list
        if (File::Exists(customCostumeDirectory + "costume_customA.bch"))
           Process::Write32((AddressList::UnusedCostumeDataPointerList.addr), (AddressList::TextToRodata.addr + 0x14));
        else errMsg[0] = "Missing: costume_customA.bch";

        if (File::Exists(customCostumeDirectory + "costume_customB.bch"))
            Process::Write32((AddressList::UnusedCostumeDataPointerList.addr + 0x08), (AddressList::TextToRodata.addr + 0x26));
        else errMsg[1] = "Missing: costume_customB.bch";

        if (File::Exists(customCostumeDirectory + "costume_customC.bch")) 
            Process::Write32((AddressList::UnusedCostumeDataPointerList.addr + 0x10), (AddressList::TextToRodata.addr + 0x38));
        else errMsg[2] = "Missing: costume_customC.bch";

        if (File::Exists(customCostumeDirectory + "costume_customD.bch")) 
            Process::Write32((AddressList::UnusedCostumeDataPointerList.addr + 0x18), (AddressList::TextToRodata.addr + 0x49));
        else errMsg[3] = "Missing: costume_customD.bch";

        if (std::any_of(errMsg.begin(), errMsg.end(), [](const std::string& entry) { return !entry.empty(); })){
            OSD::Notify(Color::Red << "ERROR");
            OSD::Notify(Color::Red << "Custom Costume files cannot be found or do");
            OSD::Notify(Color::Red << "not exist in the Tricord directory.");
            OSD::Notify(Color::Gainsboro << "Custom Costume loading will now be CANCELLED.");


            for (int iterator = 0; iterator < 4; ++iterator) {
                if (!errMsg[iterator].empty()) {
                    OSD::Notify(Color::Yellow << errMsg[iterator]);
                }
            }
            entry->Disable();
            isCostumeFail = true;
        } 
    }
       
    void greatFairyEnable(MenuEntry* entry) {
        std::string greatFairyFileName = "costume_highfairy";
        std::string errMsg = "Tricord cannot place Great Fairy Costume into Custom Costume Slot A since it is currently configured to display a different costume.\n\nWould you like to overwrite the current costume in Custom Costume Slot A with Great Fairy?";
        
        u8 greatFairyID = 0x26;

        Process::WriteString(AddressList::TextToRodata.addr, greatFairyFileName, StringFormat::Utf8);

        manageCatalogSize->Enable();

        // if Slot 1 is already in-use -> this entry is auto-disabled
        if (CustomSlots[0] != 255) {
            if (MessageBox(Color::Gainsboro << "Error", errMsg, DialogType::DialogYesNo)()) {
                menuCostumeSlotA->CanBeSelected(false);
                CustomSlots[0] = 255;
            }
            else {
                entry->Disable();
                return;
            }
        }
        else {
            if (!isCatalogPointerNull()) {
                Process::Write32((AddressList::UnusedCostumeDataPointerList.addr), AddressList::TextToRodata.addr);
                Process::Write8((CostumeCatalogLocation + catalogStartOffset + costumeCatalogSize), greatFairyID);
            }
        }
    }    

    void changeLinkCostume(MenuEntry* entry) {
        u32 playerID = chooseLink();
        u32 memoryOffset = playerID * 0x10000;

        // note 1: currently only uses the OG costume list
        Keyboard costumeList("Choose a costume:\n\nBe sure to load into a new area for changes to fully\ntake effect.");
        costumeList.Populate(universalCostumeList);

        u8 result = costumeList.Open();
        Process::Write8((AddressList::CurrCostume.addr + memoryOffset), result);
    }

    void selCostumeEffect(MenuEntry* entry) {
        // currently unimplemented, ignore for now...
    }
}