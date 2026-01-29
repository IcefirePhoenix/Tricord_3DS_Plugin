#pragma once


namespace CTRPluginFramework
{
    extern MenuEntry *DLC_SlotWriterA;
    extern MenuEntry *TricordUseDLC_SlotA;

    extern MenuEntry* cosmeticCostumeAuto;
    extern MenuEntry* doppelCostumeStageResetManager;

    extern MenuEntry* costumeRandomizerAuto;

    extern MenuEntry* infBalloonsAuto;
    extern MenuEntry* dapperAuto;
    extern MenuEntry* beamCooldownAuto;
    extern MenuEntry *swordEditAuto;

    static const u8 cosmeticNotInUse = 0xFF;

    class Costume
    {
    public:
        static void toggleTricordCustomCostumeUsage(MenuEntry *entry);
        static void overrideDLC_CostumeSlotA(MenuEntry *entry);
        static void setDLCEntryTitles(void);
        static void changeLinkCostume(MenuEntry *entry);
        static void setPlayerCostume(int player, int costumeID, bool useAltOnly = false);
        static int selectCostumeID(void);
        static void forceBearMinMax(MenuEntry* entry);
        static void setSwordChanges(MenuEntry *entry);
        static void writeSwordChanges(MenuEntry *entry);
        static void preventDoppelStageReset(MenuEntry* entry);
        static void preventDoppelLobbyReset(MenuEntry* entry);
        static void enableCosmeticCostume(MenuEntry* entry);
        static void setCosmeticCostume(MenuEntry* entry);
        static void writeCosmeticCostume(MenuEntry* entry);
        static void costumeRandomizer(MenuEntry* entry);
        static void writeRandomCostume(MenuEntry* entry);
        static void toggleRandomizeOnDefeat(MenuEntry* entry);

        // Individual costume effects

        static void luckyDodge(MenuEntry* entry);
        static void tingle(MenuEntry* entry);
        static void writeBalloons(MenuEntry* entry);
        static void dapperInstant(MenuEntry* entry);
        static void writeDapper(MenuEntry* entry);
        static void selectLinkBeam(MenuEntry* entry);
        static void writeBeamCooldown(MenuEntry* entry);

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

        static u8 cosmeticIDs[3];
        static u8 swordType[3];
        static bool customSwordsActive;
        static float customSwordHammerDmgMult;
        static bool forcingSwordDmgBoost;
    };
}
