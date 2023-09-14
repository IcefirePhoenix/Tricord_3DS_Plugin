#ifndef CHEATS_H
#define CHEATS_H

#include <CTRPluginFramework.hpp>
#include "Helpers.hpp"
#include "Unicode.h"
#include "AddressList.hpp"
#include <3ds.h>

namespace CTRPluginFramework
{
    using StringVector = std::vector<std::string>;
    extern MenuEntry* menuCostumeSlotA;
    extern MenuEntry* menuCostumeSlotB;
    extern MenuEntry* menuCostumeSlotC;
    extern MenuEntry* menuCostumeSlotD;
    extern MenuEntry* restoreGreatFairy;
    extern MenuEntry* manageCatalogSize;
    extern MenuEntry* initCustomCostumes;
    extern MenuEntry* writeCostumeIDToSlot;




    // Emote functions
    void emoteSwapper(MenuEntry* entry);
    void customEmotes(MenuEntry* entry);

    // Link Color functions
    void customColorG(MenuEntry* entry);
    void customColorB(MenuEntry* entry);
    void customColorR(MenuEntry* entry);
    int selectLinkColor(void);
    void recolorLevelTex(MenuEntry* entry);

    // Costume functions
    extern bool showSlots;
    void manageCatalogSizeAuto(MenuEntry* entry);
    void openCustomCostumeSlots(MenuEntry* entry);
    void writeCostumeSlot(MenuEntry* entry);
    void greatFairyEnable(MenuEntry* entry);
    void changeLinkCostume(MenuEntry* entry);
    void selectCostumeID(MenuEntry* entry);
    void initCustomCostumesAuto(MenuEntry* entry);

    

    // Miscellaneous functions
    void buttonSpammer(MenuEntry* entry);
    void instantText(MenuEntry* entry);
    void beamCooldown(MenuEntry* entry);

    // Player functions
    void posEditor(MenuEntry* entry);
    void openJinxOptions(MenuEntry* entry);
    void gJinx(MenuEntry* entry);
    void bJinx(MenuEntry* entry);
    void rJinx(MenuEntry* entry);
    void openSpawnOptions(MenuEntry* entry);
    void gSpawn(MenuEntry* entry);
    void bSpawn(MenuEntry* entry);
    void rSpawn(MenuEntry* entry);
    void waterStorage(MenuEntry* entry);
    void openVisibleOptions(MenuEntry* entry);
    void gVisible(MenuEntry* entry);
    void bVisible(MenuEntry* entry);
    void rVisible(MenuEntry* entry);
    void openInvincibleOptions(MenuEntry* entry);
    void gInvincible(MenuEntry* entry);
    void bInvincible(MenuEntry* entry);
    void rInvincible(MenuEntry* entry);
    // I don't really know if I wanna add these... they seem a bit unnecessary tbh
    //void openVoiceOptions(MenuEntry* entry);
    //int gVoiceClient(void);
    //int bVoiceClient(void);
    //int rVoiceClient(void);
    //int gVoice;
    //int bVoice;
    //int rVoice;
    //bool gVoiceChosen;
    //bool bVoiceChosen;
    //bool rVoiceChosen;
    void removeCollision(MenuEntry* entry);
    void linkSize(MenuEntry* entry);
    void pvpModeEnable(MenuEntry* entry);
    void openSwordOptions(MenuEntry* entry);
    void gSwordChoose(MenuEntry* entry);
    void bSwordChoose(MenuEntry* entry);
    void rSwordChoose(MenuEntry* entry);

    // Energy functions
    void infEnergy(MenuEntry* entry);
    void maxEnergySet(MenuEntry* entry);
    void energyConsumeMultiplier(MenuEntry* entry);


    // Item functions
    void openItemOptions(MenuEntry* entry);
    void gSetItem(MenuEntry* entry);
    void bSetItem(MenuEntry* entry);
    void rSetItem(MenuEntry* entry);
    void openShadowItemOptions(MenuEntry* entry);
    void gShadowSetItem(MenuEntry* entry);
    void bShadowSetItem(MenuEntry* entry);
    void rShadowSetItem(MenuEntry* entry);
    void strafingSpeedSet(MenuEntry* entry);
    void upgradeItemAlways(MenuEntry* entry);

    // Rendering functions
    void hideHUD(MenuEntry* entry);
    void disableFog(MenuEntry* entry);


    // Save functions
    void mainVoice(MenuEntry* entry);
    void heroPointCountSet(MenuEntry* entry);
    void coliseumWinCountSet(MenuEntry* entry);
    void openMerchantSlots(MenuEntry* entry);
    void merchantSlotA(MenuEntry* entry);
    void merchantSlotB(MenuEntry* entry);
    void merchantSlotC(MenuEntry* entry);
    void merchantSlotD(MenuEntry* entry);
    void merchantSlotE(MenuEntry* entry);
    void resetMerchant(MenuEntry* entry);

    // Sound functions
    void bgmSet(MenuEntry* entry);
    void bgmVolSet(MenuEntry* entry);
    void lobbyBallSong(MenuEntry* entry);
    void voiceVol(MenuEntry* entry);
    void lowHPVol(MenuEntry* entry);
}
#endif
