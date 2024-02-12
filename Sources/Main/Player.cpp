#include "Helpers.hpp"
#include "Main/Player.hpp"

#include "CTRPluginFrameworkImpl/Graphics/Icon.hpp"
#include "CTRPluginFramework/System/Touch.hpp"
#include "CTRPluginFrameworkImpl/Graphics/FloatingButton.hpp"

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework
{
    MenuEntry* jinxEditAuto;
    MenuEntry* spawnEditAuto;
    MenuEntry* visibleEditAuto;
    MenuEntry* invinciEditAuto;
    MenuEntry* pvpEditAuto;
    MenuEntry* animStoreEditAuto;
    MenuEntry* colEditAuto;
    MenuEntry* swordEditAuto;
    MenuEntry* sizeEditAuto;

    // TODO: subclasses for organization
    // TODO: Locate secondary sword model size values -> Player Size edits only affect the primary size variable
    // causing sword models to revert back to regular size during specific animations
    // also sword particle size remains unaffected in general...

    u8 jinxStatus, spawnStatus, visibleStatus, invinciStatus, animStoreStatus, collisionStatus, pvpStatus, sizeStatus, swordStatus;
    u8 swordType[3] = { 0xFF, 0xFF, 0xFF };

    float PlayerSizes = 1.0;

    /*-------------------------*/

    void togglePlayerStatus(u8 &playerStatus, ColorStatus color) 
    {
        playerStatus ^= static_cast<u8>(color);
    }

    bool checkPlayerStatus(u8 &playerStatus, int playerID) 
    {
        if (playerStatus & (1 << playerID))
            return true;
        return false;
    }

    template <typename T>
    void writePlayerChanges(int editSize, u8 &playerStatus, u32 address, T trueValue, T falseValue) 
    {
        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; ++iterateThruPlayers) {
            T valueToWrite = checkPlayerStatus(playerStatus, iterateThruPlayers) ? trueValue : falseValue;
            u32 finalAddress = address + (iterateThruPlayers * GameData::playerAddressOffset);

            switch (editSize) {
            case 8:
                Process::Write8(finalAddress, valueToWrite);
                break;
            case 16:
                Process::Write16(finalAddress, valueToWrite);
                break;
            case 32:
                Process::Write32(finalAddress, valueToWrite);
                break;
            default:
                Process::WriteFloat(finalAddress, valueToWrite);
                break;
            }
        }
    }

    void initEdit(EditType type, int playerID, ColorStatus color, u8& playerStatus) 
    {
        switch (type) {
        case NORMAL:
            togglePlayerStatus(playerStatus, color);
            break;
        case SWORD:
        {
            int swordChoice = GeneralHelpers::chooseSword();

            if (swordChoice >= 0)
                swordType[playerID] = static_cast<u8>(swordChoice);
        }
            break;
        case PLAYER_SIZE:
            PlayerSizes = setPlayerSize();
            break;
        }
    }

    void openToggleMenu(std::string entryTitle, EditType type, u8* playerStatus) 
    {
        Keyboard menu("Menu");
        StringVector bottomScreenOptions;

        bool isMenuOpen = true;
        menu.CanAbort(false);

        while (isMenuOpen) {
            u8 status = 0x0;

            if (playerStatus != nullptr)
                status = *playerStatus;
            
            bottomScreenOptions.clear();

            switch (type) {
            case NORMAL:
                bottomScreenOptions.push_back(std::string("Player 1 ") << (checkPlayerStatus(status, 0) ? GeneralHelpers::enabledSlider : GeneralHelpers::disabledSlider));
                bottomScreenOptions.push_back(std::string("Player 2 ") << (checkPlayerStatus(status, 1) ? GeneralHelpers::enabledSlider : GeneralHelpers::disabledSlider));
                bottomScreenOptions.push_back(std::string("Player 3 ") << (checkPlayerStatus(status, 2) ? GeneralHelpers::enabledSlider : GeneralHelpers::disabledSlider));
                break;
            case SWORD:
                bottomScreenOptions.push_back(std::string("Player 1: ") << GameData::getSwordNameFromID(swordType[0]));
                bottomScreenOptions.push_back(std::string("Player 2: ") << GameData::getSwordNameFromID(swordType[1]));
                bottomScreenOptions.push_back(std::string("Player 3: ") << GameData::getSwordNameFromID(swordType[2]));
                break;
            case PLAYER_SIZE:
                bottomScreenOptions.push_back(std::string("Player Size: ") << std::to_string(PlayerSizes));
                break;
            }

            bottomScreenOptions.push_back("Save and exit");

            menu.GetMessage() = entryTitle;
            menu.Populate(bottomScreenOptions);

            switch (menu.Open()) {
            case 0:
                initEdit(type, 0, greenEnabled, *playerStatus);
                break;
            case 1:
                if (type == PLAYER_SIZE) {
                    isMenuOpen = false;
                    break;
                }

                initEdit(type, 1, blueEnabled, *playerStatus);
                break;
            case 2:
                initEdit(type, 2, redEnabled, *playerStatus);
                break;
            case 3:
                isMenuOpen = false;
                break;
            }
        }
    }

    /*-------------------------*/

    void Player::writeJinxChanges(MenuEntry* entry) 
    {
        u32 jinxed = 0x00000000;
        u32 notJinxed = 0xFFFFFFFF;

        writePlayerChanges(32, jinxStatus, AddressList::IsJinxed.addr, jinxed, notJinxed);
    }

    void Player::writeSpawnChanges(MenuEntry* entry) 
    {
        u8 spawned = 0x1;
        u8 notSpawned = 0x0;

        writePlayerChanges(8, spawnStatus, AddressList::IsSpawned.addr, spawned, notSpawned);
    }

    void Player::writeVisibilityChanges(MenuEntry* entry) 
    {
        u8 visible = 0x1;
        u8 notVisible = 0x0;

        writePlayerChanges(8, visibleStatus, AddressList::IsVisible.addr, visible, notVisible);
    }

    void Player::writeInvincibilityChanges(MenuEntry* entry) 
    {
        u8 invincible = 0x16;
        u8 notInvinci = 0x0;

        writePlayerChanges(8, invinciStatus, AddressList::IsInvincible.addr, invincible, notInvinci);
    }

    // lazy workaround for 24-bit write
    void Player::writeAnimStorageChanges(MenuEntry* entry) 
    {
        u16 animStoredA = 0x0;
        u16 notStoredA = 0xFFFF;

        u8 animStoredB = 0x0;
        u8 notStoredB = 0xFF;

        if (PlayerAnimation::getAnim(static_cast<u8>(GeneralHelpers::getCurrLink()), false) == PlayerAnimation::getIDFromName("Link_SwimWait")){
            writePlayerChanges(16, animStoreStatus, AddressList::IsWaterStorage.addr, animStoredA, notStoredA);
            writePlayerChanges(8, animStoreStatus, AddressList::IsWaterStorage.addr + 0x2, animStoredB, notStoredB);
        }
    }

    void Player::writeCollisionChanges(MenuEntry* entry) 
    {
        u8 collision = 0x10;
        u8 noCollision = 0xEA;

        writePlayerChanges(8, collisionStatus, AddressList::NoCollision.addr, collision, noCollision);
    }

    void Player::writePVPChanges(MenuEntry* entry) 
    {
        u8 pvp = 0x1;
        u8 notPvp = 0x0;

        writePlayerChanges(8, pvpStatus, AddressList::PVPMode.addr, pvp, notPvp);
    }

    void Player::writeSwordChanges(MenuEntry* entry) 
    {
        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; ++iterateThruPlayers) {
            u32 finalAddress = AddressList::SwordType.addr + (iterateThruPlayers * GameData::playerAddressOffset);

            if (swordType[iterateThruPlayers] != 0xFF) {
                Process::Write8(finalAddress, swordType[iterateThruPlayers]);
            }
        }
    }

    /*-------------------------*/

    void Player::setSwordChanges(MenuEntry* entry) 
    {
        if (entry->Name() == "Set custom sword model(s)") {
            openToggleMenu("Sword Model Type Menu", SWORD);
            entry->SetName("Disable custom sword model edits");
            swordEditAuto->Enable();
        }
        else {
            entry->SetName("Set custom sword model(s)");
            swordEditAuto->Disable();
        }
    }

    void Player::setSizeChanges(MenuEntry* entry) 
    {
        if (entry->Name() == "Set custom player model size") {
            openToggleMenu("Player Size Menu", PLAYER_SIZE);
            entry->SetName("Disable custom player size edits");

            if (PlayerSizes > 0.0)
                Process::WriteFloat(AddressList::PlayerModelSize.addr, PlayerSizes);
        }
        else {
            entry->SetName("Set custom player model size");
            Process::WriteFloat(AddressList::PlayerModelSize.addr, 1.0);
        }
    }

    void Player::setJinxChanges(MenuEntry* entry) 
    {
        std::string intro = "Jinx Model Status Menu\n\nEnable Jinx " + GeneralHelpers::enabledSlider << Color::White << "\nDisable Jinx " + GeneralHelpers::disabledSlider;
        if (entry->Name() == "Toggle jinxed model edits") {
            openToggleMenu(intro, NORMAL, &jinxStatus);
            entry->SetName("Disable jinxed model edits");
            jinxEditAuto->Enable();
        }
        else {
            entry->SetName("Toggle jinxed model edits");
            jinxEditAuto->Disable();
        }
    }

    void Player::setSpawnChanges(MenuEntry* entry) 
    {
        std::string intro = "Player Spawn Status Menu\n\nSpawned " + GeneralHelpers::enabledSlider << Color::White << "\nNot spawned " + GeneralHelpers::disabledSlider;
        if (entry->Name() == "Toggle player spawn edits") {
            openToggleMenu(intro, NORMAL, &spawnStatus);
            entry->SetName("Disable player spawn edits");
            spawnEditAuto->Enable();
        }
        else {
            entry->SetName("Toggle player spawn edits");
            spawnEditAuto->Disable();
        }
    }

    void Player::setVisibilityChanges(MenuEntry* entry) 
    {
        std::string intro = "Player Model Visibility Menu\n\nVisible " + GeneralHelpers::enabledSlider << Color::White << "\nNot visible " + GeneralHelpers::disabledSlider;
        if (entry->Name() == "Toggle player visibility edits") {
            openToggleMenu(intro, NORMAL, &visibleStatus);
            entry->SetName("Disable player visibility edits");
            visibleEditAuto->Enable();
        }
        else {
            entry->SetName("Toggle player visibility edits");
            visibleEditAuto->Disable();
        }
    }

    void Player::setInvincibilityChanges(MenuEntry* entry) 
    {
        std::string intro = "Player Invincibility Status Menu\n\nEnable invincibility " + GeneralHelpers::enabledSlider << Color::White << "\nDisable invincibility " + GeneralHelpers::disabledSlider;
        if (entry->Name() == "Toggle player invincibility edits") {
            openToggleMenu(intro, NORMAL, &invinciStatus);
            entry->SetName("Disable player invincibility edits");
            invinciEditAuto->Enable();
        }
        else {
            entry->SetName("Toggle player invincibility edits");
            invinciEditAuto->Disable();
        }
    }

    void Player::setAnimStorageChanges(MenuEntry* entry) 
    {
        std::string intro = "Animation Storage Status Menu\n\nEnable animation storage " + GeneralHelpers::enabledSlider << Color::White << "\nDisable animation storage " + GeneralHelpers::disabledSlider;
        if (entry->Name() == "Toggle animation storage edits") {
            openToggleMenu(intro, NORMAL, &animStoreStatus);
            entry->SetName("Disable animation storage edits");
            animStoreEditAuto->Enable();
        }
        else {
            entry->SetName("Toggle animation storage edits");
            animStoreEditAuto->Disable();
        }
    }

    void Player::setCollisionChanges(MenuEntry* entry) 
    {
        std::string intro = "Player Collision Status Menu\n\nDefault collision " + GeneralHelpers::enabledSlider << Color::White << "\nNo collision " + GeneralHelpers::disabledSlider;
        if (entry->Name() == "Toggle collision edits (experimental!)") {
            openToggleMenu(intro, NORMAL, &collisionStatus);
            entry->SetName("Disable collision edits");
            colEditAuto->Enable();
        }
        else {
            entry->SetName("Toggle collision edits (experimental!)");
            colEditAuto->Disable();
        }
    }

    void Player::setPVPChanges(MenuEntry* entry) 
    {
        std::string intro = "Player PvP Status Menu\n\nEnable PvP " + GeneralHelpers::enabledSlider << Color::White << "\nDisable PvP " + GeneralHelpers::disabledSlider;
        if (entry->Name() == "Toggle PvP damage edits") {
            openToggleMenu(intro, NORMAL, &pvpStatus);
            entry->SetName("Disable PvP damage edits");
            pvpEditAuto->Enable();
        }
        else {
            entry->SetName("Toggle PvP damage edits");
            pvpEditAuto->Disable();
        }
    }

    float setPlayerSize(void) 
    {
        float result;

        Keyboard sizeKB("Set player size:");
        sizeKB.IsHexadecimal(false);
        sizeKB.Open(result);

        if (result < 0.0)
            MessageBox(Color::Gainsboro << "Error", "Player Sizes cannot be negative.")();
        else
            return result;

        return 1.0;
    }

    /*-------------------------*/

    void showPosEditorIntroMsg(bool showMsg) 
    {
        if (showMsg) {
            OSD::Notify("[POSITION LOAD/SAVE] Use the touchscreen buttons to save/load");
            OSD::Notify("the current Link's position.");
        }
    }

    // TODO create new icons to replace these placeholders 
    FloatingButton loadBtn(IntRect(100, 210, 15, 15), Icon::DrawCentreOfGravity);
    FloatingButton saveBtn(IntRect(125, 210, 15, 15), Icon::DrawUnsplash);
    bool isPositionSaved[3] = { false, false, false };
    float positions[3][3];

    void Player::posEditor(MenuEntry* entry) 
    {
        showPosEditorIntroMsg(entry->WasJustActivated());
        resetPositionEditorSaves(entry->WasJustActivated());

        if (GeneralHelpers::isSinglePlayer() && !GeneralHelpers::isLoadingScreen() && Level::getCurrLevel() != Level::getIDFromName("Hytopia Castle")){
            loadBtn.Draw();
            saveBtn.Draw();

            if (!GeneralHelpers::isPauseScreen()) {
                loadBtn.Update(Touch::IsDown(), IntVector(Touch::GetPosition()));
                saveBtn.Update(Touch::IsDown(), IntVector(Touch::GetPosition()));
            }
        }

        resetPositionEditorSaves(GeneralHelpers::isLoadingScreen());

        int currLink = GeneralHelpers::getCurrLink();
        std::string color = GeneralHelpers::getLinkColorAsStr(currLink);

        if (loadBtn()) {
            if (isPositionSaved[currLink]) {
                loadPlayerPos(currLink);
                OSD::Notify("[POSITION LOAD/SAVE] Loaded " + color + "'s last saved position.");
            }
            else 
                OSD::Notify("[POSITION LOAD/SAVE] No saved position found for " + color + ".");
        }

        if (saveBtn()) {
            savePlayerPos(currLink);
            OSD::Notify("[POSITION LOAD/SAVE] Saved " + color + "'s current position.");
        }
    }

    void loadPlayerPos(int playerID) 
    {

        u32 offset = playerID * GameData::playerAddressOffset;

        Process::WriteFloat(AddressList::PositionX.addr + offset, positions[playerID][0]);
        Process::WriteFloat(AddressList::PositionY.addr + offset, positions[playerID][1]);
        Process::WriteFloat(AddressList::PositionZ.addr + offset, positions[playerID][2]);
    }

    void savePlayerPos(int playerID) 
    {
        u32 offset = playerID * GameData::playerAddressOffset;

        Process::ReadFloat(AddressList::PositionX.addr + offset, positions[playerID][0]);
        Process::ReadFloat(AddressList::PositionY.addr + offset, positions[playerID][1]);
        Process::ReadFloat(AddressList::PositionZ.addr + offset, positions[playerID][2]);
        
        isPositionSaved[playerID] = true;
    }

    void resetPositionEditorSaves(bool reset) 
    {
        if (reset)
            std::fill(isPositionSaved, isPositionSaved + 3, false);
    }

    /*-------------------------*/

    void Player::bypassDoppelDemo(MenuEntry* entry) 
    {
        if (Level::getCurrLevel() == Level::getIDFromName("Hytopia Castle") && Level::getCurrStage() == 0x2) {
            GeneralHelpers::forceEnableDoppels();

            u32 blueDataOffset = 0x10000;
            u32 redDataOffset = 0x20000;

            if (Level::hasStageBegan() && Level::getElapsedTime() < 0x5) {
                // set blue location to triforce
                Process::WriteFloat((AddressList::PositionX.addr + blueDataOffset), -0.8);
                Process::WriteFloat((AddressList::PositionY.addr + blueDataOffset), 1.1452);
                Process::WriteFloat((AddressList::PositionZ.addr + blueDataOffset), -9.95);

                // set red location to triforce
                Process::WriteFloat((AddressList::PositionX.addr + redDataOffset), 0.8);
                Process::WriteFloat((AddressList::PositionY.addr + redDataOffset), 1.1452);
                Process::WriteFloat((AddressList::PositionZ.addr + redDataOffset), -9.95);
            }
        }
    }

    // TODO: player face selector
}