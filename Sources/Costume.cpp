#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
    void openCustomCostumeSlots(MenuEntry* entry) {
        // TODO: when enabled, cause the 4 custom costume slot
        // menu entries to appear in the plugin menu
    }


    void customCostumeSlotA(MenuEntry* entry) {
        // TODO: basically replicate/automate the current process
        // of enabling the costume slots
    }


    void customCostumeSlotB(MenuEntry* entry) {
        // TODO: see customCostumeSlotA()
    }
    

    void customCostumeSlotC(MenuEntry* entry) {
        // TODO: see customCostumeSlotA()
    }


    void customCostumeSlotD(MenuEntry* entry) {
        // TODO: see customCostumeSlotA()
        // Keep in mind that if greatFairyEnable() is enabled, 
        // the Great Fairy costume will be placed in this slot
        //
        // Therefore, we need to disable (gray out?) this entry if 
        // greatFairyEnable() is enabled AND VICE VERSA
    }
    

    void greatFairyEnable(MenuEntry* entry) {
        // TODO: automate/replicate process of restoring
        // the great fairy costume, using available padding areas

        // **see customCostumeSlotD()
    }
    

    void gChangeCostume(MenuEntry* entry) {
        // TODO:
        // 1) open a Combo Box and populate it with the list of 
        // available costumes, including the 4 DLC costumes (remember that 
        // by default, these are the Hero's Tunic, so there are no underlying 
        // issues here)
        // 2) do a simple 8-bit write to the correct costume address(es)

    }
    

    void bChangeCostume(MenuEntry* entry) {
        // TODO: see gChangeCostume()
    }
    

    void rChangeCostume(MenuEntry* entry) {
        // TODO: see gChangeCostume()
    }
}