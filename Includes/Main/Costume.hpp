#ifndef COSTUME_HPP
#define COSTUME_HPP

#include "CTRPluginFramework.hpp"
#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Unicode.h"

namespace CTRPluginFramework {
    // MenuEntry* resetCostume;
    extern MenuEntry* menuCostumeSlotA;
    extern MenuEntry* menuCostumeSlotB;
    extern MenuEntry* menuCostumeSlotC;
    extern MenuEntry* menuCostumeSlotD;
    extern MenuEntry* triggerCostumeSlots;

    extern MenuEntry* restoreGreatFairy;
    extern MenuEntry* manageCatalogSize;

    extern MenuEntry* initCustomCostumes;
    extern MenuEntry* writeCostumeIDToSlot;

    int chooseSlotAddNum(void);
    void disableCustomCostumeMenuOpt(void);
    void enableCustomCostumeOpt(void);
    bool isCustomCostumeConflict(int openSlots);
    void addCustomSlotsToMenu(int openSlots);
    void resetCostumeArray(void);

    class Costume {
    public:
        static void manageCatalogSizeAuto(MenuEntry* entry);
        static void openCustomCostumeSlots(MenuEntry* entry);
        static void writeCostumeSlot(MenuEntry* entry);
        static void greatFairyEnable(MenuEntry* entry);
        static void changeLinkCostume(MenuEntry* entry);
        static void selectCostumeID(MenuEntry* entry);
        static void initCustomCostumesAuto(MenuEntry* entry);
        static void selCostumeEffect(MenuEntry* entry);
        static void forceBearMinMax(MenuEntry* entry);
    };
}
#endif