#include "Helpers.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    MenuEntry* physicsSelG;
    MenuEntry* physicsSelB;
    MenuEntry* physicsSelR;
    MenuEntry* physicsEditAutoG;
    MenuEntry* physicsEditAutoB;
    MenuEntry* physicsEditAutoR;
    MenuEntry* moonJumpEntry;
    MenuEntry* flightEntry;
    MenuEntry* reWarp;
    MenuEntry* doppelEnableAuto;
    MenuEntry* challengeEditAuto;
    MenuEntry* controlAllAuto;

    u16 physicsStatus[3];
    int warpData[3] = { -1, -1, -1 };
    float ascentSpeed = 0.5, descentSpeed = -0.5, lateralSpeed = 0.09;
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
    void Gameplay::changePhysics(MenuEntry* entry)
    {
        MenuEntry* physicsEntries[3] = {
            physicsEditAutoG,
            physicsEditAutoB,
            physicsEditAutoR
        };

        int player = reinterpret_cast<int>(entry->GetArg());
        std::string baseEntryName, result = physicsSelectMenu(player);

        // get the entry's base name -- everything before the colon
        std::string longName = entry->Name();
        std::size_t colonPosition = longName.find(':');
        if (colonPosition != std::string::npos)
        {
            baseEntryName = longName.substr(0, colonPosition);
        }

        if (!result.empty())
        {
            if (result == "Not edited") { physicsEntries[player]->Disable(); }
            else { physicsEntries[player]->Enable(); }

            entry->SetName(baseEntryName + ": " + result);
        }
    }

    std::string physicsSelectMenu(int player)
    {
        Keyboard physics("Choose a type of physics:");
        static const StringVector physicsList =
        {
            "Reset",
            "Water",
            "Lava",
            "Ice",
            "Quicksand"
        };

        physics.Populate(physicsList);

        switch (physics.Open())
        {
            case 0:
                return "Not edited";
            case 1:
                physicsStatus[player] = Collision::colIDFromName("Water");
                return "Water";
            case 2:
                physicsStatus[player] = Collision::colIDFromName("Lava");
                return "Lava";
            case 3:
                physicsStatus[player] = Collision::colIDFromName("Ice");
                return "Ice";
            case 4:
                physicsStatus[player] = Collision::colIDFromName("Quicksand_plane");
                return "Quicksand";
            default:
                return "";
        }
    }

    // Note: do not freeze collision if:
    // 1) Link is airborne -- ensures fall zones works properly
    // 2) if Link is swimming -- ensures water/lava acts properly
    // 3) if Link is sinking in quicksand -- prevents infinite sinking if quicksand is active
    void Gameplay::writePhysicsChanges(MenuEntry* entry)
    {
        int player = reinterpret_cast<int>(entry->GetArg());

        u8 sinkingStatus;
        u16 currentCol, targetCol = physicsStatus[player];
        u32 addressOffset = player * GameData::playerAddressOffset;

        Process::Read8(AddressList::CostumeAttrD.addr + addressOffset, sinkingStatus);
        bool isSinking = (sinkingStatus & 0x80) == 0x80;

        currentCol = Collision::getCurrCol(player);
        bool checkValidColWrite =
            currentCol != Collision::colIDFromName("Water") &&
            currentCol != Collision::colIDFromName("Lava") &&
            currentCol != Collision::colIDFromName("Air") &&
            currentCol != Collision::colIDFromName("Fall_plane");

        if (!isSinking && checkValidColWrite)
        {
            Collision::setCurrCol(player, targetCol);
        }
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

    int warpGetLevel(int locCategory)
    {
        int worldID = -1;
        std::string chosenLevel = "";

        StringVector DoTWithLobby = Level::dotZoneList;
        DoTWithLobby.insert(DoTWithLobby.begin(), "DoT Warp Room");

        switch (locCategory)
        {
            case 0: // choose from village, shops, castle
                chosenLevel = warpSelLevel(Level::getWorldNamesfromID(0, true));
                break;
            case 1: // coliseum
                chosenLevel = "Coliseum";
                break;
            case 2: // choose world level
                worldID = Level::selWorld(false, false);
                if (worldID >= 0)
                    chosenLevel = warpSelLevel(Level::getWorldNamesfromID(worldID, false));
                break;
            case 3: // choose DoT level
                chosenLevel = warpSelLevel(DoTWithLobby);
                break;
            default:
                break;
        }

        if (!chosenLevel.empty())
            return Level::levelIDFromName(chosenLevel);
        return -1;
    }

    std::string warpSelLevel(StringVector locNames)
    {
        Keyboard selLevel("Select a location:");
        selLevel.Populate(locNames);
        int result = selLevel.Open();

        if (result >= 0)
            return locNames[result];
        return "";
    }

    int warpGetStage(u8 levelID)
    {
        // we are given the levelID, this determines what stage menu to display
        if (levelID == Level::levelIDFromName("Hytopia Castle"))
            return warpSelStage(Level::hytopiaCastleStageList);
        else if (levelID == Level::levelIDFromName("Hytopia"))
            return 1;
        else if (levelID == Level::levelIDFromName("Hytopia Shops"))
            return warpSelStage(Level::hytopiaShopsStageList);
        else if (levelID == Level::levelIDFromName("Coliseum"))
            return warpSelStage(Level::arenaList);
        else if (levelID == Level::levelIDFromName("DoT Warp Room"))
            return 1;
        else
            return Level::selBasicStage();
    }

    int warpSelStage(StringVector stageNames)
    {
        Keyboard selStage("Select an area:");
        selStage.Populate(stageNames);
        return selStage.Open() + 1; // adjust for 1-indexing instead of 0-indexing
    }

    int warpSelChallenge(void)
    {
        Keyboard challenge("Choose a challenge:");
        challenge.Populate(Level::challengeList);
        return challenge.Open();
    }

    void Gameplay::instantWarp(MenuEntry* entry)
    {
        int targetLevel = -3, targetStage = -3, targetChallenge = -3, targetSpawn = 0;
        targetLevel = warpGetLevel(Level::selWorld(true, true)); // Level::selWorld() returns 0-3 given the params (use DoT and nonLevels)

        if (targetLevel >= 0x0)
            targetStage = warpGetStage((u8) targetLevel);
        else
            return;

        if (targetStage >= 0x1)
        {
            if (Level::isInDrablands(targetLevel) && !Level::isInDoT(targetLevel))
            {
                targetChallenge = warpSelChallenge();
                if (targetChallenge == -1)
                return;
            }

            // proceed with warp
            Process::Write8(AddressList::TargetLevelID.addr, (u8)targetLevel);
            Process::Write8(AddressList::TargetStageID.addr, (u8)targetStage);
            // TODO: Process::Write8(AddressList::TargetSpawnID.addr, targetspawn);

            startWarp();

            doppelEnableAuto->Enable();

            if (targetChallenge >= 0x1)
            {
                Level::setCurrChal((u8)targetChallenge);
                challengeEditAuto->Enable();
            }

            warpData[0] = targetLevel;
            warpData[1] = targetStage;
            warpData[2] = targetChallenge;

            reWarp->SetName("Return to last warp: " + Level::levelNameFromID(warpData[0]) + " - " + std::to_string(warpData[1]));
        }
    }

    void Gameplay::warpAgain(MenuEntry* entry)
    {
        if (warpData[0] != -1 && warpData[1] != -1)
        {
            Process::Write8(AddressList::TargetLevelID.addr, warpData[0]);
            Process::Write8(AddressList::TargetStageID.addr, warpData[1]);

            startWarp();
        }
        else
            MessageBox("Error", "Previous warp data not set!")();
    }

    void Gameplay::resetRoom(MenuEntry *entry)
    {
        Process::Write8(AddressList::TargetLevelID.addr, Level::getCurrLevel());
        Process::Write8(AddressList::TargetStageID.addr, Level::getCurrStage());
        startWarp();
    }

    void startWarp(void)
    {
        Process::Write16(AddressList::WarpActivation.addr, 0x001E);
        Process::Write32(AddressList::Warp.addr, AddressList::WarpPointer.addr);
    }

    void Gameplay::stageWarp(MenuEntry* entry)
    {
        u8 targetLevel = Level::getCurrLevel();
        if (Level::isInDrablands()){
            u8 targetStage = warpGetStage(targetLevel);

            Process::Write8(AddressList::TargetLevelID.addr, targetLevel);
            Process::Write8(AddressList::TargetStageID.addr, targetStage);

            startWarp();
        }
        else
            MessageBox("Error", "Currently not in Drablands!")();
    }

    void Gameplay::midWarpDoppelEnable(MenuEntry* entry)
    {
        if (GeneralHelpers::isLoadingScreen()){
            if (Level::getCurrLevel() >= Level::levelIDFromName("Coliseum"))
                GeneralHelpers::forceDoppelStatus(true);
            else
                GeneralHelpers::forceDoppelStatus(false);

            entry->Disable();
        }
    }

    void Gameplay::writeChallengeEdit(MenuEntry* entry)
    {
        // this works since we wrote the new target chalID right before calling this function
        Level::setCurrChal(Level::getCurrChallenge());

        if (Level::hasStageBegan()){
            entry->Disable();
        }
    }
}