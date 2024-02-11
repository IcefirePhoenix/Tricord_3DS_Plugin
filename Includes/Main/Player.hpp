#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "CTRPluginFramework.hpp"
#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Unicode.h"

namespace CTRPluginFramework {
    extern MenuEntry* jinxEditAuto;
    extern MenuEntry* spawnEditAuto;
    extern MenuEntry* visibleEditAuto;
    extern MenuEntry* invinciEditAuto;
    extern MenuEntry* pvpEditAuto;
    extern MenuEntry* animStoreEditAuto;
    extern MenuEntry* colEditAuto;
    extern MenuEntry* swordEditAuto;
    extern MenuEntry* sizeEditAuto;

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

    template <typename T>
    void writePlayerChanges(int editSize, u8& playerStatus, u32 address, T trueValue, T falseValue);
    void loadPlayerPos(int playerID);
    void savePlayerPos(int playerID);
    void resetPositionEditorSaves(bool reset);
    void openToggleMenu(std::string entryTitle, EditType type, u8* playerStatus = nullptr);
    void togglePlayerStatus(u8& playerStatus, ColorStatus color);
    void initEdit(EditType type, int playerID, ColorStatus color, u8& playerStatus);

    bool checkPlayerStatus(u8& playerStatus, int playerID);

    float setPlayerSize(void);

    // TODO: subclasses for organization?

    class Player {
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

    };
}
#endif