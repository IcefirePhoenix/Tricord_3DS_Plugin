#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
    void mainVoice(MenuEntry* entry) {
        // TODO: 
        // 1) open combo box with 5 options (4 voices + voiceless)
        // 2) simple 8-bit write to voice address
    }

    void heroPointCountSet(MenuEntry* entry) {
        // TODO: 
        // 1) open numerical keyboard 
        // 2) simple write to hero point address 
        
        // Note: we need to convert from decimal to hex here...
    }

    void coliseumWinCountSet(MenuEntry* entry) {
        // TODO: 
        // 1) open numerical keyboard 
        // 2) simple write to coliseum win address 
        
        // Note: we need to convert from decimal to hex here...
    }

    void openMerchantSlots(MenuEntry* entry) {
        // TODO: when enabled, cause the 5 merchant slot options to appear in the menu
    }

    void merchantSlotA(MenuEntry* entry) {
        // TODO:
        // okay not sure tbh
        // 
        // we can either do a numerical keyboard and the user just types in the item ID...
        // OR we open a combo box with A LOT of options to scroll through...
    }

    void merchantSlotB(MenuEntry* entry) {
        // TODO: see merchantSlotA()
    }

    void merchantSlotC(MenuEntry* entry) {
        // TODO: see merchantSlotA()
    }

    void merchantSlotD(MenuEntry* entry) {
        // TODO: see merchantSlotA()
    }

    void merchantSlotE(MenuEntry* entry) {
        // TODO: see merchantSlotA()
    }

    void resetMerchant(MenuEntry* entry) {
        // TODO: simple memory write to merchant reset address
    }
}