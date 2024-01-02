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

    // extern MenuEntry* resetCostume;
    extern MenuEntry* menuCostumeSlotA;
    extern MenuEntry* menuCostumeSlotB;
    extern MenuEntry* menuCostumeSlotC;
    extern MenuEntry* menuCostumeSlotD;

    extern MenuEntry* restoreGreatFairy;
    extern MenuEntry* manageCatalogSize;

    extern MenuEntry* initCustomCostumes;
    extern MenuEntry* writeCostumeIDToSlot;

    extern MenuEntry* resetMiscellaneous;
    extern MenuEntry* instantTextDisplay;

    extern MenuEntry* managePlayerCodes;
    extern MenuEntry* menuFreecam;
    extern MenuEntry* editFreecamControls;
    extern MenuEntry* editFreecamSen;

    
    // Menu functions
    void InitFreecamCodes(PluginMenu& menu);
    void InitEmoteFolder(PluginMenu& menu);
    void InitColorFolder(PluginMenu& menu);
    void InitCostumeFolder(PluginMenu& menu);
    void InitMiscFolder(PluginMenu& menu);
    void InitPlayerFolder(PluginMenu& menu);
    void InitEnergyFolder(PluginMenu& menu);
    void InitItemsFolder(PluginMenu& menu);
    void InitRenderFolder(PluginMenu& menu);
    void InitSaveFolder(PluginMenu& menu);
    void InitSoundFolder(PluginMenu& menu);
    void AddAllFolders(PluginMenu& menu);


    // Emote functions
    void lobbyEmoteSwapper(MenuEntry* entry);
    void drablandEmoteSwapper(MenuEntry* entry);
    void customEmotes(MenuEntry* entry);
    void refreshEmoteGraphics(const u32 color[][4], std::vector<u8>& layout, bool useBlank);

    // Freecam functions
    void useFreecam(MenuEntry* entry);
    void shiftCamNorth(void);
    void shiftCamSouth(void);
    void shiftCamEast(void);
    void shiftCamWest(void);
    void zoomCamIn(void);
    void zoomCamOut(void);
    void raiseCam(void);
    void lowerCam(void);
    void rotateCamXCounter(void);
    void rotateCamXClockwise(void);
    void rotateCamZCounter(void);
    void rotateCamZClockwise(void);
    void editHotkeys(MenuEntry* entry);
    void lockCamera(void);
    void managePlayerLock(void);
    void editSensitivity(MenuEntry* entry);
    float setSensitivity(std::string message);

    // Link Color functions
    void customColor(MenuEntry* entry);
    int selCustomLinkColor(void);
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
    void selCostumeEffect(MenuEntry* entry);

    // Miscellaneous functions
    void buttonSpammer(MenuEntry* entry);
    void displayPhoto(MenuEntry* entry);
    void instantText(MenuEntry* entry);
    void beamCooldown(MenuEntry* entry);
    void defaultMisc(MenuEntry* entry);
    void seeChestContents(MenuEntry* entry);

    // Player functions
    void ToggleMenuChange(void);
    void resetPlayer(MenuEntry* entry);
    void setPlayerChanges(MenuEntry* entry);
    void posEditor(MenuEntry* entry);
    void jinxOpt(MenuEntry* entry);
    void spawnOpt(MenuEntry* entry);
    void visibilityOpt(MenuEntry* entry);
    void invincibleOpt(MenuEntry* entry);
    void waterStorage(MenuEntry* entry);
    void removeCollision(MenuEntry* entry);
    void pvpMode(MenuEntry* entry);
    void respawnIndicator(MenuEntry* entry);
    void swordModelOpt(MenuEntry* entry);
    void linkSize(MenuEntry* entry);
    void bypassDoppelDemo(MenuEntry* entry);

    // Energy functions
    void infEnergy(MenuEntry* entry);
    void maxEnergySet(MenuEntry* entry);
    void energyConsumeMultiplier(MenuEntry* entry);


    // Item functions
    void itemOpt(MenuEntry* entry);
    void shadowItemOpt(MenuEntry* entry);
    void strafingSpeedSet(MenuEntry* entry);
    void upgradeItemAlways(MenuEntry* entry);

    // Rendering functions
    void hideHUD(MenuEntry* entry);
    void disableFog(MenuEntry* entry);
    void disableScrollingText(MenuEntry* entry);

    // Save functions
    void useVoiceless(MenuEntry* entry);
    void mainVoice(MenuEntry* entry);
    void heroPointCountSet(MenuEntry* entry);
    void coliseumWinCountSet(MenuEntry* entry);
    void openMerchantMatMenu(int slotNumber);
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
