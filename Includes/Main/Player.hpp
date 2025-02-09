#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "CTRPF.hpp"
#include "Cheats.hpp"
#include "Helpers.hpp"
#include "Unicode.h"

namespace CTRPluginFramework
{
    extern MenuEntry* jinxEditAuto;
    extern MenuEntry* spawnEditAuto;
    extern MenuEntry* visibleEditAuto;
    extern MenuEntry* invinciEditAuto;
    extern MenuEntry* pvpEditAuto;
    extern MenuEntry* animStoreEditAuto;
    extern MenuEntry* colEditAuto;
    extern MenuEntry* swordEditAuto;
    extern MenuEntry* sizeEditAuto;
    extern MenuEntry* rotationOffsetManager;

    enum ColorStatus
    {
        greenEnabled = 1 << 0, // 0001
        blueEnabled = 1 << 1,  // 0010
        redEnabled = 1 << 2    // 0100
    };

    enum EditType
    {
        NORMAL,
        SWORD,
        PLAYER_SIZE
    };

    void openToggleMenu(std::string entryTitle, EditType type, u8* playerStatus = nullptr);
    void togglePlayerStatus(u8& playerStatus, ColorStatus color);
    void initEdit(EditType type, int playerID, ColorStatus color, u8& playerStatus);
    bool checkPlayerStatus(u8 &playerStatus, int playerID);
    float setPlayerSize(void);

    void loadPlayerPos(int playerID);
    void savePlayerPos(int playerID);
    void resetPositionEditorSaves(bool reset);
    void showPosEditorIntroMsg(bool showMsg);

    class Player
    {
    public:
        static void writeSwordChanges(MenuEntry* entry);
        static void writeJinxChanges(MenuEntry* entry);
        static void writeSpawnChanges(MenuEntry* entry);
        static void writeVisibilityChanges(MenuEntry* entry);
        static void writeInvincibilityChanges(MenuEntry* entry);
        static void writeAnimStorageChanges(MenuEntry* entry);
        static void writeCollisionChanges(MenuEntry* entry);
        static void writePVPChanges(MenuEntry* entry);

        static void setSwordChanges(MenuEntry* entry);
        static void setSizeChanges(MenuEntry* entry);
        static void setJinxChanges(MenuEntry* entry);
        static void setSpawnChanges(MenuEntry* entry);
        static void setVisibilityChanges(MenuEntry* entry);
        static void setInvincibilityChanges(MenuEntry* entry);
        static void setAnimStorageChanges(MenuEntry* entry);
        static void setCollisionChanges(MenuEntry* entry);
        static void setPVPChanges(MenuEntry* entry);

        static void posEditor(MenuEntry* entry);

        static void bypassDoppelDemo(MenuEntry* entry);
        static void doppelsToPlayer(MenuEntry* entry);

        static void disableOffset(MenuEntry *entry);
        static void resetOffset(void);
        static void overwriteOffset(double adjustment);

        template <typename T>
        static void writePlayerChanges(int editSize, u8 &playerStatus, u32 address, T trueValue, T falseValue);

        static u8 jinxStatus, spawnStatus, visibleStatus, invinciStatus, animStoreStatus, collisionStatus, pvpStatus;
        static u8 swordType[3];
        static float PlayerSizes;
    };
}
#endif