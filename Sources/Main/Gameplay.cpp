#include "Helpers.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    MenuEntry* physicsEditAutoG;
    MenuEntry* physicsEditAutoB;
    MenuEntry* physicsEditAutoR;
    MenuEntry* moonJumpEntry;
    MenuEntry* flightEntry;
    u16 physicsStatus[3];
    float ascentSpeed = 0.5, descentSpeed = -0.5, lateralSpeed = 0.09;

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

    // gearbox
    void Gameplay::changePhysicsG(MenuEntry* entry)
    {
        std::string result = physicsSelectMenu(0);
        if (result != ""){
            if (result == "None"){
                entry->SetName("Player 1 (Green): None");
                physicsEditAutoG->Disable();
            } else {
                entry->SetName("Player 1 (Green): "+result);
                physicsEditAutoG->Enable();
            }
        }
    }

    // gearbox
    void Gameplay::changePhysicsB(MenuEntry* entry)
    {
        std::string result = physicsSelectMenu(1);
        if (result != ""){
            if (result == "None"){
                entry->SetName("Player 2 (Blue): None");
                physicsEditAutoB->Disable();
            } else {
                entry->SetName("Player 2 (Blue): "+result);
                physicsEditAutoB->Enable();
            }
        }
    }

    // gearbox
    void Gameplay::changePhysicsR(MenuEntry* entry)
    {
        std::string result = physicsSelectMenu(2);
        if (result != ""){
            if (result == "None"){
                entry->SetName("Player 3 (Red): None");
                physicsEditAutoR->Disable();
            } else {
                entry->SetName("Player 3 (Red): "+result);
                physicsEditAutoR->Enable();
            }
        }
    }

    std::string physicsSelectMenu(int Link)
    {
        Keyboard Physics("Choose a type of physics:");
        static const StringVector physicsList =
        {
            "Reset",
            "Water",
            "Lava",
            "Ice",
            "Quicksand"
        };

        Physics.Populate(physicsList);

        switch(Physics.Open()){
            case 0:
                return "None";
            case 1:
                physicsStatus[Link] = 0x0187;
                return "Water";
            case 2:
                physicsStatus[Link] = 0x0167;
                return "Lava";
            case 3:
                physicsStatus[Link] = 0x0121;
                return "Ice";
            case 4:
                physicsStatus[Link] = 0x00C1;
                return "Quicksand";
            default:
                return "";
        }
    }

    void writePhysicsChanges(int Link)
    {
        // if Link is airborne (anim 02), do not freeze collision (ensures fall zones work properly)
        // if Link is sinking in quicksand, do not freeze collision (otherwise you sink endlessly if quicksand is active)
        // if Link is in a totem, do not freeze collision (otherwise you sink immediately when thrown if quicksand is active)
        // if touching water or lava, do not freeze collision (prevents you from being able to walk on water/lava)
        u16 towrite = physicsStatus[Link];
        u32 offset = Link*GameData::playerAddressOffset;
        u8 sinkingval;
        Process::Read8(AddressList::CostumeAttrD.addr + offset, sinkingval);
        bool sinking = (sinkingval & 0x80) == 0x80;
        u8 totemval;
        Process::Read8(AddressList::TotemCarriedBy.addr + offset, totemval);
        bool notCarried = totemval == 0x0;
        u16 currentcoll;
        Process::Read16(AddressList::CollisionCurrent.addr + offset, currentcoll);
        bool swimCheck = currentcoll != 0x0187 && currentcoll != 0x0167;
        u8 currentanim = PlayerAnimation::getAnim(Link, false);
        if (currentanim != 0x2 && !sinking && notCarried && swimCheck){
            Process::Write16(AddressList::CollisionCurrent.addr + offset, towrite);
        }
    }

    void Gameplay::physicsChangeG(MenuEntry* entry)
    {
        writePhysicsChanges(0);
    }

    void Gameplay::physicsChangeB(MenuEntry* entry)
    {
        writePhysicsChanges(1);
    }

    void Gameplay::physicsChangeR(MenuEntry* entry)
    {
        writePhysicsChanges(2);
    }

    // Moon Jump - Auto descent
    void Gameplay::moonJump(MenuEntry* entry)
    {
        if (flightEntry->WasJustActivated()){
            flightEntry->Disable();
        }
        int Link = GeneralHelpers::getCurrLink();
        u32 offset = Link*GameData::playerAddressOffset;
        u32 addrX = AddressList::SpeedX.addr + offset;
        u32 addrY = AddressList::SpeedY.addr + offset;
        u32 addrZ = AddressList::SpeedZ.addr + offset;
        // Hotkeys: North, South, East, West, Ascend
        // Note: South and East are positive, North and West are negative
        if (entry->Hotkeys[0].IsDown()){
            Process::WriteFloat(addrZ, lateralSpeed*-1);
        } else if (entry->Hotkeys[1].IsDown()){
            Process::WriteFloat(addrZ, lateralSpeed);
        } else {
            Process::WriteFloat(addrZ, 0);
        }
        if (entry->Hotkeys[2].IsDown()){
            Process::WriteFloat(addrX, lateralSpeed);
        } else if (entry->Hotkeys[3].IsDown()){
            Process::WriteFloat(addrX, lateralSpeed*-1);
        } else {
            Process::WriteFloat(addrX, 0);
        }
        if (entry->Hotkeys[4].IsDown()){
            Process::WriteFloat(addrY, ascentSpeed);
        } else {
            Process::WriteFloat(addrY, descentSpeed);
        }
    }

    // Flight - All manual
    void Gameplay::flight(MenuEntry* entry)
    {
        if (moonJumpEntry->WasJustActivated()){
            moonJumpEntry->Disable();
        }
        int Link = GeneralHelpers::getCurrLink();
        u32 offset = Link*GameData::playerAddressOffset;
        u32 addrX = AddressList::SpeedX.addr + offset;
        u32 addrY = AddressList::SpeedY.addr + offset;
        u32 addrZ = AddressList::SpeedZ.addr + offset;
        // Hotkeys: North, South, East, West, Ascend, Descend
        // Note: South and East are positive, North and West are negative
        if (entry->Hotkeys[0].IsDown()){
            Process::WriteFloat(addrZ, lateralSpeed*-1);
        } else if (entry->Hotkeys[1].IsDown()){
            Process::WriteFloat(addrZ, lateralSpeed);
        } else {
            Process::WriteFloat(addrZ, 0);
        }
        if (entry->Hotkeys[2].IsDown()){
            Process::WriteFloat(addrX, lateralSpeed);
        } else if (entry->Hotkeys[3].IsDown()){
            Process::WriteFloat(addrX, lateralSpeed*-1);
        } else {
            Process::WriteFloat(addrX, 0);
        }
        if (entry->Hotkeys[4].IsDown()){
            Process::WriteFloat(addrY, ascentSpeed);
        } else if (entry->Hotkeys[5].IsDown()){
            Process::WriteFloat(addrY, descentSpeed);
        } else {
            Process::WriteFloat(addrY, 0.025); // Results in Link maintaining Y position
        }
    }

    // Adjust ascension speed
    void Gameplay::adjustAscentSpeed(MenuEntry* entry)
    {
        Keyboard Speed("Choose a speed power:");
        static const StringVector speedOptions =
        {
            "Weak",
            "Medium",
            "Strong"
        };

        Speed.Populate(speedOptions);

        switch(Speed.Open()){
            case 0:
                ascentSpeed = 0.2;
                entry->SetName("Adjust ascent speed: Weak");
                break;
            case 1:
                ascentSpeed = 0.5;
                entry->SetName("Adjust ascent speed: Medium");
                break;
            case 2:
                ascentSpeed = 0.7;
                entry->SetName("Adjust ascent speed: Strong");
                break;
        }
    }

    // Adjust descension speed
    void Gameplay::adjustDescentSpeed(MenuEntry* entry)
    {
        Keyboard Speed("Choose a speed power:");
        static const StringVector speedOptions =
        {
            "Weak",
            "Medium",
            "Strong"
        };

        Speed.Populate(speedOptions);

        switch(Speed.Open()){
            case 0:
                descentSpeed = -0.2;
                entry->SetName("Adjust descent speed: Weak");
                break;
            case 1:
                descentSpeed = -0.5;
                entry->SetName("Adjust descent speed: Medium");
                break;
            case 2:
                descentSpeed = -0.7;
                entry->SetName("Adjust descent speed: Strong");
                break;
        }
    }

    // Adjust lateral movement speed
    void Gameplay::adjustLateralSpeed(MenuEntry* entry)
    {
        Keyboard Speed("Choose a speed power:");
        static const StringVector speedOptions =
        {
            "Weak",
            "Medium",
            "Strong"
        };

        Speed.Populate(speedOptions);

        switch(Speed.Open()){
            case 0:
                lateralSpeed = 0.04;
                entry->SetName("Adjust lateral speed: Weak");
                break;
            case 1:
                lateralSpeed = 0.09;
                entry->SetName("Adjust lateral speed: Medium");
                break;
            case 2:
                lateralSpeed = 0.2;
                entry->SetName("Adjust lateral speed: Strong");
                break;
        }
    }

    // checkbox
    void Gameplay::controlAllPlayers(MenuEntry* entry)
    {
        Process::Write8(AddressList::ActiveLink.addr, 0x3);
    }

    void Gameplay::infTime(MenuEntry* entry)
    {
        // 0xEA61 --> 1000:0(1)
        // If the rightmost seconds digit is visible, it always flickers whenever the game tries to tick the timer down
        // However, we can avoid this flicker by pushing this digit offscreen, by setting the minutes to have 4 digits
        Process::Write16(AddressList::TimeLeft.addr, 0xEA61);
    }


}