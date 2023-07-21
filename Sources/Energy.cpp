#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
    void infEnergy(MenuEntry* entry) {
        // TODO: simple 32-bit (8-bit??) write to infinite energy address and repeat for all Links
        // Note: uhh what's with those addresses in the spreadsheet?
    }

    void maxEnergySet(MenuEntry* entry) {
        // TODO: simple 8-bit write to max energy address and repeat for all Links
        // Note: uhh what's with those addresses in the spreadsheet??
    }

    void energyConsumeMultiplier(MenuEntry* entry) {
        // TODO: simple 32-bit write to energy consumption address and repeat for all Links
    }
}