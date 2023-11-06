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
#include "3ds.h"

namespace CTRPluginFramework
{
    // additional costume slots are hidden by default
    extern bool showSlots = false;
    u8 CustomSlots[4] = { 255, 255, 255, 255 };

    void openCustomCostumeSlots(MenuEntry* entry) {

        if (showSlots != false) {

            MessageBox(Color::Gainsboro << "Success", "Additional Costume Slots disabled in the menu!")();
            showSlots = false;

            menuCostumeSlotA->Hide();
            menuCostumeSlotB->Hide();
            menuCostumeSlotC->Hide();
            menuCostumeSlotD->Hide();

            initCustomCostumes->Disable();
            writeCostumeIDToSlot->Disable();

            // reset costume array
            for (int i = 0; i < 4; i++) {
                CustomSlots[i] = 255;
            }
        }
        else {
            initCustomCostumes->Enable();
            manageCatalogSize->Enable();
            writeCostumeIDToSlot->Enable();

            showSlots = true;

            const StringVector slotCount =
            {
              "1 slot",
              "2 slots",
              "3 slots",
              "4 slots"
            };

            Keyboard addSlots("Choose a costume:");
            addSlots.Populate(slotCount);

            int choice = addSlots.Open();

            // no need for if-statement here
            menuCostumeSlotA->Show(); 
            menuCostumeSlotA->CanBeSelected(true);

            // really redundant; using a switch statement jumbles the entry order...
            if (choice == 1) {
                menuCostumeSlotB->Show();
            }
            else if (choice == 2) {
                menuCostumeSlotB->Show();
                menuCostumeSlotC->Show();
            }
            else if (choice == 3) {
                menuCostumeSlotB->Show();
                menuCostumeSlotC->Show();
                menuCostumeSlotD->Show();
            }

            if (restoreGreatFairy->IsActivated() && choice >= 0) {
                menuCostumeSlotA->CanBeSelected(false);

                std::string errMsg = ("Custom Costume Slot A is currently in-use by Restore Great Fairy Costume. Please disable Restore ");
                errMsg += ("Great Fairy Costume in order to use this costume slot.\n\nNote: The option to configure Custom Costume Slot A ");
                errMsg += ("has automatically been disabled in the menu.");

                MessageBox(Color::Gainsboro << "Error", errMsg)();
            }
        }
    }

    u32 CostumeCatalogLocation;
    u8 costumeCatalogSize, catalogIncSize;

    void manageCatalogSizeAuto(MenuEntry* entry) {
        u8 isObtained;

        // read pointer value -> get catalog dynamic location
        Process::Read32(AddressList::CostumeCatalogPointer.addr, CostumeCatalogLocation);

        // check for null pointer... this should ONLY be running when a catalog is open
        if (CostumeCatalogLocation != 0x00000000) {

            costumeCatalogSize = catalogIncSize = 0;

            // count obtained costumes
            for (u32 i = 0x0; i < 0x26; i++) {
                isObtained = 0;

                Process::Read8((AddressList::CostumeObtainStatus.addr + i), isObtained);

                if (isObtained) {
                    costumeCatalogSize++;
                }
            }

            // check if slots A-C have a costume chosen
            for (int i = 3; i > 0; i--) {
                if (CustomSlots[i] != 255) {
                    catalogIncSize++;
                }
            }

            // check if slot A is in-use
            if (CustomSlots[0] != 255 || restoreGreatFairy->IsActivated() == true) {
                catalogIncSize++;
            }

            Process::Write8((CostumeCatalogLocation + 0xE4), (costumeCatalogSize + catalogIncSize));
        }
    }

    void selectCostumeID(MenuEntry *entry) {
        u8 result;
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
            // populate with vanilla costumes
            costumeList.Populate(universalCostumeList);
            result = costumeList.Open();
        }
        else {
            // populate with custom costumes
            costumeList.Populate(customCostumeList);
            result = costumeList.Open() + 0x26; // adding 0x26 to get to 0x26-0x29 range
        }

