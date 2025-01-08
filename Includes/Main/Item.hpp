#ifndef ITEM_HPP
#define ITEM_HPP

#define NOITEM 0x9

#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // helper functions
    void saveCurrItems(void);
    void initShadowItemList(void);
    void readCurrStrafingSpeeds(void);
    void resetCurrStrafingSpeeds(void);

    float setSpeedKB(void);

    u32 getShadowItemAddress(void);

    std::string readCurrItems(u32 address, bool useShadow);

    class Item
    {
    public:
        static void initItemAddresses(void);
        static void manageItems(MenuEntry* entry);
        static void itemOpt(MenuEntry* entry);
        static void shadowItemOpt(MenuEntry* entry);
        static void strafingSpeedSet(MenuEntry* entry);
        static void upgradeItemAlways(MenuEntry* entry);

        static StringVector shadowItemList;
        static u8 currItems[3];
        static u32 addresses[6];
    };
}
#endif