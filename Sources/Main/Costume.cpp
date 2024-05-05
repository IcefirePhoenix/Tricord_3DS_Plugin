#include "Helpers.hpp"
#include "Main/Costume.hpp"

#include "CTRPluginFramework/Graphics/OSD.hpp"
#include "CTRPluginFramework/Menu/MessageBox.hpp"

#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"

#include <CTRPluginFramework.hpp>

#include <array>
#include <bitset>
#include "csvc.h"
#include "pIDindex.h"

namespace CTRPluginFramework
{
    MenuEntry* menuCostumeSlotA;
    MenuEntry* menuCostumeSlotB;
    MenuEntry* menuCostumeSlotC;
    MenuEntry* menuCostumeSlotD;
    MenuEntry* triggerCostumeSlots;

    MenuEntry* restoreGreatFairy;
    MenuEntry* manageCatalogSize;

    MenuEntry* initCustomCostumes;
    MenuEntry* writeCostumeIDToSlot;

    MenuEntry* writeCosmeticCostumeID;

    bool showSlots = false;
    bool isCostumeFail = false;

    u8 CustomSlots[4] = { 255, 255, 255, 255 };

    u32 catalogStartOffset = 0xE8;
    u32 CostumeCatalogLocation;
    u8 costumeCatalogSize, catalogIncSize;

    u8 cosmeticIDs[3] = {0xFF, 0xFF, 0xFF};

    /*
    std::bitset<32> indCostumeEffectsA[3];
    std::bitset<8> indCostumeEffectsB[3];
    u8 legendary[3];
    u8 bearDodge[3];
    */
    
    std::bitset<42> indCostumeEffects[3];
    u8 tingleBalloons[3];
    u8 dapperSpinA[3]; u8 dapperSpinB[3];

    void Costume::openCustomCostumeSlots(MenuEntry* entry) 
    {
        if (showSlots) 
        {
            disableCustomCostumeMenuOpt();
            resetCostumeArray();

            entry->SetName("Open Custom Costume Slots");
        }
        else 
        {
            int openedCustomSlots = chooseSlotAddNum();

            addCustomSlotsToMenu(openedCustomSlots);
            isCustomCostumeConflict(openedCustomSlots);      
            enableCustomCostumeOpt();

            entry->SetName("Close Custom Costume Slots");
        }
        showSlots = !showSlots;
    }

    void disableCustomCostumeMenuOpt(void) 
    {
        menuCostumeSlotA->Hide();
        menuCostumeSlotB->Hide();
        menuCostumeSlotC->Hide();
        menuCostumeSlotD->Hide();

        initCustomCostumes->Disable();
        writeCostumeIDToSlot->Disable();
    }

    void enableCustomCostumeOpt(void) 
    {
        manageCatalogSize->Enable();
        writeCostumeIDToSlot->Enable();

        if (!isCostumeFail)
            initCustomCostumes->Enable();
    }

    bool isCustomCostumeConflict(int openSlots) 
    {
        if (restoreGreatFairy->IsActivated() && openSlots >= 0) 
        {
            menuCostumeSlotA->CanBeSelected(false);

            std::string errMsg = ("Custom Costume Slot A is currently in-use by Restore Great Fairy Costume. Please disable Restore ");
            errMsg += ("Great Fairy Costume in order to use this costume slot.\n\nNote: The option to configure Custom Costume Slot A ");
            errMsg += ("has automatically been disabled in the menu.");

            MessageBox(Color::Gainsboro << "Error", errMsg)();
            return true;
        }
        return false;
    }

