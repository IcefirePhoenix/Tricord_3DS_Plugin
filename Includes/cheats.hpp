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
    extern MenuEntry* triggerCostumeSlots;

    extern MenuEntry* restoreGreatFairy;
    extern MenuEntry* manageCatalogSize;

    extern MenuEntry* initCustomCostumes;
    extern MenuEntry* writeCostumeIDToSlot;

    extern MenuEntry* autoBeamCooldown;
    extern MenuEntry* instantTextDisplay;

    extern MenuEntry* menuFreecam;
    extern MenuEntry* editFreecamControls;
    extern MenuEntry* editFreecamSen;
    extern MenuEntry* swapZoom;

    extern MenuEntry* jinxEditAuto;
    extern MenuEntry* spawnEditAuto;
    extern MenuEntry* visibleEditAuto;
    extern MenuEntry* invinciEditAuto;
    extern MenuEntry* pvpEditAuto;
    extern MenuEntry* animStoreEditAuto;
    extern MenuEntry* colEditAuto;
    extern MenuEntry* swordEditAuto;
    extern MenuEntry* sizeEditAuto;


    // move to player class later
    enum ColorStatus {
        greenEnabled = 1 << 0, // 0001
        blueEnabled = 1 << 1,  // 0010
        redEnabled = 1 << 2    // 0100
    };

    enum EditType {
        NORMAL,
        SWORD,
        PLAYER_SIZE
    };
    
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
    void manageFreecamPlayerLock(void);
    void editSensitivity(MenuEntry* entry);
    float setSensitivity(std::string message);
    void setZoomType(MenuEntry* entry);

    // Link Color functions
    void customColor(MenuEntry* entry);
    int selCustomLinkColor(void);
    void recolorLevelTex(MenuEntry* entry);

    // Costume functions
    void manageCatalogSizeAuto(MenuEntry* entry);
    void openCustomCostumeSlots(MenuEntry* entry);
    void writeCostumeSlot(MenuEntry* entry);
    void greatFairyEnable(MenuEntry* entry);
    void changeLinkCostume(MenuEntry* entry);
    void selectCostumeID(MenuEntry* entry);
    void initCustomCostumesAuto(MenuEntry* entry);
    void selCostumeEffect(MenuEntry* entry);
    int chooseSlotAddNum(void);
    void disableCustomCostumeMenuOpt(void);
    void enableCustomCostumeOpt(void);
    bool isCustomCostumeConflict(int openSlots);
    void addCustomSlotsToMenu(int openSlots);
    void resetCostumeArray(void);
    int chooseSlotAddNum(void);
    bool isCatalogPointerNull(void);



    // Miscellaneous functions
    void buttonSpammer(MenuEntry* entry);
    void managePhotoDisp(MenuEntry* entry);
    void instantText(MenuEntry* entry);
    void selectLinkBeam(MenuEntry* entry);
    void seeChestContents(MenuEntry* entry);
    void setBeamCooldown(MenuEntry* entry);
    void showPhotoBtnIntroMsg(bool showMsg);
    void displayPhoto(bool photoCheck);


    // Player functions
    
    void openToggleMenu(std::string entryTitle, EditType type, u8 *playerStatus = nullptr);
    void togglePlayerStatus(u8 &playerStatus, ColorStatus color);
    bool checkPlayerStatus(u8 &playerStatus, int playerID);

    template <typename T>
    void writePlayerChanges(int editSize, u8& playerStatus, u32 address, T trueValue, T falseValue);
    void writeSwordChanges(MenuEntry* entry);
    void writeSizeChanges(MenuEntry* entry);
    void writeJinxChanges(MenuEntry* entry);
    void writeSpawnChanges(MenuEntry* entry);
    void writeVisibilityChanges(MenuEntry* entry);
    void writeInvincibilityChanges(MenuEntry* entry);
    void writeAnimStorageChanges(MenuEntry* entry);
    void writeCollisionChanges(MenuEntry* entry);
    void writePVPChanges(MenuEntry* entry);

    void setSwordChanges(MenuEntry* entry);
    void setSizeChanges(MenuEntry* entry);
    void setJinxChanges(MenuEntry* entry);
    void setSpawnChanges(MenuEntry* entry);
    void setVisibilityChanges(MenuEntry* entry);
    void setInvincibilityChanges(MenuEntry* entry);
    void setAnimStorageChanges(MenuEntry* entry);
    void setCollisionChanges(MenuEntry* entry);
    void setPVPChanges(MenuEntry* entry);
    bool initEdit(EditType type, int playerID, ColorStatus color, u8 &playerStatus);

    float setPlayerSize(void);

    void posEditor(MenuEntry* entry);
    void loadPlayerPos(int playerID);
    void savePlayerPos(int playerID);
    void resetPositionEditorSaves(bool reset);
    void respawnIndicator(MenuEntry* entry);
    void bypassDoppelDemo(MenuEntry* entry);
    
    // Energy functions
    void infEnergy(MenuEntry* entry);
    void maxEnergySet(MenuEntry* entry);
    void energyConsumeMultiplier(MenuEntry* entry);


    // Item functions
    std::string readCurrItems(u32 address, bool useShadow);
    void itemOpt(MenuEntry* entry);
    void shadowItemOpt(MenuEntry* entry);
    void strafingSpeedSet(MenuEntry* entry);
    void readCurrStrafingSpeeds(void);
    void upgradeItemAlways(MenuEntry* entry);
    float setSpeedKB(void);

    // Rendering functions
    bool isNullPointer(u32 address); // move to helper file
    void triggerHideHUD(MenuEntry* entry);
    void hideHUD(void);
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
