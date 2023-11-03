#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/PlayerHelper.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
    void useVoiceless(MenuEntry* entry) {
        // this is a checkbox; auto-overwrites mainVoice
        Process::Write8(AddressList::MainVoice.addr, 0x4);
    }

    void mainVoice(MenuEntry* entry) {
        Keyboard voiceMenu("Choose a voice:\n\nThese changes can be observable online. Keep in\nmind your selection will be overwritten by Voiceless,\nif used.");
        voiceMenu.Populate(voiceList);

        u8 result = voiceMenu.Open();
        Process::Write8(AddressList::MainVoice.addr, result);
    }

    void heroPointCountSet(MenuEntry* entry) {
        u32 result;
        Keyboard heroPointInput("Set Hero Point count:");
        heroPointInput.IsHexadecimal(false);
        heroPointInput.Open(result);

        // no need to check for negatives -- if one is inputted, it automatically turns into the 32-bit positive limit
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
        openMerchantMatMenu(0);
    }

    void merchantSlotB(MenuEntry* entry) {
        openMerchantMatMenu(2);
    }

    void merchantSlotC(MenuEntry* entry) {
        openMerchantMatMenu(4);
    }

    void merchantSlotD(MenuEntry* entry) {
        openMerchantMatMenu(6);
    }

    void merchantSlotE(MenuEntry* entry) {
        openMerchantMatMenu(8);
    }
    
    // slot number is just an offset used to navigate between them without having to define 5 separate addresses
    // note: each slot has a 8-bit spacer between them; increment by 2
    void openMerchantMatMenu(int slotNumber) {
        int world = selectMaterialWorld();
        int material = selectMaterialIndiv(world);
        
        Process::Write8((AddressList::EditMerchantStock.addr + static_cast<unsigned char>(slotNumber)), static_cast<unsigned char>(material));
    };

    void resetMerchant(MenuEntry* entry) {
        Process::Write8(AddressList::ResetMerchantStock.addr, 0x0);
        MessageBox(Color::Gainsboro << "Street Merchant Stall has been re-stocked.")();
    }
}