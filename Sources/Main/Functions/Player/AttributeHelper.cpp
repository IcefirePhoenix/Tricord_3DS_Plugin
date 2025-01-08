#include "Helpers.hpp"
#include "Main/Player.hpp"

namespace CTRPluginFramework
{
    /**
     * TODO: Edits made to player size ONLY affect primary sword size variable -> sword models revert back to
     * regular size temporarily during specific animations. Also, sword particle size is not affected in general.
     */

    u8 Player::jinxStatus = 0x0;
    u8 Player::spawnStatus = 0x0;
    u8 Player::visibleStatus = 0x0;
    u8 Player::invinciStatus = 0x0;
    u8 Player::animStoreStatus = 0x0;
    u8 Player::collisionStatus = 0x0;
    u8 Player::pvpStatus = 0x0;

    u8 Player::swordType[3] = {0xFF, 0xFF, 0xFF};

    float Player::PlayerSizes = 1.0;

    /* ------------------ */

    // Toggles status bit for individual players
    void togglePlayerStatus(u8 &playerStatus, ColorStatus color)
    {
        playerStatus ^= static_cast<u8>(color);
    }

    // Reads status bits for individual players
    bool checkPlayerStatus(u8 &playerStatus, int playerID)
    {
        if (playerStatus & (1 << playerID))
            return true;
        return false;
    }

    // Generic function that applies status edits
    template <typename T>
    void Player::writePlayerChanges(int editLength, u8& playerStatus, u32 address, T trueValue, T falseValue)
    {
        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
        {
            T valueToWrite = checkPlayerStatus(playerStatus, iterateThruPlayers) ? trueValue : falseValue;
            u32 finalAddress = address + (iterateThruPlayers * PLAYER_OFFSET);

            switch (editLength)
            {
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
    template void Player::writePlayerChanges<u8>(int editLength, u8 &playerStatus, u32 address, u8 trueValue, u8 falseValue);
    template void Player::writePlayerChanges<u16>(int editLength, u8 &playerStatus, u32 address, u16 trueValue, u16 falseValue);
    template void Player::writePlayerChanges<u32>(int editLength, u8 &playerStatus, u32 address, u32 trueValue, u32 falseValue);
    template void Player::writePlayerChanges<float>(int editLength, u8 &playerStatus, u32 address, float trueValue, float falseValue);

    // Depending on user choice, call appropriate methods to update statuses OR other player data
    void initEdit(EditType type, int playerID, ColorStatus color, u8 &playerStatus)
    {
        int swordChoice = -1;

        switch (type)
        {
            case NORMAL:
                togglePlayerStatus(playerStatus, color);
                break;
            case SWORD:
                swordChoice = GeneralHelpers::chooseSword();
                if (swordChoice >= 0)
                    Player::swordType[playerID] = swordChoice;
                break;
            case PLAYER_SIZE:
                Player::PlayerSizes = setPlayerSize();
                break;
        }
    }

    // Menu interface for toggling statuses
    void openToggleMenu(std::string entryTitle, EditType type, u8 *playerStatus)
    {
        Keyboard menu("Menu");
        StringVector bottomScreenOptions;

        bool isMenuOpen = true;
        menu.CanAbort(false);

        while (isMenuOpen)
        {
            u8 status = 0x0;

            if (playerStatus != nullptr)
                status = *playerStatus;

            bottomScreenOptions.clear(); // clear bottom screen per frame...

            // populate menu options differently depending on category...
            switch (type)
            {
                case NORMAL:
                    bottomScreenOptions.push_back(std::string("Player 1 ") << (checkPlayerStatus(status, 0) ? ENABLED_SLIDER : DISABLED_SLIDER));
                    bottomScreenOptions.push_back(std::string("Player 2 ") << (checkPlayerStatus(status, 1) ? ENABLED_SLIDER : DISABLED_SLIDER));
                    bottomScreenOptions.push_back(std::string("Player 3 ") << (checkPlayerStatus(status, 2) ? ENABLED_SLIDER : DISABLED_SLIDER));
                    break;
                case SWORD:
                    bottomScreenOptions.push_back(std::string("Player 1: ") << GameData::getSwordNameFromID(Player::swordType[0]));
                    bottomScreenOptions.push_back(std::string("Player 2: ") << GameData::getSwordNameFromID(Player::swordType[1]));
                    bottomScreenOptions.push_back(std::string("Player 3: ") << GameData::getSwordNameFromID(Player::swordType[2]));
                    break;
                case PLAYER_SIZE:
                    bottomScreenOptions.push_back(std::string("Player Size: ") << std::to_string(Player::PlayerSizes));
                    break;
            }

            // regardless of category, last option should always be this:
            bottomScreenOptions.push_back("Save and exit");

            // update top screen with real-time current statuses...
            menu.GetMessage() = entryTitle;
            menu.Populate(bottomScreenOptions);

            switch (menu.Open())
            {
                // player 1
                case 0:
                    initEdit(type, 0, greenEnabled, *playerStatus);
                    break;

                // player 2 OR "Save and Exit"
                case 1:
                    if (type == PLAYER_SIZE)
                    {
                        isMenuOpen = false;
                        break;
                    }
                    initEdit(type, 1, blueEnabled, *playerStatus);
                    break;

                // player 3
                case 2:
                    initEdit(type, 2, redEnabled, *playerStatus);
                    break;

                // "Save and Exit"
                case 3:
                    isMenuOpen = false;
                    break;
            }
        }
    }
}