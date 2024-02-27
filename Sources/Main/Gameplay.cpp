#include "Helpers.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    MenuEntry* physicsEditAutoG;
    MenuEntry* physicsEditAutoB;
    MenuEntry* physicsEditAutoR;
    MenuEntry* moonJumpEntry;
    MenuEntry* flightEntry;
    MenuEntry* doppelEnableAuto;
    MenuEntry* challengeEditAuto;
    MenuEntry* controlAllAuto;
    u16 physicsStatus[3];
    float ascentSpeed = 0.5, descentSpeed = -0.5, lateralSpeed = 0.09;
    u8 targetlevel = 1; u8 chalID;
    bool controlAuto = false;

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
                entry->SetName("Physics - Player 1 (Green): None");
                physicsEditAutoG->Disable();
            } else {
                entry->SetName("Physics - Player 1 (Green): "+result);
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
                entry->SetName("Physics - Player 2 (Blue): None");
                physicsEditAutoB->Disable();
            } else {
                entry->SetName("Physics - Player 2 (Blue): "+result);
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
                entry->SetName("Physics - Player 3 (Red): None");
                physicsEditAutoR->Disable();
            } else {
                entry->SetName("Physics - Player 3 (Red): "+result);
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
        u16 towrite = physicsStatus[Link];
        u32 offset = Link*GameData::playerAddressOffset;
        // if Link is sinking in quicksand, do not freeze collision (otherwise you sink endlessly if quicksand is active)
        u8 sinkingval;
        Process::Read8(AddressList::CostumeAttrD.addr + offset, sinkingval);
        bool sinking = (sinkingval & 0x80) == 0x80;
        // if Link is airborne (coll 0x1F or 0xA), do not freeze collision
        // (ensures fall zones work properly, and collision doesn't apply while carried in a totem)
        // if touching water or lava, do not freeze collision (ensures water/lava acts normally)
        u16 currentcoll;
        Process::Read16(AddressList::CollisionCurrent.addr + offset, currentcoll);
        bool collCheck = currentcoll != 0x0187 && currentcoll != 0x0167 && currentcoll != 0x1F && currentcoll != 0xA;
        if (!sinking && collCheck){
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

    // gearbox
    void Gameplay::controlAllPlayers(MenuEntry* entry)
    {
        if (entry->Name() == "Control all players"){
            controlAuto = true;
            controlAllAuto->Enable();
            entry->SetName("Reset control to Player 1 (Green)");
        } else {
            controlAuto = false;
            controlAllAuto->Disable();
            Process::Write8(AddressList::ActiveLink.addr, 0x0);
            Process::Write8(AddressList::CameraLinkFocus.addr, 0x0);
            entry->SetName("Control all players");
        }
    }

    void Gameplay::writePlayerControl(MenuEntry* entry)
    {
        if (controlAuto){
            Process::Write8(AddressList::ActiveLink.addr, 0x3);
        }
    }

    void Gameplay::infTime(MenuEntry* entry)
    {
        // 0xEA61 --> 1000:0(1)
        // If the rightmost seconds digit is visible, it always flickers whenever the game tries to tick the timer down
        // However, we can avoid this flicker by pushing this digit offscreen, by setting the minutes to have 4 digits
        Process::Write16(AddressList::TimeLeft.addr, 0xEA61);
    }

    void Gameplay::instantWarp(MenuEntry* entry)
    {
        u8 targetstage = 1, targetspawn = 0;
        bool challenge = false; // EXPERIMENTAL - CHALLENGE CHOICE

        Keyboard Location("Choose a location:");
        static const StringVector locationList =
        {
            "Hytopia",
            "Coliseum",
            "Drablands",
            "Den of Trials"
        };
        static StringVector levelList;

        Location.Populate(locationList, true);

        switch(Location.Open()){
            case 0:
            {
                // Hytopia Village, Shops, Castle
                Keyboard HytopiaLevel("Choose a level in Hytopia:");
                static const StringVector hytopiaLevels = GameData::hytopiaLevelList;
                HytopiaLevel.Populate(hytopiaLevels, true);
                int result = HytopiaLevel.Open();
                if (result < 0) return;
                targetlevel = Level::getIDFromName(hytopiaLevels[result]);
                switch(result){
                    case 0:
                        // Hytopia Village
                        targetstage = 1;
                        break;
                    case 1:
                    {
                        // Hytopia Shops
                        Keyboard HytopiaShopStage("Choose a stage in Hytopia Shops:");
                        HytopiaShopStage.Populate(GameData::hytopiaShopsStageList, true);
                        targetstage = HytopiaShopStage.Open() + 1;
                        break;
                    }
                    case 2:
                    {
                        // Hytopia Castle
                        Keyboard HytopiaCastleStage("Choose a stage in Hytopia Castle:");
                        HytopiaCastleStage.Populate(GameData::hytopiaCastleStageList, true);
                        targetstage = HytopiaCastleStage.Open() + 1;
                        break;
                    }
                }
                break;
            }
            case 1:
            {
                // Coliseum stages
                targetlevel = Level::getIDFromName("Coliseum");
                Keyboard ColiseumStage("Choose a stage in the Coliseum:");
                ColiseumStage.Populate(GameData::worldList, true);
                targetstage = ColiseumStage.Open() + 1;
                break;
            }
            case 2:
            {
                // Drablands locations
                int world = GameData::selWorld(false);
                if (world < 0) return;
                Keyboard Level("Choose a level:");
                levelList = GameData::getWorldNamesfromID(world);
                Level.Populate(levelList, true);
                int level = Level.Open();
                if (level < 0) return;
                targetlevel = Level::getIDFromName(levelList[level]);

                // EXPERIMENTAL - CHALLENGE CHOICE
                Keyboard Challenge("Choose a challenge:");
                static const StringVector chalList = GameData::challengeList;
                Challenge.Populate(chalList, true);
                chalID = Challenge.Open();
                if (chalID < 0) return;
                else if (chalID > 0) challenge = true;

                targetstage = chooseDrablandsStage();
                break;
            }
            case 3:
            {
                // Den of Trials lobby and zones
                Keyboard DoT("Choose an area in the Den of Trials:");
                static const StringVector dotList = {
                    "Den of Trials Lobby",
                    "Forest Zone",
                    "Flooded Zone",
                    "Scorching Zone",
                    "Frozen Zone",
                    "Fortified Zone",
                    "Desert Zone",
                    "Shadow Zone",
                    "Baneful Zone"
                };
                DoT.Populate(dotList, true);
                int result = DoT.Open();
                if (result < 0) return;
                else if (result == 0){
                    targetlevel = Level::getIDFromName("Den of Trials");
                    targetstage = 1;
                } else {
                    targetlevel = Level::getIDFromName(dotList[result]);
                    targetstage = chooseDrablandsStage();
                }
                break;
            }
            default:
                return;
        }
        // Check stage selection (the last step) was successful, if not, abort
        if (targetstage < 1){
            return;
        }

        // Write target level, stage and spawn IDs
        // Write pointer to warp address
        Process::Write8(AddressList::TargetLevelID.addr, targetlevel);
        Process::Write8(AddressList::TargetStageID.addr, targetstage);
        // TODO: Process::Write8(AddressList::TargetSpawnID.addr, targetspawn);
        Process::Write32(AddressList::Warp.addr, AddressList::WarpPointer.addr);

        // If a challenge is chosen, write that challenge
        if (challenge){
            challengeEditAuto->Enable();
        }

        // Mid-warp Doppel enable
        doppelEnableAuto->Enable();
    }

    void Gameplay::stageWarp(MenuEntry* entry)
    {
        // Can only be used if in a Drablands level
        u8 currentlevel = Level::getCurrLevel();
        if ((currentlevel >= Level::getIDFromName("Deku Forest") && currentlevel <= Level::getIDFromName("Sky Temple")) || 
            (currentlevel >= Level::getIDFromName("Forest Zone") && currentlevel <= Level::getIDFromName("Baneful Zone"))){
            // Keyboard
            u8 targetstage = chooseDrablandsStage();
            Process::Write8(AddressList::TargetLevelID.addr, currentlevel);
            Process::Write8(AddressList::TargetStageID.addr, targetstage);
            Process::Write32(AddressList::Warp.addr, AddressList::WarpPointer.addr);
        }
    }

    int chooseDrablandsStage()
    {
        Keyboard Stage("Choose a stage:");
        static const StringVector stages =
        {
            "Stage 1",
            "Stage 2",
            "Stage 3",
            "Stage 4",
            "Treasure Room"
        };
        Stage.Populate(stages, true);
        return Stage.Open() + 1;
    }

    void Gameplay::midWarpDoppelEnable(MenuEntry* entry)
    {
        u8 loading;
        Process::Read8(AddressList::LoadingStatus.addr, loading);
        if (loading){
            if (targetlevel >= 4){
                Process::Write8(AddressList::DoppelsEnabled.addr, 0x1);
            } else {
                Process::Write8(AddressList::DoppelsEnabled.addr, 0x0);
            }
            entry->Disable();
        }
    }

    void Gameplay::writeChallengeEdit(MenuEntry* entry)
    {
        Process::Write8(AddressList::ChallengeID.addr, chalID);
        if (Level::hasStageBegan()){
            entry->Disable();
        }
    }
}