#include "Helpers.hpp"
#include "Main/Player.hpp"

namespace CTRPluginFramework
{
    /**
     * TODO: Edits made to player size ONLY affect primary sword size variable -> sword models revert back to
     * regular size temporarily during specific animations. Also, sword particle size is not affected in general.
     */

    u8 Player::jinxStatus = PlayerMask::PLAYER_NONE;
    u8 Player::spawnStatus = PlayerMask::PLAYER_NONE;
    u8 Player::visibleStatus = PlayerMask::PLAYER_NONE;
    u8 Player::invinciStatus = PlayerMask::PLAYER_NONE;
    u8 Player::animStoreStatus = PlayerMask::PLAYER_NONE;
    u8 Player::collisionStatus = PlayerMask::PLAYER_NONE;
    u8 Player::pvpStatus = PlayerMask::PLAYER_NONE;

    float Player::PlayerSizes = 1.0;

    /* ------------------ */

    // Reads status bits for individual players
    bool checkPlayerStatus(u8& playerStatus, int playerID)
    {
        if (playerStatus & (BIT(playerID)))
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
    template void Player::writePlayerChanges<u8>(int editLength, u8& playerStatus, u32 address, u8 trueValue, u8 falseValue);
    template void Player::writePlayerChanges<u16>(int editLength, u8& playerStatus, u32 address, u16 trueValue, u16 falseValue);
    template void Player::writePlayerChanges<u32>(int editLength, u8& playerStatus, u32 address, u32 trueValue, u32 falseValue);
    template void Player::writePlayerChanges<float>(int editLength, u8& playerStatus, u32 address, float trueValue, float falseValue);
}