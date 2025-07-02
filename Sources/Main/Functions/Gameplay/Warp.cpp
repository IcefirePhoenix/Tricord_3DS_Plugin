// #include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

#define MAX_BOOKMARKS 3

namespace CTRPluginFramework
{
    MenuEntry *doppelEnableAuto;
    MenuEntry *reWarp;
    MenuEntry *bookmark1;
    MenuEntry *bookmark2;
    MenuEntry *bookmark3;

    Preferences::WarpDestination bookmarkedWarps[3];
    MenuEntry *bookmarkEntries[3];

    bool firstWarp = true;
    int warpData[3] = {-1, -1, -1};

    /* ------------------ */

    // Helper function - restores stored bookmarks from settings file
    void Gameplay::restoreBookmarks(void)
    {
        // lazy
        bookmarkEntries[0] = bookmark1;
        bookmarkEntries[1] = bookmark2;
        bookmarkEntries[2] = bookmark3;

        std::copy(std::begin(Preferences::SavedWarps), std::end(Preferences::SavedWarps), std::begin(bookmarkedWarps));
        for (int i = 0; i < MAX_BOOKMARKS; i++)
        {
            int levelID = bookmarkedWarps[i].levelID;
            int stageID = bookmarkedWarps[i].stageID;

            if (levelID != 0 && stageID != 0)
            {
                bookmarkEntries[i]->SetName("Go to: " + retrieveNonStageName(levelID, stageID, Level::getLevelByID(levelID)->first));
                bookmarkEntries[i]->CanBeSelected(true);
            }
            else
                bookmarkEntries[i]->CanBeSelected(false);
        }
    }

    // Resets the bookmark list
    void Gameplay::resetBookmarks(MenuEntry *entry)
    {
        for (int i = 0; i < MAX_BOOKMARKS; i++)
        {
            bookmarkedWarps[i].levelID = 0;
            bookmarkedWarps[i].stageID = 0;

            bookmarkEntries[i]->CanBeSelected(false);
            bookmarkEntries[i]->SetName("(Warp bookmark not set)");
        }
        std::copy(std::begin(bookmarkedWarps), std::end(bookmarkedWarps), std::begin(Preferences::SavedWarps));
        MessageBox("Success", "Bookmarks have been reset!")();
    }

    // Adds current location to bookmark list
    void Gameplay::setBookmark(MenuEntry *entry)
    {
        int selection = -1;
        StringVector slotOptions =
        {
            "Slot 1",
            "Slot 2",
            "Slot 3"
        };

        Keyboard chooseSlot("Bookmark Current Location", "Select a slot to overwrite.");
        chooseSlot.Populate(slotOptions);
        selection = chooseSlot.Open();

        if (selection >= 0)
        {
            bookmarkedWarps[selection].levelID = Level::getCurrLevel();
            bookmarkedWarps[selection].stageID = Level::getCurrStage();
            std::copy(std::begin(bookmarkedWarps), std::end(bookmarkedWarps), std::begin(Preferences::SavedWarps));

            std::string levelName = Level::getLevelByID(Level::getCurrLevel())->first;
            bookmarkEntries[selection]->SetName("Go to: " + retrieveNonStageName(bookmarkedWarps[selection].levelID, bookmarkedWarps[selection].stageID, levelName));
            bookmarkEntries[selection]->CanBeSelected(true);
        }
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
        int targetCategory = -3, targetLevelID = -3, targetWorld = -3, targetStage = -3, targetChallenge = 0;
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

        startWarp();

        Level::setCurrChal(targetChallenge);
        challengeEditAuto->Enable();

        // save this warp's data for future re-warps...
        warpData[0] = targetLevelID;
        warpData[1] = targetStage;
        warpData[2] = targetChallenge;

        reWarp->SetName("Return to last warp: " + retrieveNonStageName(targetLevelID, targetStage, levelName));

        // optional: change default Doppel costumes...
        if (firstWarp)
        {
            if (GeneralHelpers::showMsgKbd("Doppel Costume Confirmation", firstWarpIntro, DialogType::DialogYesNo))
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
        int challenge = 0;
        if (warpData[0] != -1 && warpData[1] != -1)
        {
            Process::Write8(AddressList::getAddress("TargetLevelID"), warpData[0]);
            Process::Write8(AddressList::getAddress("TargetStageID"), warpData[1]);

            if (Level::isInDrablands(warpData[0]) && !Level::isInDoT(true, warpData[0]))
            {
                const Level level = Level::getLevelByID(warpData[0])->second;
                if (warpData[0] != Level::levelIDFromName("DoT Warp Room"))
                    challenge = level.selChallenge(level.getChallenges());

                if (challenge < 0)
                    return;

                startWarp();
            }
        }
        else
            MessageBox("Error", "Previous warp data not set!")();
    }

    // Returns the player to a previously bookmarked location
    void Gameplay::returnToBookmark(MenuEntry* entry)
    {
        int bookmarkID = reinterpret_cast<int>(entry->GetArg());
        int challenge = 0;
        int levelID = bookmarkedWarps[bookmarkID].levelID;
        int stageID = bookmarkedWarps[bookmarkID].stageID;

        if (levelID != 0 && stageID != 0)
        {
            Process::Write8(AddressList::getAddress("TargetLevelID"), levelID);
            Process::Write8(AddressList::getAddress("TargetStageID"), stageID);

            if (Level::isInDrablands(levelID) && !Level::isInDoT(true, levelID))
            {
                const Level level = Level::getLevelByID(levelID)->second;
                if (levelID != Level::levelIDFromName("DoT Warp Room"))
                    challenge = level.selChallenge(level.getChallenges());

                if (challenge < 0)
                    return;
            }
            startWarp();

            // save this warp's data for future re-warps...
            warpData[0] = levelID;
            warpData[1] = stageID;
            warpData[2] = challenge;

            // update rewarp; isolate location name from this entry...
            int pos = entry->Name().find(':');
            if (pos != std::string::npos && pos + 2 <= entry->Name().length())
                reWarp->SetName("Return to last warp: " + entry->Name().substr(pos + 2));
        }
        else
            MessageBox("Error", "Bookmark data invalid!")();
    }

    // Get the exact name of locations that do not use traditional stage-based system OR returns normal name if not applicable
    std::string Gameplay::retrieveNonStageName(int levelID, int stageID, std::string originalName)
    {
        // note: cannot use Level::levelIDFromName for switch as its result is evaluated at runtime, not compile-time...
        switch (levelID)
        {
            case 0x1: // hytopia castle
                return Level::hytopiaCastleStageList[stageID - 1];
            case 0x2: // hytopia
                return originalName;
            case 0x3: // hytopia shops
                return Level::hytopiaShopsStageList[stageID - 1];
            case 0x4: // coliseum
                return Level::arenaList[stageID - 1];
            case 0x59: // DoT warp hub
                return originalName;
            default:
                if (stageID == 5)
                    return originalName + " - Treasure Room";
                else
                    return originalName + " - " + std::to_string(stageID);
        }
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

        // If single player lobby is bypassed, manually allow Doppel use
        doppelEnableAuto->Enable();
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