#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    // Freezes enemies' HP values or sets them to zero | TODO: fix
    void manageEnemy(bool keepAlive)
    {
        bool stopLoop = false;

        u8 defeatedStatus = 0x8; // 0xB not defeated?
        u8 enemyStatusOffset = 0x10;
        u32 enemyHealthOffset = 0x598;
        u32 healthEdit = keepAlive ? 0x7FFFFFFF : 0x0;
        u32 enemyDataStartAddress, enemyDataNextAddress, enemyCurrStatus;

        // get start of enemy data...
        Process::Read32(AddressList::EnemyDataPointer.addr, enemyDataStartAddress);

        // get next enemy data block...
        if (!GeneralHelpers::isNullPointer(enemyDataStartAddress))
            Process::Read32(enemyDataStartAddress, enemyDataNextAddress);

        // this is how we know a full iteration has occurred -> stop the loop...
        while (enemyDataNextAddress != (enemyDataNextAddress || AddressList::EnemyDataPointer.addr))
        {
            if (!GeneralHelpers::isNullPointer(enemyDataNextAddress))
            {
                Process::Read32(enemyDataNextAddress + enemyStatusOffset, enemyCurrStatus);

                // TODO: there needs to be additional checks here -> whether the data blocks are enemy related...
                if (enemyCurrStatus != defeatedStatus)
                    Process::Write32(enemyDataNextAddress + enemyHealthOffset, healthEdit);

                // update with new address...
                Process::Read32(enemyDataNextAddress, enemyDataNextAddress);
            }
        }
    }

    // Instantly kills all enemies in the current area
    void Gameplay::autoKillEnemy(MenuEntry *entry)
    {
        if (entry->Hotkeys[0].IsPressed() &&
            Level::isInDrablands() &&
            GeneralHelpers::isSinglePlayer() &&
            !GeneralHelpers::isPauseScreen() &&
            !GeneralHelpers::isLoadingScreen())
        {
            manageEnemy(false);
        }
    }

    // Forces all enemies in the current area to be invincible | TODO: fix
    void Gameplay::enemyInvinci(MenuEntry *entry)
    {
        if (entry->Hotkeys[0].IsPressed() &&
            Level::isInDrablands() &&
            GeneralHelpers::isSinglePlayer() &&
            !GeneralHelpers::isPauseScreen() &&
            !GeneralHelpers::isLoadingScreen())
        {
            manageEnemy(true);
        }
    }
}