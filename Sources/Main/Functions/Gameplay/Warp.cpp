#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    MenuEntry *doppelEnableAuto;
    MenuEntry *reWarp;

    int warpData[3] = { -1, -1, -1 };
    bool firstWarp = true;

    /* ------------------ */

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
        int targetCategory = -3, targetLevelID = -3, targetWorld = -3, targetStage = -3, targetChallenge = 0, targetSpawn = 0;
        std::string firstWarpIntro = "This is your first warp. Make sure Doppels are wearing Hero's Tunics?";
        std::string levelName = "";
        std::pair<std::string, Level> targetLevelData = {};

    selectCategory:
        targetCategory = Level::selCategory();

        // user abort
        if (targetCategory == -1)
            return;

    selectWorld:
        switch (targetCategory)
        {
            case 0:
                targetWorld = static_cast<int>(Level::World::Hytopia);
                break;
            case 1:
                targetWorld = static_cast<int>(Level::World::Coliseum);
                break;
            case 2:
                targetWorld = Level::selDrablandsWorld(false);
                break;
            case 3:
                targetWorld = static_cast<int>(Level::World::DoT);
                break;
            default: // sys sleep abort
                return;
        }

        if (targetWorld < 0)
            goto selectCategory;

    selectLevel:
        targetLevelData = Level::selLevel(targetWorld);

        if (targetLevelData.first.empty())
        {
            if (targetWorld != 2)
                goto selectCategory;
            else
                goto selectWorld;
        }

    selectStage:
        targetLevelID = targetLevelData.second.getLevelID();
        levelName = targetLevelData.first;
        targetStage = Level::selStage(targetLevelID);

        // stages are not 0-indexed...
        if (targetStage < 1)
        {
            if (targetLevelID == Level::levelIDFromName("Coliseum"))
                goto selectCategory;
            else
                goto selectLevel;
        }

    selectChallenge:
        if (Level::isInDrablands(targetLevelID) && !Level::isInDoT(true, targetLevelID))
        {
            targetChallenge = Level::selChallenge(targetLevelData.second.getChallenges());
            if (targetChallenge < 0)
                goto selectStage;
        }

        // proceed with warp...
        Process::Write8(AddressList::getAddress("TargetLevelID"), targetLevelID);
        Process::Write8(AddressList::getAddress("TargetStageID"), targetStage);
        // TODO: Process::Write8(AddressList::getAddress("TargetSpawnID"), targetspawn);

        startWarp();

        // since single-lobby is bypassed, manually allow Doppel use...
        doppelEnableAuto->Enable();

        Level::setCurrChal(targetChallenge);
        challengeEditAuto->Enable();

        // save this warp's data for future re-warps...
        warpData[0] = targetLevelID;
        warpData[1] = targetStage;
        warpData[2] = targetChallenge;

        reWarp->SetName("Return to last warp: " + levelName + " - " + std::to_string(warpData[1]));

        // optional: change default Doppel costumes...
        if (firstWarp)
        {
            if (GeneralHelpers::showMsgKbd(firstWarpIntro, DialogType::DialogYesNo))
            {
                // set default Doppel costumes from Bear Minimum -> Hero's Tunic
                Costume::setPlayerCostume(GameData::getPlayerIDFromColor("Blue"), GameData::getCostumeIDFromName("Hero's Tunic"));
                Costume::setPlayerCostume(GameData::getPlayerIDFromColor("Red"), GameData::getCostumeIDFromName("Hero's Tunic"));
            }
            firstWarp = false;
        }
    }

    // Warps to the last area visited via warping
    void Gameplay::warpAgain(MenuEntry *entry)
    {
        if (warpData[0] != -1 && warpData[1] != -1)
        {
            Process::Write8(AddressList::getAddress("TargetLevelID"), warpData[0]);
            Process::Write8(AddressList::getAddress("TargetStageID"), warpData[1]);

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
            Process::Write8(AddressList::getAddress("TargetLevelID"), Level::getCurrLevel());
            Process::Write8(AddressList::getAddress("TargetStageID"), Level::getCurrStage());
            startWarp();
        }
    }

    // Inits the warp sequence
    void startWarp(void)
    {
        u16 warpInit = 0x001E;
        Process::Write16(AddressList::getAddress("WarpActivation"), warpInit);
        Process::Write32(AddressList::getAddress("Warp"), AddressList::getAddress("WarpPointer"));
    }

    // Warp between stages in the current Drablands level
    void Gameplay::stageWarp(MenuEntry *entry)
    {
        u8 targetLevel = Level::getCurrLevel();
        if (Level::isInDrablands())
        {
            u8 targetStage = Level::selStage(targetLevel);

            Process::Write8(AddressList::getAddress("TargetLevelID"), targetLevel);
            Process::Write8(AddressList::getAddress("TargetStageID"), targetStage);

            startWarp();
        }
        else
            MessageBox("Error", "Currently not in Drablands!")();
    }

    // Toggles whether Doppels should be automatically enabled following a warp
    void Gameplay::midWarpDoppelEnable(MenuEntry *entry)
    {
        if (GeneralHelpers::isLoadingScreen(false))
        {
            if (Level::getCurrLevel() >= Level::levelIDFromName("Coliseum"))
                GeneralHelpers::forceDoppelStatus(true);
            else
                GeneralHelpers::forceDoppelStatus(false);

            entry->Disable();
        }
    }
}