    void addCustomSlotsToMenu(int openSlots) 
    {
        menuCostumeSlotA->Show();
        menuCostumeSlotA->CanBeSelected(true);

        // redundant; using a switch statement jumbles the entire menu's entry order...
        if (openSlots == 1) 
            menuCostumeSlotB->Show();
        
        else if (openSlots == 2) 
        {
            menuCostumeSlotB->Show();
            menuCostumeSlotC->Show();
        }
        
        else if (openSlots == 3) 
        {
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

    void resetCostumeArray(void)
    {
        for (int i = 0; i < 4; i++) 
            CustomSlots[i] = 255;
    }

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

    void Costume::manageCatalogSizeAuto(MenuEntry* entry) 
    {
        u8 isObtained;
        u8 vanillaCostumeCount = 0x25;
        u32 catalogSizeOffset = 0xE4;

        // get catalog dynamic location from pointer
        Process::Read32(AddressList::CostumeCatalogPointer.addr, CostumeCatalogLocation);
        
        if (!GeneralHelpers::isNullPointer(CostumeCatalogLocation)) 
        {
            costumeCatalogSize = catalogIncSize = 0;
            for (u8 i = 0x0; i <= vanillaCostumeCount; i++) 
            {
                isObtained = 0;
                Process::Read8((AddressList::CostumeObtainStatus.addr + i), isObtained);

                if (isObtained)
                    costumeCatalogSize++;
            }

            // increase catalog size to accommodate custom costume slots B-D
            for (int i = 3; i > 0; --i) 
            {
                if (CustomSlots[i] != 255) 
                    catalogIncSize++;
            }

            // check if slot A is in-use
            if (CustomSlots[0] != 255 || restoreGreatFairy->IsActivated() == true)
                catalogIncSize++;
            
            Process::Write8((CostumeCatalogLocation + catalogSizeOffset), (costumeCatalogSize + catalogIncSize));
        }
    }

    void Costume::selectCostumeID(MenuEntry *entry) 
    {
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
            costumeList.Populate(GameData::universalCostumeList);
            result = costumeList.Open();
        }
        else 
        {
            costumeList.Populate(GameData::customCostumeList);
            result = costumeList.Open() + unusedRange; // get 0x26-0x29 range
        }

        if (entry->Name() == menuCostumeSlotA->Name()) 
        {
            menuCostumeSlotA->SetName("   Set slot A: " << GameData::getCostumeNameFromID(result));
            CustomSlots[0] = result;  
        }
        else if (entry->Name() == menuCostumeSlotB->Name()) 
        {
            menuCostumeSlotB->SetName("   Set slot B: " << GameData::getCostumeNameFromID(result));
            CustomSlots[1] = result;
        }
        else if (entry->Name() == menuCostumeSlotC->Name()) 
        {
            menuCostumeSlotC->SetName("   Set slot C: " << GameData::getCostumeNameFromID(result));
            CustomSlots[2] = result;
        }
        else if (entry->Name() == menuCostumeSlotD->Name()) 
        {
            menuCostumeSlotD->SetName("   Set slot D: " << GameData::getCostumeNameFromID(result));
            CustomSlots[3] = result;
        }
    }

    void Costume::writeCostumeSlot(MenuEntry* entry) 
    {
        if (!GeneralHelpers::isNullPointer(CostumeCatalogLocation)) 
        {
            u32 DLCAddressStart = CostumeCatalogLocation + catalogStartOffset + costumeCatalogSize;
            for (u32 iterator = 0x0; iterator < 0x4; ++iterator) 
            {
                if (CustomSlots[iterator] != 255)
                    Process::Write8(DLCAddressStart + iterator, CustomSlots[iterator]);
            }
        }
    }

    // TODO: costume names + descriptions? locate msbt pointers
    // Example:
    // Custom Costume #1
    // "A custom costume imported in-game via the Tricord plugin!
    // This otherworldly garb comes with no powers, but it looks quite dapper!"
    void Costume::initCustomCostumesAuto(MenuEntry* entry) 
    {
        std::string customCostumeDirectory = "/Tricord/Custom/Costumes/";
        std::string fileNames = "costume_customA\0\0\0costume_customB\0\0\0costume_customC\0\0\0costume_customD";
        std::array <std::string, 4> errMsg = { "", "", "", "" };

        u32 RODataOffset = 0x14;

        Process::WriteString(AddressList::TextToRodata.addr + RODataOffset, fileNames, StringFormat::Utf8);

        // restore Slot A pointer -> directs to costume_customA rather than costume_highfairy
        if (!restoreGreatFairy->IsActivated()) 
        {
            Process::Write32((AddressList::UnusedCostumeDataPointers.addr), (AddressList::TextToRodata.addr + 0x14));
            menuCostumeSlotA->CanBeSelected(true);
        }

        // edit pointer list
        if (File::Exists(customCostumeDirectory + "costume_customA.bch"))
           Process::Write32((AddressList::UnusedCostumeDataPointers.addr), (AddressList::TextToRodata.addr + 0x14));
        else errMsg[0] = "Missing: costume_customA.bch";

        if (File::Exists(customCostumeDirectory + "costume_customB.bch"))
            Process::Write32((AddressList::UnusedCostumeDataPointers.addr + 0x08), (AddressList::TextToRodata.addr + 0x26));
        else errMsg[1] = "Missing: costume_customB.bch";

        if (File::Exists(customCostumeDirectory + "costume_customC.bch")) 
            Process::Write32((AddressList::UnusedCostumeDataPointers.addr + 0x10), (AddressList::TextToRodata.addr + 0x38));
        else errMsg[2] = "Missing: costume_customC.bch";

        if (File::Exists(customCostumeDirectory + "costume_customD.bch")) 
            Process::Write32((AddressList::UnusedCostumeDataPointers.addr + 0x18), (AddressList::TextToRodata.addr + 0x49));
        else errMsg[3] = "Missing: costume_customD.bch";

        bool showErrMsg = std::any_of(errMsg.begin(), errMsg.end(), [](const std::string& index) {
            return !index.empty();
        });

        if (showErrMsg)
        {
            OSD::Notify(Color::Red << "ERROR");
            OSD::Notify(Color::Red << "Custom Costume files cannot be found or do");
            OSD::Notify(Color::Red << "not exist in the Tricord directory.");
            OSD::Notify(Color::Gainsboro << "Custom Costume loading will now be CANCELLED.");


            for (int iterator = 0; iterator < 4; ++iterator) 
            {
                if (!errMsg[iterator].empty()) 
                    OSD::Notify(Color::Yellow << errMsg[iterator]);
            }
            entry->Disable();
            isCostumeFail = true;
        } 
    }
       
    void Costume::greatFairyEnable(MenuEntry* entry) 
    {
        std::string greatFairyFileName = "costume_highfairy";
        std::string errMsg = "Tricord cannot place Great Fairy Costume into Custom Costume Slot A since it is currently configured to display a different costume.\n\nWould you like to overwrite the current costume in Custom Costume Slot A with Great Fairy?";
        
        u8 greatFairyID = 0x26; // slot A ID

        Process::WriteString(AddressList::TextToRodata.addr, greatFairyFileName, StringFormat::Utf8);

        manageCatalogSize->Enable();

        // if Slot 1 is already in-use -> this entry is auto-disabled
        if (CustomSlots[0] != 255) 
        {
            if (MessageBox(Color::Gainsboro << "Error", errMsg, DialogType::DialogYesNo)()) 
            {
                menuCostumeSlotA->CanBeSelected(false);
                CustomSlots[0] = 255;
            }
            else 
            {
                entry->Disable();
                return;
            }
        }
        else 
        {
            if (!GeneralHelpers::isNullPointer(CostumeCatalogLocation)) 
            {
                Process::Write32((AddressList::UnusedCostumeDataPointers.addr), AddressList::TextToRodata.addr);
                Process::Write8((CostumeCatalogLocation + catalogStartOffset + costumeCatalogSize), greatFairyID);
            }
        }
    }    

    void Costume::changeLinkCostume(MenuEntry* entry) 
    {
        int linkChoice = GeneralHelpers::chooseLink();

        if (linkChoice >= 0) {
            u32 playerID = static_cast<u32>(linkChoice);   
            u32 memoryOffset = playerID * 0x10000;

            // note 1: currently only uses the OG costume list
            Keyboard costumeList("Choose a costume:\n\nBe sure to load into a new area for changes to fully\ntake effect.");
            costumeList.Populate(GameData::universalCostumeList);

            u8 result = costumeList.Open();
            Process::Write8((AddressList::CurrCostume.addr + memoryOffset), result);
        }
    }

    void Costume::forceBearMinMax(MenuEntry* entry)
    {
        Keyboard Bear("Force Bear Minimum or Bear Maximum, or\nReset to restore the Hero Point check.");
        static const StringVector bearOptions = 
        {
            "Reset",
            "Bear Minimum",
            "Bear Maximum"
        };
        Bear.Populate(bearOptions, true);
        switch (Bear.Open()){
            case 0:
                Process::Patch(AddressList::BearMinMaxCheckA.addr, 0xA3A00001);
                Process::Patch(AddressList::BearMinMaxCheckB.addr, 0xB3A00000);
                break;
            case 1:
                Process::Patch(AddressList::BearMinMaxCheckA.addr, 0xA3A00000);
                Process::Patch(AddressList::BearMinMaxCheckB.addr, 0xB3A00000);
                break;
            case 2:
                Process::Patch(AddressList::BearMinMaxCheckA.addr, 0xA3A00001);
                Process::Patch(AddressList::BearMinMaxCheckB.addr, 0xB3A00001);
                break;
        }
    }

    void Costume::preventLobbyReset(MenuEntry* entry)
    {
        if (entry->Name() == "Preserve Doppel costume edits in single player lobby")
        {
            Process::Patch(AddressList::DoppelLobbyReset.addr, 0xEA000007);
            entry->SetName("Reset Doppel costume edits in single player lobby");
        }
        else
        {
            Process::Patch(AddressList::DoppelLobbyReset.addr, 0x1A000007);
            entry->SetName("Preserve Doppel costume edits in single player lobby");
        }
    }

    void Costume::enableCosmeticCostume(MenuEntry* entry)
    {
        if (entry->Name() == "Enable Cosmetic Costumes") {
            // Edit all LDRB instructions in graphics functions to direct to the alt costume address
            Process::Patch(AddressList::CostumeIDOffsetAuraA.addr, 0xE5D00D61);
            Process::Patch(AddressList::CostumeIDOffsetAuraB.addr, 0xE5D01D61);
            Process::Patch(AddressList::CostumeIDOffsetAuraC.addr, 0xE5D01D61);
            Process::Patch(AddressList::CostumeIDOffsetAuraD.addr, 0xE5D01D61);
            Process::Patch(AddressList::CostumeIDOffsetCheetah.addr, 0xE5D01D61);
            Process::Patch(AddressList::CostumeIDOffsetDune.addr, 0xE5D00D61);
            Process::Patch(AddressList::CostumeIDOffsetCheer.addr, 0xE5D00D61);
            Process::Patch(AddressList::CostumeIDOffsetSwordPtcl.addr, 0xE5D00D61);

            // Add custom function to text->rodata padding and redirect model loader bl to it
            u32 custFuncStart = AddressList::TextToRodata.addr + 0x64;
            Process::Patch(custFuncStart, 0xE5900008);
            Process::Patch(custFuncStart + 0x4, 0xE5D00D61);
            Process::Patch(custFuncStart + 0x8, 0xE12FFF1E);
            u32 blOffset = (custFuncStart - 0x8 - AddressList::CostumeIDFunBLModel.addr) / 4;
            Process::Patch(AddressList::CostumeIDFunBLModel.addr, 0xEB000000 + blOffset);

            writeCosmeticCostumeID->Enable();
            entry->SetName("Disable Cosmetic Costumes");
        }
        else {
            // Edit all LDRB instructions back to normal
            Process::Patch(AddressList::CostumeIDOffsetAuraA.addr, 0xE5D00064);
            Process::Patch(AddressList::CostumeIDOffsetAuraB.addr, 0xE5D01064);
            Process::Patch(AddressList::CostumeIDOffsetAuraC.addr, 0xE5D01064);
            Process::Patch(AddressList::CostumeIDOffsetAuraD.addr, 0xE5D01064);
            Process::Patch(AddressList::CostumeIDOffsetCheetah.addr, 0xE5D01064);
            Process::Patch(AddressList::CostumeIDOffsetDune.addr, 0xE5D00064);
            Process::Patch(AddressList::CostumeIDOffsetCheer.addr, 0xE5D00064);
            Process::Patch(AddressList::CostumeIDOffsetSwordPtcl.addr, 0xE5D00064);

            // Reset bl in model loader
            // Need to know: Does this hold the same offset value in all regions, or not?
            Process::Patch(AddressList::CostumeIDFunBLModel.addr, 0xEB0FDCF8);

            writeCosmeticCostumeID->Disable();
            entry->SetName("Enable Cosmetic Costumes");
        }
    }

    void Costume::setCosmeticCostume(MenuEntry* entry)
    {
        int linkChoice = GeneralHelpers::chooseLink();

        if (linkChoice >= 0) {
            u32 playerID = static_cast<u32>(linkChoice);   
            u32 memoryOffset = playerID * GameData::playerAddressOffset;
            u8 currEffectCostumeID; u8 currCosmeticCostumeID;
            Process::Read8(AddressList::CurrCostume.addr + memoryOffset, currEffectCostumeID);
            if (cosmeticIDs[linkChoice] != 0xFF) {
                currCosmeticCostumeID = cosmeticIDs[linkChoice];
            }
            else {
                currCosmeticCostumeID = currEffectCostumeID;
            }
            std::string currEffectCostume = GameData::getCostumeNameFromID(currEffectCostumeID);
            std::string currCosmeticCostume = GameData::getCostumeNameFromID(currCosmeticCostumeID);
            std::string selectedPlayer = "";
            switch (linkChoice) {
                case 0:
                    selectedPlayer = "Player 1 (Green)";
                    break;
                case 1:
                    selectedPlayer = "Player 2 (Blue)";
                    break;
                case 2:
                    selectedPlayer = "Player 3 (Red)";
                    break;
            }

            std::string topscreenMessage = 
            "Set new cosmetic costume?\nOr reset to the effective costume?\n\nSelected: " + selectedPlayer
            + "\n\nCurrent effective costume: " + currEffectCostume
            + "\nCurrent cosmetic costume: " + currCosmeticCostume;

            Keyboard setReset(topscreenMessage);
            StringVector setResetOptions = 
            {
                "Set new",
                "Reset"
            };
            setReset.Populate(setResetOptions);
            int setResetResult = setReset.Open();

            if (setResetResult == 0) {
                Keyboard costumeList("Choose a costume:\n\nBe sure to load into a new area for changes to fully\ntake effect.");
                costumeList.Populate(GameData::universalCostumeList);

                u8 result = costumeList.Open();
                if (result >= 0){
                    cosmeticIDs[linkChoice] = result;
                }
            }
            else if (setResetResult == 1) {
                cosmeticIDs[linkChoice] = 0xFF;
                Process::Write8(AddressList::CurrCostumeAlt.addr + memoryOffset, currEffectCostumeID);
            }
        }
    }

    void Costume::writeCosmeticCostume(MenuEntry* entry)
    {
        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++) {
            if (cosmeticIDs[iterateThruPlayers] != 0xFF) {
                Process::Write8(AddressList::CurrCostumeAlt.addr + iterateThruPlayers*GameData::playerAddressOffset, cosmeticIDs[iterateThruPlayers]);
            }
        }
    }

