#ifndef ITEM_HPP
#define ITEM_HPP

#include "CTRPluginFramework.hpp"
#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Unicode.h"

namespace CTRPluginFramework {
    float setSpeedKB(void);
    u32 getShadowItemAddress(void);
    void saveCurrItems(void);
    void initItemAddresses(void);
    void initShadowItemList(void);
    void readCurrStrafingSpeeds(void);
    void resetCurrStrafingSpeeds(void);
    std::string readCurrItems(u32 address, bool useShadow);

    class Item {
    public:
    	static void removeCurrItems(MenuEntry* entry);
        static void freezeCurrItems(MenuEntry* entry);
        static void itemOpt(MenuEntry* entry);
        static void shadowItemOpt(MenuEntry* entry);
        static void strafingSpeedSet(MenuEntry* entry);
        static void upgradeItemAlways(MenuEntry* entry);

    };
}
#endif