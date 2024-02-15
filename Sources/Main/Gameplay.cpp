#include "Helpers.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    MenuEntry* physicsEditAuto;
    u16 physicsStatus;

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
        // Still uncertain if the second address (32D81EE4) is relevant
        Process::Write8(AddressList::FairiesCurrent.addr, 0xA);
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

    // gearbox
    void Gameplay::changePhysics(MenuEntry* entry)
    {
        if (entry->Name() == "Change ground physics") {
            Keyboard Physics("Choose a type of physics:");
            static const StringVector physicsList =
            {
                "Water",
                "Lava",
                "Ice",
                "Quicksand"
            };

            Physics.Populate(physicsList);

            switch(Physics.Open()){
                case 0:
                    physicsStatus = 0x0187;
                    break;
                case 1:
                    physicsStatus = 0x0167;
                    break;
                case 2:
                    physicsStatus = 0x0121;
                    break;
                case 3:
                    physicsStatus = 0x00C1;
                    break;
                default:
                    return;
            }

            entry->SetName("Disable custom ground physics edits");
            physicsEditAuto->Enable();
        }
        else {
            entry->SetName("Change ground physics");
            physicsEditAuto->Disable();
        }
    }

    void Gameplay::writePhysicsChanges(MenuEntry* entry)
    {
        // if Link is airborne (anim 02), do not freeze collision (ensures fall zones work properly)
        // if Link is sinking in quicksand, do not freeze collision (otherwise you sink endlessly)
        int Link;
        u32 targetaddr;
        u32 sinkingaddr;
        for (Link = 0, targetaddr = AddressList::CollisionCurrent.addr, sinkingaddr = AddressList::CostumeAttrD.addr;
            Link < 3; Link++, targetaddr += GameData::playerAddressOffset, sinkingaddr += GameData::playerAddressOffset){
            u8 sinkingval;
            Process::Read8(sinkingaddr, sinkingval);
            bool sinking = (sinkingval & 0x80) == 0x80;
            if (PlayerAnimation::getAnim(Link, false) != 0x2 && !sinking){
                Process::Write16(targetaddr, physicsStatus);
            }
        }
    }

    void Gameplay::infTime(MenuEntry* entry)
    {
        // 0xEA61 --> 1000:0(1)
        // If the rightmost seconds digit is visible, it always flickers whenever the game tries to tick the timer down
        // However, we can avoid this flicker by pushing this digit offscreen, by setting the minutes to have 4 digits
        Process::Write16(AddressList::TimeLeft.addr, 0xEA61);
    }


}