    /* --------------------------------- */

    void Costume::setIndCostumeEffect(MenuEntry* entry)
    {
        pIDindex args = *reinterpret_cast<pIDindex*>(entry->GetArg());
        int player = args.playerID;
        int i = args.index;
        std::string longName = entry->Name();
        std::string prefix = longName.substr(0, 4);
        std::string baseName = longName.substr(4, std::string::npos);
        if (prefix == "( ) ") {
            indCostumeEffects[player].set(i);
            entry->SetName("(X) " + baseName);
        }
        else {
            indCostumeEffects[player].reset(i);
            entry->SetName("( ) " + baseName);
        }
    }

    void Costume::luckyDodge(MenuEntry* entry)
    {
        int player = reinterpret_cast<int>(entry->GetArg());
        if (entry->Name() == "( ) Lucky Dodge") {
            // Select between 3 dodge chances
            Keyboard dodge("Select the dodge rate:");
            StringVector dodgeChances = 
            {
                "25%",
                "50%",
                "75%"
            };
            dodge.Populate(dodgeChances);
            switch (dodge.Open()) {
                case 0:
                    indCostumeEffects[player].set(6);
                    entry->SetName("(X) Lucky Dodge - 25%");
                    break;
                case 1:
                    indCostumeEffects[player].set(7);
                    entry->SetName("(X) Lucky Dodge - 50%");
                    break;
                case 2:
                    indCostumeEffects[player].set(41);
                    entry->SetName("(X) Lucky Dodge - 75%");
                    break;
            }
        }
        else {
            indCostumeEffects[player].reset(6);
            indCostumeEffects[player].reset(7);
            indCostumeEffects[player].reset(41);
            entry->SetName("( ) Lucky Dodge");
        }
    }

