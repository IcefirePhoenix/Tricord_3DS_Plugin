#include "cheats.hpp"
#include "csvc.h"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/PlayerHelper.hpp"

#include "CTRPluginFrameworkImpl/Menu.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/System/FSPath.hpp"

#include "CTRPluginFramework/Menu/MessageBox.hpp"
#include "CTRPluginFramework/Graphics/OSD.hpp"

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

            // this is redundant and long and I really dislike it...
            // thing is, I tried a switch statement and it jumbled the order of every entry so yeah :(
            menuCostumeSlotA->Show(); // no need for if-statement here

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
                menuCostumeSlotA->Hide();

                std::string errMsg = ("Custom Costume Slot A is currently in-use by Restore Great Fairy. Please disable Restore ");
                errMsg += ("Great Fairy in order to use this costume slot.\n\nNote: The option to configure Custom Costume Slot A ");
                errMsg += ("has automatically been hidden in the menu.");

                MessageBox(Color::Gainsboro << "Error", errMsg)();
                return;
            }
        }
    }

    u32 CostumeCatalogLocation;
    u8 costumeCatalogSize;
    u8 catalogIncSize;


    void manageCatalogSizeAuto(MenuEntry* entry) {
        u8 isObtained;

        // read pointer value -> get catalog dynamic location
        Process::Read32(AddressList::CostumeCatalogPointer.addr, CostumeCatalogLocation);

        // check for null pointer... this should ONLY be running when a catalog is open
        if (CostumeCatalogLocation != 0x00000000) {

            costumeCatalogSize = 0;
            catalogIncSize = 0;

            // count obtained costumes
            for (u32 i = 0x0; i < 0x26; i++) {
                isObtained = 0;

                Process::Read8((AddressList::CostumeObtainStatus.addr + i), isObtained);

                if (isObtained) {
                    costumeCatalogSize++;
                }
            }

            // check if slots A-C have a costume chosen
            for (int i = 3; i >= 1; i--) {
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
            // if OG, then populate keyboard with this:
            costumeList.Populate(universalCostumeList);
            result = costumeList.Open();
        }
        else {
            // if custom, then populate keyboard with this:
            costumeList.Populate(customCostumeList);
            result = costumeList.Open() + 0x26; // adding 0x25 to get to 0x26-0x29 range
        }

        if (entry->Name() == "Set custom costume slot 1") {
            CustomSlots[0] = result;
        }
        else if (entry->Name() == "Set custom costume slot 2") {
            CustomSlots[1] = result;
        }
        else if (entry->Name() == "Set custom costume slot 3") {
            CustomSlots[2] = result;
        }
        else if (entry->Name() == "Set custom costume slot 4") {
            CustomSlots[3] = result;
        }
    }

    void writeCostumeSlot(MenuEntry* entry) {
        // check for null pointer
        // aka: don't write if catalog is NOT currently open
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
        }
    }

    void initCustomCostumesAuto(MenuEntry* entry) {
        // when function is fixed + working, this should be enabled at all times
        // meaning the entry should be set to Enable() straight from InitMenu()
        std::string customCostumeDirectory = ("/Tricord/Custom/Costumes");

        // init string labels
        // should this just be one large block of code / one instruction
        // is that possible lol

        // should also write descriptions and names too for the catalog
        // but first figure out where those + pointers are located in memory...
        // are we supporting english only or every language?
        
        // custom name: Custom Costume #1
        // custom description: "A custom costume imported in-game via the Tricord plugin!
        // This otherworldly garb comes with no powers, but it looks quite dapper!"

        Process::WriteString(AddressList::TextToRodata.addr + 0x14, "costume_customA", StringFormat::Utf8);
        Process::WriteString(AddressList::TextToRodata.addr + 0x26, "costume_customB", StringFormat::Utf8);
        Process::WriteString(AddressList::TextToRodata.addr + 0x38, "costume_customC", StringFormat::Utf8);
        Process::WriteString(AddressList::TextToRodata.addr + 0x49, "costume_customD", StringFormat::Utf8);
        
        //Process::Write32((AddressList::UnusedCostumeDataPointerList.addr), (AddressList::TextToRodata.addr + 0x14));
        //Process::Write32((AddressList::UnusedCostumeDataPointerList.addr + 0x08), (AddressList::TextToRodata.addr + 0x26));

        /*
        // check if file exist
        // then edit pointer list
        if (File::Exists(customCostumeDirectory + "/costume_customA.bch")){
            Process::Write32((AddressList::UnusedCostumeDataPointerList.addr), (AddressList::TextToRodata.addr + 0x14));
        } 
        else if (File::Exists(customCostumeDirectory + "/costume_customB.bch")) {
            Process::Write32((AddressList::UnusedCostumeDataPointerList.addr + 0x08), (AddressList::TextToRodata.addr + 0x26));
        }
        //else if (File::Exists(customCostumeDirectory + "/costume_customC.bch")) {
        //    Process::Write32((AddressList::UnusedCostumeDataPointerList.addr + 0x10), (AddressList::TextToRodata.addr + 0x38));
       // }
        else {
            MessageBox(Color::Gainsboro << "Error", "Custom Costume files cannot be found or do ot exist in the Tricord directory.")();
        }*/
    }
       
    void greatFairyEnable(MenuEntry* entry) {

        manageCatalogSize->Enable();

        Process::WriteString(AddressList::TextToRodata.addr, "costume_highfairy", StringFormat::Utf8);
        
        // if Slot 1 is already in use and Fairy is enabled
        if (CustomSlots[0] != 255) {
            if (MessageBox(Color::Gainsboro << "Error", "Restore Great Fairy Costume cannot be used while Custom Costume Slot A is in-use.\n\nWould you like to disable Custom Costume Slot A?", DialogType::DialogYesNo)()){
                CustomSlots[0] = 255;
                menuCostumeSlotA->Hide();
            }
            else {
                entry->Disable();
            }
            return;
        }

        else if (CustomSlots[0] == 255) {

            // check for null pointer
            if (CostumeCatalogLocation != 0x00000000) {
                Process::Write32((AddressList::UnusedCostumeDataPointerList.addr), AddressList::TextToRodata.addr);
                Process::Write8((CostumeCatalogLocation + 0xE8 + costumeCatalogSize), 0x29); // write to catalog    
            }

            if (entry->WasJustActivated()) {
                // use a callback
                MessageBox(Color::Gainsboro << "Note", "Custom Costume Slot A cannot be used while Restore Great Fairy is enabled. To use it, please disable Restore Great Fairy first.\n\nThe option to configure Custom Costume Slot A will automatically be hidden in the menu.")();
            }
        }
    }    

    void changeLinkCostume(MenuEntry* entry) {
        int playerID = chooseLink();

        // note 1: currently only uses the OG costume list
        
        // note 2: didn't use selectCostumeID() here in order to add note
        // eventually when this is changed to selectCostumeID(), the note should be added as 
        // a menu callback message box
        Keyboard costumeList("Choose a costume:\n\nBe sure to load into a new area for changes to fully take effect.");
        costumeList.Populate(universalCostumeList);

        u8 result = costumeList.Open();

        switch (playerID) {
        case 0:
            Process::Write8(AddressList::CurrCostume.addr, result);
            break;
        case 1:
            Process::Write8((AddressList::CurrCostume.addr + 0x10000), result);
            break;
        case 2:
            Process::Write8((AddressList::CurrCostume.addr + 0x20000), result);
            break;
        default:
            OSD::Notify(("ERROR: Cannot write Costume data for Player  " + std::to_string(playerID)), Color::Red);
            break;
        }
    }
}