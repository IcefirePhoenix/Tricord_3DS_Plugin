#include "Helpers.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    void Gameplay::infEnergy(MenuEntry* entry) 
    {
        // TODO: simple 32-bit (8-bit??) write to infinite energy address and repeat for all Links
        // Note: verify spreadsheet data...
    }

    void Gameplay::maxEnergySet(MenuEntry* entry)
    {
        // TODO: simple 8-bit write to max energy address and repeat for all Links
        // Note: verify spreadsheet data...
    }

    void Gameplay::energyConsumeMultiplier(MenuEntry* entry)
    {
        // TODO: simple 32-bit write to energy consumption address and repeat for all Links
    }

    // checkbox
    void Gameplay::infHealth(MenuEntry* entry)
    {
        Process::Write8(AddressList::HealthCurrent.addr, 0x24);
    }
    
    // hotkey entry
    void Gameplay::noHealth(MenuEntry* entry)
    {
        if (entry->Hotkeys[0].IsPressed()) 
            Process::Write8(AddressList::HealthCurrent.addr, 0x0);
    }

    // checkbox
    void Gameplay::infFairy(MenuEntry* entry)
    {
        // do find the correct pointer
    }

    void manageEnemy(bool keepAlive)
    {
        bool stopLoop = false;

        u8 defeatedStatus = 0x8; // 0xB not defeated?
        u8 enemyStatusOffset = 0x10;
        u32 enemyHealthOffset = 0x598;
        u32 healthEdit = keepAlive ? 0x7FFFFFFF : 0x0;

        u32 enemyDataStartAddress, enemyDataNextAddress, enemyCurrStatus;

        Process::Read32(AddressList::EnemyDataPointer.addr, enemyDataStartAddress); // get start of enemy data
        
        if (!GeneralHelpers::isNullPointer(enemyDataStartAddress))
            Process::Read32(enemyDataStartAddress, enemyDataNextAddress);           // get next enemy data block

        // this is how we know a full iteration has occurred -> stop the loop
        while (enemyDataNextAddress != (enemyDataNextAddress || AddressList::EnemyDataPointer.addr))            
        {
            if (!GeneralHelpers::isNullPointer(enemyDataNextAddress))
            {
                Process::Read32(enemyDataNextAddress + enemyStatusOffset, enemyCurrStatus);
                if (enemyCurrStatus != defeatedStatus) // there needs to be additional checks here -> whether the data blocks are enemy related 
                    Process::Write32(enemyDataNextAddress + enemyHealthOffset, healthEdit);

                // update with new address
                Process::Read32(enemyDataNextAddress, enemyDataNextAddress);
            }
        }
    }

    void Gameplay::autoKillEnemy(MenuEntry* entry)
    {	
        if (entry->Hotkeys[0].IsPressed()       && 
            Level::isInDrablands()              && 
            GeneralHelpers::isSinglePlayer()    && 
            !GeneralHelpers::isPauseScreen()    && 
            !GeneralHelpers::isLoadingScreen())

            manageEnemy(false);
    }

    void Gameplay::enemyInvinci(MenuEntry* entry)
    {
        if (entry->Hotkeys[0].IsPressed()       && 
            Level::isInDrablands()              &&  
            GeneralHelpers::isSinglePlayer()    && 
            !GeneralHelpers::isPauseScreen()    && 
            !GeneralHelpers::isLoadingScreen())

            manageEnemy(true);
    }

    // checkbox
    void Gameplay::controlAllPlayers(MenuEntry* entry)
    {
        Process::Write8(AddressList::ActiveLink.addr, 0x3);
    }

    // or maybe changePhysics (if other similar changes are discovered)
    void Gameplay::icePhysicsEverywhere(MenuEntry* entry)
    {
        
    }

    void Gameplay::infTIme(MenuEntry* entry)
    {

    }


}