    void Costume::writeIndCostumeEffects(MenuEntry* entry)
    {
        int player = reinterpret_cast<int>(entry->GetArg());
        u32 attributesA;
        u8 attributesB;
        u32 addrA = AddressList::CostumeAttrA.addr + player*GameData::playerAddressOffset;
        u32 addrB = AddressList::CostumeAttrD.addr + player*GameData::playerAddressOffset;
        Process::Read32(addrA, attributesA);
        Process::Read8(addrB, attributesB);

        std::bitset<42> fullset = indCostumeEffects[player];
        std::bitset<42> subset = fullset >> 32;
        u32 attrToWriteA = attributesA | fullset.to_ulong();
        u8 attrToWriteB = attributesB | subset.to_ulong();

        Process::Write32(addrA, attrToWriteA);
        Process::Write32(addrB, attrToWriteB);

        // If statements for the attributes with distant addresses
        if (fullset.test(40)) {
            Process::Write8(AddressList::HeartDropRate.addr, 0x1);
        }
        if (fullset.test(41)) {
            Process::Write8(AddressList::BearDodge.addr, 0x1);
        }
    }

    void Costume::tingle(MenuEntry* entry)
    {
        // TODO
    }

    void Costume::dapperInstant(MenuEntry* entry)
    {
        // TODO
    }

