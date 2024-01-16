#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/GameData.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/Material.hpp"
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
        }
    }

    void merchantSlotA(MenuEntry* entry) {
       int material = openMerchantMatMenu(0);
       entry->SetName("Set 1st material slot: " << Material::getMaterialName(material));
    }

    void merchantSlotB(MenuEntry* entry) {
        int material = openMerchantMatMenu(2);
        entry->SetName("Set 2nd material slot: " << Material::getMaterialName(material));
    }

    void merchantSlotC(MenuEntry* entry) {
        int material = openMerchantMatMenu(4);
        entry->SetName("Set 3rd material slot: " << Material::getMaterialName(material));
    }

    void merchantSlotD(MenuEntry* entry) {
        int material = openMerchantMatMenu(6);
        entry->SetName("Set 4th material slot: " << Material::getMaterialName(material));
    }

    void merchantSlotE(MenuEntry* entry) {
        int material = openMerchantMatMenu(8);
        entry->SetName("Set 5th material slot: " << Material::getMaterialName(material));
    }
    
    // since each slot has a 8-bit spacer between them, slot number is just an offset
    // used to navigate between them without having to define 5 separate addresses
    int openMerchantMatMenu(u8 slotNumber) {
        int world = Material::selectMaterialWorld();
        int material = Material::selectMaterialIndiv(world);
        
        Process::Write8(AddressList::EditMerchantStock.addr + slotNumber, static_cast<unsigned char>(material));
        return material;
    };

    void resetMerchant(MenuEntry* entry) {
        Process::Write8(AddressList::ResetMerchantStock.addr, 0x0);
        MessageBox(Color::Gainsboro << "Street Merchant Stall has been re-stocked.")();
    }
}