        if (entry->Name() == "Set custom costume slot A") {
            CustomSlots[0] = result;
        }
        else if (entry->Name() == "Set custom costume slot B") {
            CustomSlots[1] = result;
        }
        else if (entry->Name() == "Set custom costume slot C") {
            CustomSlots[2] = result;
        }
        else if (entry->Name() == "Set custom costume slot D") {
            CustomSlots[3] = result;
        }
    }

    void writeCostumeSlot(MenuEntry* entry) {
        // check for null pointer
        if (CostumeCatalogLocation != 0x00000000) {
            u32 DLCAddressStart = (CostumeCatalogLocation + 0xE8 + costumeCatalogSize);
            // write costume ID to desired slot ONLY if costume was previously chosen
            // note: changes aren't lost upon catalog exit/destroy = chosen values are maintained in array
            if (CustomSlots[0] != 255) {
                Process::Write8(DLCAddressStart, CustomSlots[0]);
            }

            if (CustomSlots[1] != 255) {
                Process::Write8((DLCAddressStart + 0x1), CustomSlots[1]);
            }

            if (CustomSlots[2] != 255) {
                Process::Write8((DLCAddressStart + 0x2), CustomSlots[2]);
            }

            if (CustomSlots[3] != 255) {
                Process::Write8((DLCAddressStart + 0x3), CustomSlots[3]);
            }

            if (!restoreGreatFairy->IsActivated()) {
                menuCostumeSlotA->CanBeSelected(true);
            }
        }
    }

    void initCustomCostumesAuto(MenuEntry* entry) {

        std::string customCostumeDirectory = ("/Tricord/Custom/Costumes");
       
        // init string labels... can/should this be done in one instruction...?
   
        Process::WriteString(AddressList::TextToRodata.addr + 0x14, "costume_customA", StringFormat::Utf8);
        Process::WriteString(AddressList::TextToRodata.addr + 0x26, "costume_customB", StringFormat::Utf8);
        Process::WriteString(AddressList::TextToRodata.addr + 0x38, "costume_customC", StringFormat::Utf8);
        Process::WriteString(AddressList::TextToRodata.addr + 0x49, "costume_customD", StringFormat::Utf8);

        // also costume names + descriptions? locate msbt pointers
        // Example:
        // Custom Costume #1
        // "A custom costume imported in-game via the Tricord plugin!
        // This otherworldly garb comes with no powers, but it looks quite dapper!"
        
        // this is to restore the pointer list -> directs to costume_customA rather than costume_highfairy
        if (!restoreGreatFairy->IsActivated()) {
            // this would be the correct one to use, but for now we'll just use the default Hero's Tunic
            // Process::Write32((AddressList::UnusedCostumeDataPointerList.addr), (AddressList::TextToRodata.addr + 0x14));
            Process::Write32((AddressList::UnusedCostumeDataPointerList.addr), 0x0060B658);
        }

        // currently a mess; ignore for now... possibly move this to a reset function...
        // check if file exist, then edit pointer list

        /*
        if (File::Exists(customCostumeDirectory + "/costume_customA.bch")){
            Process::Write32((AddressList::UnusedCostumeDataPointerList.addr), (AddressList::TextToRodata.addr + 0x14));
        } 
        else if (File::Exists(customCostumeDirectory + "/costume_customB.bch")) {
            Process::Write32((AddressList::UnusedCostumeDataPointerList.addr + 0x08), (AddressList::TextToRodata.addr + 0x26));
        }
        //else if (File::Exists(customCostumeDirectory + "/costume_customC.bch")) {
            Process::Write32((AddressList::UnusedCostumeDataPointerList.addr + 0x10), (AddressList::TextToRodata.addr + 0x38));
        }
        else {
            MessageBox(Color::Gainsboro << "Error", "Custom Costume files cannot be found or do ot exist in the Tricord directory.")();
        }
        */
    }
       
    void greatFairyEnable(MenuEntry* entry) {

        manageCatalogSize->Enable();

        Process::WriteString(AddressList::TextToRodata.addr, "costume_highfairy", StringFormat::Utf8);
        
        // if Slot 1 is already in-use -> this entry is enabled
        if (CustomSlots[0] != 255) {
            if (MessageBox(Color::Gainsboro << "Error", "Tricord cannot place Great Fairy Costume into Custom Costume Slot A since it is currently configured to display a different costume.\n\nWould you like to overwrite the current costume in Custom Costume Slot A with Great Fairy?", DialogType::DialogYesNo)() == true) {
                    menuCostumeSlotA->CanBeSelected(false);
                    CustomSlots[0] = 255;
            }
            else {
                entry->Disable();
                return;
            }
        }
        
        if (CustomSlots[0] == 255) {
            // check for null pointer
            if (CostumeCatalogLocation != 0x00000000) {
                Process::Write32((AddressList::UnusedCostumeDataPointerList.addr), AddressList::TextToRodata.addr);
                Process::Write8((CostumeCatalogLocation + 0xE8 + costumeCatalogSize), 0x26); // write to catalog    
            }
        }
    }    

    void changeLinkCostume(MenuEntry* entry) {
        u32 playerID = chooseLink();
        playerID = playerID * 0x10000;

        // note 1: currently only uses the OG costume list
        // note 2: didn't use selectCostumeID() here in order to add menu note 

        Keyboard costumeList("Choose a costume:\n\nBe sure to load into a new area for changes to fully take effect.");
        costumeList.Populate(universalCostumeList);

        u8 result = costumeList.Open();
        Process::Write8((AddressList::CurrCostume.addr + playerID), result);
    }

    void selCostumeEffect(MenuEntry* entry) {
        // currently unimplemented, ignore for now...
    }
}