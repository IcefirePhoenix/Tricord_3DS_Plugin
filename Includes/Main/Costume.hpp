#pragma once


namespace CTRPluginFramework
{
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

    extern MenuEntry* writeCosmeticCostumeID;

    int chooseSlotAddNum(void);
    void selCosmeticCostume(int player);
    void disableCustomCostumeMenuOpt(void);
    void enableCustomCostumeOpt(void);
    void checkCustomCostumeConflict(void);
    void addCustomSlotsToMenu(int openSlots);
    void resetCostumeArray(void);

    class Costume
    {
    public:
        static void manageCatalogSizeAuto(MenuEntry* entry);
        static void openCustomCostumeSlots(MenuEntry* entry);
        static void writeToCostumeSlot(MenuEntry* entry);
        static void greatFairyEnable(MenuEntry* entry);
        static void changeLinkCostume(MenuEntry* entry);
        static void selectCostumeID(MenuEntry* entry);
        static void initCustomCostumesAuto(MenuEntry* entry);
        static void forceBearMinMax(MenuEntry* entry);
        static void preventLobbyReset(MenuEntry* entry);
        static void enableCosmeticCostume(MenuEntry* entry);
        static void setCosmeticCostume(MenuEntry* entry);
        static void writeCosmeticCostume(MenuEntry* entry);
        static void changeSlotMappings(MenuEntry *entry);

        static const int unusedDLC_slotIDs[4];
        static const StringVector BCH_fileNames;
    };
}