    /*
    void Costume::zora(MenuEntry* entry)
    {
        int player = reinterpret_cast<int>(entry->GetArg());
        if (entry->Name() == "( ) Zora Costume - Enhanced swimming") {
            indCostumeEffectsA[player].set(1);
            entry->SetName("(X) Zora Costume - Enhanced swimming");
        }
        else {
            indCostumeEffectsA[player].reset(1);
            entry->SetName("( ) Zora Costume - Enhanced swimming");
        }
    }

    void Costume::goron(MenuEntry* entry)
    {
        int player = reinterpret_cast<int>(entry->GetArg());
        if (entry->Name() == "( ) Goron Garb - Burn immunity and lava swimming") {
            indCostumeEffectsA[player].set(2);
            entry->SetName("(X) Goron Garb - Burn immunity and lava swimming");
        }
        else {
            indCostumeEffectsA[player].reset(2);
            entry->SetName("( ) Goron Garb - Burn immunity and lava swimming");
        }
    }

    void Costume::parka(MenuEntry* entry)
    {
        int player = reinterpret_cast<int>(entry->GetArg());
        if (entry->Name() == "( ) Cozy Parka - Freeze and ice slip immunity") {
            indCostumeEffectsA[player].set(3);
            entry->SetName("(X) Cozy Parka - Freeze and ice slip immunity");
        }
        else {
            indCostumeEffectsA[player].reset(3);
            entry->SetName("( ) Cozy Parka - Freeze and ice slip immunity");
        }
    }

    void Costume::ninja(MenuEntry* entry)
    {
        int player = reinterpret_cast<int>(entry->GetArg());
        if (entry->Name() == "( ) Ninja Gi - Instant triple damage dash") {
            indCostumeEffectsA[player].set(4);
            entry->SetName("(X) Ninja Gi - Instant triple damage dash");
        }
        else {
            indCostumeEffectsA[player].reset(4);
            entry->SetName("( ) Ninja Gi - Instant triple damage dash");
        }
    }

    void Costume::spinAttack(MenuEntry* entry)
    {
        int player = reinterpret_cast<int>(entry->GetArg());
        if (entry->Name() == "( ) Spin Attack Attire - Great Spin Attack") {
            indCostumeEffectsA[player].set(5);
            entry->SetName("(X) Spin Attack Attire - Great Spin Attack");
        }
        else {
            indCostumeEffectsA[player].reset(5);
            entry->SetName("( ) Spin Attack Attire - Great Spin Attack");
        }
    }

    void Costume::rupee(MenuEntry* entry)
    {
        int player = reinterpret_cast<int>(entry->GetArg());
        if (entry->Name() == "( ) Rupee Regalia - Increased rupee drop rate") {
            indCostumeEffectsA[player].set(11);
            entry->SetName("(X) Rupee Regalia - Increased rupee drop rate");
        }
        else {
            indCostumeEffectsA[player].reset(11);
            entry->SetName("( ) Rupee Regalia - Increased rupee drop rate");
        }
    }

    void Costume::doubleDmg(MenuEntry* entry)
    {
        int player = reinterpret_cast<int>(entry->GetArg());
        if (entry->Name() == "( ) Bear / Cursed - Double damage taken") {
            indCostumeEffectsA[player].set(19);
            entry->SetName("(X) Bear / Cursed - Double damage taken");
        }
        else {
            indCostumeEffectsA[player].reset(19);
            entry->SetName("( ) Bear / Cursed - Double damage taken");
        }
    }

    void Costume::template(MenuEntry* entry)
    {
        int player = reinterpret_cast<int>(entry->GetArg());
        if (entry->Name() == "( ) ") {
            indCostumeEffectsA[player].set(#);
            entry->SetName("(X) ");
        }
        else {
            indCostumeEffectsA[player].reset(#);
            entry->SetName("( ) ");
        }
    }
    */

}