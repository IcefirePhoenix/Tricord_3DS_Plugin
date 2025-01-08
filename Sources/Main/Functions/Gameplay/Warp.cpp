#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    MenuEntry *doppelEnableAuto;
    MenuEntry *reWarp;

    int warpData[3] = { -1, -1, -1 };
    bool firstWarp = true;

    /* ------------------ */

    // Helper function that selects the world/region for the next warp
    int warpGetLevel(int locCategory)
    {
        int worldID = -1;
        std::string chosenLevel = "";

        // warp room technically isn't considered part of DoT, so add it manually...
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

    // Helper function that returns the chosen location/world as a string
    std::string warpSelLevel(StringVector locNames)
    {
        Keyboard selLevel("Select a location:");
        selLevel.Populate(locNames);

        int result = selLevel.Open();
        if (result >= 0)
            return locNames[result];

        return "";
    }

    // Helper function that retrieves the possible list of stages based on the chosen area/level
    int warpGetStage(u8 levelID)
    {
        // we are given the levelID, this determines what stage menu list to display...
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
            return Level::selBasicStage(); // Drablands...
    }

    // Helper function that sets the current stage
    int warpSelStage(StringVector stageNames)
    {
        Keyboard selStage("Select an area:");
        selStage.Populate(stageNames);
        return selStage.Open() + 1; // adjust for 1-indexing instead of 0-indexing...
    }

    // Helper function that sets the current area's challenge, if applicable
    int warpSelChallenge(void)
    {
        Keyboard challenge("Choose a challenge:");
        challenge.Populate(Level::challengeList);
        return challenge.Open();
    }

    // Sets the current challenge index
    void Gameplay::writeChallengeEdit(MenuEntry *entry)
    {
        // this works since we wrote the new target chalID right before calling this function...
        Level::setCurrChal(Level::getCurrChallenge());

        // stop writing once stage has fully loaded...
        if (Level::hasStageBegan())
            entry->Disable();
    }

    // Driver code for warping to a desired area
    void Gameplay::instantWarp(MenuEntry *entry)
    {
        int targetLevel = -3, targetStage = -3, targetChallenge = 0, targetSpawn = 0;
        std::string firstWarpIntro = "This is your first warp. Make sure Doppels are wearing Hero's Tunics?";

        // Level::selWorld() returns world category ID (0-3) which DOES includes DoT and nonLevels in this case...
        targetLevel = warpGetLevel(Level::selWorld(true, true));

        // proceed to Stage selection if Level ID is valid...
        if (targetLevel >= 0x0)
            targetStage = warpGetStage((u8)targetLevel);
        else
            return;

        // proceed to Challenge selection if stage ID is valid and Level is in Drablands...
        if (targetStage >= 0x1)
        {
            if (Level::isInDrablands(targetLevel) && !Level::isInDoT(targetLevel))
            {
                targetChallenge = warpSelChallenge();
                if (targetChallenge == -1)
                    return;
            }

            // proceed with warp...
            Process::Write8(AddressList::TargetLevelID.addr, (u8)targetLevel);
            Process::Write8(AddressList::TargetStageID.addr, (u8)targetStage);
            // TODO: Process::Write8(AddressList::TargetSpawnID.addr, targetspawn);

            startWarp();

            // since single-lobby is bypassed, manually allow Doppel use...
            doppelEnableAuto->Enable();

            Level::setCurrChal((u8)targetChallenge);
            challengeEditAuto->Enable();

            // save this warp's data for future re-warps...
            warpData[0] = targetLevel;
            warpData[1] = targetStage;
            warpData[2] = targetChallenge;

            reWarp->SetName("Return to last warp: " + Level::levelNameFromID(warpData[0]) + " - " + std::to_string(warpData[1]));

            // optional: change default Doppel costumes...
            if (firstWarp)
            {
                if (GeneralHelpers::showMsgKbd(firstWarpIntro, DialogType::DialogYesNo))
                {
                    // set default Doppel costumes from Bear Minimum -> Hero's Tunic
                    Process::Write8(AddressList::CurrCostume.addr + PLAYER_OFFSET, GameData::getCostumeIDFromName("Hero's Tunic"));
                    Process::Write8(AddressList::CurrCostume.addr + (2 * PLAYER_OFFSET), GameData::getCostumeIDFromName("Hero's Tunic"));
                }
                firstWarp = false;
            }
        }
    }

    // Warps to the last area visited via warping
    void Gameplay::warpAgain(MenuEntry *entry)
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

    // "Resets" the current area by warping to the same area
    void Gameplay::resetRoom(MenuEntry *entry)
    {
        if (entry->Hotkeys[0].IsPressed())
        {
            Process::Write8(AddressList::TargetLevelID.addr, Level::getCurrLevel());
            Process::Write8(AddressList::TargetStageID.addr, Level::getCurrStage());
            startWarp();
        }
    }

    // Inits the warp sequence
    void startWarp(void)
    {
        u16 warpInit = 0x001E;
        Process::Write16(AddressList::WarpActivation.addr, warpInit);
        Process::Write32(AddressList::Warp.addr, AddressList::WarpPointer.addr);
    }

    // Warp between stages in the current Drablands level
    void Gameplay::stageWarp(MenuEntry *entry)
    {
        u8 targetLevel = Level::getCurrLevel();
        if (Level::isInDrablands())
        {
            u8 targetStage = warpGetStage(targetLevel);

            Process::Write8(AddressList::TargetLevelID.addr, targetLevel);
            Process::Write8(AddressList::TargetStageID.addr, targetStage);

            startWarp();
        }
        else
            MessageBox("Error", "Currently not in Drablands!")();
    }

    // Toggles whether Doppels should be automatically enabled following a warp
    void Gameplay::midWarpDoppelEnable(MenuEntry *entry)
    {
        if (GeneralHelpers::isLoadingScreen())
        {
            if (Level::getCurrLevel() >= Level::levelIDFromName("Coliseum"))
                GeneralHelpers::forceDoppelStatus(true);
            else
                GeneralHelpers::forceDoppelStatus(false);

            entry->Disable();
        }
    }
}