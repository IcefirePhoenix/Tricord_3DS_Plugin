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

    extern MenuEntry* writeCosmeticCostumeID;

    extern MenuEntry* autoInfBalloons;
    extern MenuEntry* autoDapper;
    extern MenuEntry* autoBeamCooldown;

    extern MenuEntry* autoCostumeRandomizer;

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
        static void forceBearMinMax(MenuEntry* entry);
        static void preventLobbyReset(MenuEntry* entry);
        static void enableCosmeticCostume(MenuEntry* entry);
        static void setCosmeticCostume(MenuEntry* entry);
        static void writeCosmeticCostume(MenuEntry* entry);

        // Individual costume effects

        static void luckyDodge(MenuEntry* entry);
        static void tingle(MenuEntry* entry);
        static void writeBalloons(MenuEntry* entry);
        static void dapperInstant(MenuEntry* entry);
        static void writeDapper(MenuEntry* entry);
        static void selectLinkBeam(MenuEntry* entry);
        static void setBeamCooldown(MenuEntry* entry);

        static void setIndCostumeEffect(MenuEntry* entry);
        static void writeIndCostumeEffects(MenuEntry* entry);

        // All-player costume effects

        // Item costumes
        static void kokiri(MenuEntry* entry);
        static void bigBomb(MenuEntry* entry);
        static void torrent(MenuEntry* entry);
        static void boomeranger(MenuEntry* entry);
        static void fireBlazer(MenuEntry* entry);
        static void gustGarb(MenuEntry* entry);
        static void robowear(MenuEntry* entry);
        static void hammerwear(MenuEntry* entry);

        // Other
        static void goron(MenuEntry* entry);
        static void lightArmor(MenuEntry* entry);
        static void linebeck(MenuEntry* entry);
        static void cheetah(MenuEntry* entry);
        static void swordDmgBoost(MenuEntry* entry);
        static void swordBeam(MenuEntry* entry);
        static void knockbackImmunity(MenuEntry* entry);

        // Bonus - Exe
        static void setCheetahMultiplier(MenuEntry* entry);
        static void setCactoDmg(MenuEntry* entry);
        static void setSwordHammerDmgMult(MenuEntry* entry);
        static void swordBeamsAnyHealth(MenuEntry* entry);

        // Costume Randomizers
        static void costumeRandomizer(MenuEntry* entry);
        static void writeRandomCostume(MenuEntry* entry);
    };
}
#endif