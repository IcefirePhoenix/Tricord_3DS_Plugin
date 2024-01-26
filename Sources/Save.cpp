#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/GameData.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/Material.hpp"
#include "Helpers/LevelStatusEditor.hpp"
#include "Helpers/GeneralHelpers.hpp"

namespace CTRPluginFramework
{
    void useVoiceless(MenuEntry* entry) {
        Process::Write8(AddressList::MainVoice.addr, 0x4);
    }

    void mainVoice(MenuEntry* entry) {
        Keyboard voiceMenu("Choose a voice:\n\nThese changes can be observable online. Keep in\nmind your selection will be overwritten by Voiceless,\nif used.");
        voiceMenu.Populate(GameData::voiceList);

        u8 result = voiceMenu.Open();
        Process::Write8(AddressList::MainVoice.addr, result);
        
        entry->SetName("Set Main Voice: " + GameData::getVoiceAsStr(result));
    }

    void heroPointCountSet(MenuEntry* entry) {
        u32 result;
        Keyboard heroPointInput("Set Hero Point count:");
        heroPointInput.IsHexadecimal(false);
        heroPointInput.Open(result);

        if (result > 999) {
            MessageBox(Color::Gainsboro << "Invalid Hero Point count! Cannot be negative OR higher than the maximum of 999.")();
        }
        else {
            Process::Write32(AddressList::HeroPointCount.addr, result);
            entry->SetName("Set Hero Point count: " + result);
        }
    }

    void coliseumWinCountSet(MenuEntry* entry) {
        u32 result;
        Keyboard heroPointInput("Set Coliseum Win count:");
        heroPointInput.IsHexadecimal(false);
        heroPointInput.Open(result);

        if (result > 999) {
            MessageBox(Color::Gainsboro << "Invalid Coliseum Win count! Cannot be negative OR higher than the maximum of 999.")();
        }
        else {
            Process::Write32(AddressList::HeroPointCount.addr, result);
            entry->SetName("Set Coliseum Win count: " + result);
        }
    }

    void merchantSlotA(MenuEntry* entry) {
        std::string material = openMerchantMatMenu(0);
        entry->SetName("Set 1st material slot: " << material);
    }

    void merchantSlotB(MenuEntry* entry) {
        std::string material = openMerchantMatMenu(2);
        entry->SetName("Set 2nd material slot: " << material);
    }

    void merchantSlotC(MenuEntry* entry) {
        std::string material = openMerchantMatMenu(4);
        entry->SetName("Set 3rd material slot: " << material);
    }

    void merchantSlotD(MenuEntry* entry) {
        std::string material = openMerchantMatMenu(6);
        entry->SetName("Set 4th material slot: " << material);
    }

    void merchantSlotE(MenuEntry* entry) {
        std::string material = openMerchantMatMenu(8);
        entry->SetName("Set 5th material slot: " << material);
    }
    
    // since each slot has a 8-bit spacer between them, slot number is just an offset
    // used to navigate between them without having to define 5 separate addresses
    std::string openMerchantMatMenu(u8 slotNumber) {
        int world = Material::selectMaterialWorld();
        int material = Material::selectMaterialIndiv(world);
        
        Process::Write8(AddressList::EditMerchantStock.addr + slotNumber, static_cast<unsigned char>(material));
        return Material::getMaterialName(world, material);
    };

    void resetMerchant(MenuEntry* entry) {
        Process::Write8(AddressList::ResetMerchantStock.addr, 0x0);
        MessageBox(Color::Gainsboro << "Street Merchant Stall has been re-stocked.")();
    }

    void selLevelCompletion(MenuEntry* entry) {
        std::string msg, modeStr;
        int world = GameData::selWorld(false);
        int mode = selPlayMode();

        std::string outro = "\n\nPress " + std::string(FONT_B) + " to save and exit this menu.";

        if (mode < 2)
            modeStr = mode ? "Multiplayer Completion." : "Single-player Completion.";
        else
            modeStr = "Multiplayer and Single-player Completion";

        msg = "Currently editing Level Completion statuses\nfor " << GameData::worldIDToStr(world) << ", " << modeStr <<  outro;
        LevelStatusEditor(msg, GameData::getWorldNamesfromID(world), world, mode)();
    }

    int selPlayMode(void) {
        Keyboard mode("Choose a mode:");
        static const StringVector modeList =
        {
            "Single-player",
            "Multiplayer",
            "Both"
        };

        mode.Populate(modeList);
        return mode.Open();
    }
}