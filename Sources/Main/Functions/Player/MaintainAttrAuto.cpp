#include "Helpers.hpp"
#include "Main/Player.hpp"

namespace CTRPluginFramework
{
    MenuEntry *jinxEditAuto;
    MenuEntry *spawnEditAuto;
    MenuEntry *visibleEditAuto;
    MenuEntry *invinciEditAuto;
    MenuEntry *pvpEditAuto;
    MenuEntry *animStoreEditAuto;
    MenuEntry *colEditAuto;
    MenuEntry *swordEditAuto;
    MenuEntry *sizeEditAuto;

    /* The following are all wrapper functions + definitions for status-based values: */

    void Player::writeJinxChanges(MenuEntry *entry)
    {
        u32 jinxed = 0x00000000;
        u32 notJinxed = 0xFFFFFFFF;

        Player::writePlayerChanges(32, Player::jinxStatus, AddressList::getAddress("IsJinxed"), jinxed, notJinxed);
    }

    void Player::writeSpawnChanges(MenuEntry *entry)
    {
        u8 spawned = 0x1;
        u8 notSpawned = 0x0;

        Player::writePlayerChanges(8, Player::spawnStatus, AddressList::getAddress("IsSpawned"), spawned, notSpawned);
    }

    void Player::writeVisibilityChanges(MenuEntry *entry)
    {
        u8 visible = 0x1;
        u8 notVisible = 0x0;

        Player::writePlayerChanges(8, Player::visibleStatus, AddressList::getAddress("IsVisible"), visible, notVisible);
    }

    void Player::writeInvincibilityChanges(MenuEntry *entry)
    {
        u8 invincible = 0x16;
        u8 notInvinci = 0x0;

        Player::writePlayerChanges(8, Player::invinciStatus, AddressList::getAddress("IsInvincible"), invincible, notInvinci);
    }

    void Player::writeAnimStorageChanges(MenuEntry *entry)
    {
        u16 animStoredA = 0x0;
        u16 notStoredA = 0xFFFF;

        u8 animStoredB = 0x0;
        u8 notStoredB = 0xFF;

        if (PlayerAnimation::getAnim(static_cast<u8>(GeneralHelpers::getCurrLink()), false) == PlayerAnimation::getIDFromName("Link_SwimWait"))
        {
            // lazy workaround for 24-bit write...
            Player::writePlayerChanges(16, Player::animStoreStatus, AddressList::getAddress("IsWaterStorage"), animStoredA, notStoredA);
            Player::writePlayerChanges(8, Player::animStoreStatus, AddressList::getAddress("IsWaterStorage") + 0x2, animStoredB, notStoredB);
        }
    }

    void Player::writeCollisionChanges(MenuEntry *entry)
    {
        u8 collision = 0x10;
        u8 noCollision = 0xEA;

        Player::writePlayerChanges(8, Player::collisionStatus, AddressList::getAddress("NoCollision"), collision, noCollision);
    }

    void Player::writePVPChanges(MenuEntry *entry)
    {
        u8 pvp = 0x1;
        u8 notPvp = 0x0;

        Player::writePlayerChanges(8, Player::pvpStatus, AddressList::getAddress("PVPMode"), pvp, notPvp);
    }

    void Player::writeSwordChanges(MenuEntry *entry)
    {
        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
        {
            u32 finalAddress = AddressList::getAddress("SwordType") + (iterateThruPlayers * PLAYER_OFFSET);

            if (Player::swordType[iterateThruPlayers] != 0xFF)
                Process::Write8(finalAddress, Player::swordType[iterateThruPlayers]);
        }
    }
}