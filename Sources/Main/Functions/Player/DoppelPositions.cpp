#include "Helpers.hpp"
#include "Main/Player.hpp"

namespace CTRPluginFramework
{
    // Brings Doppels to active player location -> recreates behavior found in the debug build
    void Player::doppelsToPlayer(MenuEntry *entry)
    {
        float currX, currY, currZ;
        u8 activePlayer = 0x0;
        u32 offset = 0x0;

        u32 xAddr = AddressList::getAddress("PositionX");
        u32 yAddr = AddressList::getAddress("PositionY");
        u32 zAddr = AddressList::getAddress("PositionZ");

        if (entry->Hotkeys[0].IsPressed())
        {
            // read active player's position data...
            activePlayer = GeneralHelpers::getCurrLink();
            offset = activePlayer * PLAYER_OFFSET;

            Process::ReadFloat(xAddr + offset, currX);
            Process::ReadFloat(yAddr + offset, currY);
            Process::ReadFloat(zAddr + offset, currZ);

            // place Doppels at active player's location...
            for (int playerToMove = 0; playerToMove < 3; playerToMove++)
            {
                if (playerToMove != activePlayer)
                {
                    Process::WriteFloat(xAddr, currX);
                    Process::WriteFloat(yAddr, currY);
                    Process::WriteFloat(zAddr, currZ);
                }

                xAddr += PLAYER_OFFSET;
                yAddr += PLAYER_OFFSET;
                zAddr += PLAYER_OFFSET;
            }
        }
    }

    // Automatically activates Doppels and places them on the Triforce portal in the single-player lobby
    void Player::bypassDoppelDemo(MenuEntry *entry)
    {
        if (Level::getCurrLevel() == Level::levelIDFromName("Hytopia Castle") && Level::getCurrStage() == 0x2)
        {
            GeneralHelpers::forceDoppelStatus(true);

            u32 blueDataOffset = PLAYER_OFFSET;
            u32 redDataOffset = PLAYER_OFFSET * 2;

            // set locations immediately post-level load...
            if (Level::hasStageBegan() && Level::getElapsedTime() < 0x5)
            {
                // set blue location to triforce...
                Process::WriteFloat((AddressList::getAddress("PositionX") + blueDataOffset), -0.8);
                Process::WriteFloat((AddressList::getAddress("PositionY") + blueDataOffset), 1.1452);
                Process::WriteFloat((AddressList::getAddress("PositionZ") + blueDataOffset), -9.95);

                // set red location to triforce...
                Process::WriteFloat((AddressList::getAddress("PositionX") + redDataOffset), 0.8);
                Process::WriteFloat((AddressList::getAddress("PositionY") + redDataOffset), 1.1452);
                Process::WriteFloat((AddressList::getAddress("PositionZ") + redDataOffset), -9.95);
            }
        }
    }
}