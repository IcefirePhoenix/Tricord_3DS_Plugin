#include "Helpers.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    void Gameplay::infEnergy(MenuEntry* entry) 
    {
        // Write 900 since this is the max for the big energy gauge
        Process::WriteFloat(AddressList::EnergyCurrent.addr, 900);
    }

    void Gameplay::maxEnergySet(MenuEntry* entry)
    {
        if (entry->Name() == "Set maximum energy") {
            float newMaxEnergy;
            Keyboard editMaxEnergy("Maximum energy\n\nDefault value for normal gauge is 600\nDefault value for large gauge is 900");
            editMaxEnergy.IsHexadecimal(false);
            if (editMaxEnergy.Open(newMaxEnergy) == 0){
                entry->SetName("Disable custom maximum energy edits");
                Process::WriteFloat(AddressList::EnergyMax.addr, newMaxEnergy);
                Process::WriteFloat(AddressList::EnergyMaxPitRecovery.addr, newMaxEnergy);
            }
        }
        else {
            entry->SetName("Set maximum energy");
            Process::WriteFloat(AddressList::EnergyMax.addr, 600);
            Process::WriteFloat(AddressList::EnergyMaxPitRecovery.addr, 600);
        }
    }

    void Gameplay::energyConsumeMultiplier(MenuEntry* entry)
    {
        if (entry->Name() == "Set energy consumption multiplier") {
            float newMultiplier = 1;
            Keyboard editEnergyConsumeMultiplier("Energy consumption multiplier");
            editEnergyConsumeMultiplier.IsHexadecimal(false);
            if (editEnergyConsumeMultiplier.Open(newMultiplier) == 0 && newMultiplier > 0){
                entry->SetName("Disable custom energy consumption edits");
                Process::WriteFloat(AddressList::EnergyMaxMultiplicand.addr, 600*newMultiplier);
            }
        }
        else {
            entry->SetName("Set energy consumption multiplier");
            Process::WriteFloat(AddressList::EnergyMaxMultiplicand.addr, 600);
        }
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