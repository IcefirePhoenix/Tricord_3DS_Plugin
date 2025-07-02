#include "Helpers.hpp"

namespace CTRPluginFramework
{
    Level::Level(){}

	Level::Level(u8 levelID, bool dummyStatus, int world, std::array<u8, 3> challenges) :
		_ID(levelID), _isDummy(dummyStatus), _world(world), _challenges(challenges)
	{
	}

    const std::map<std::string, Level> Level::nonLevelMap =
    {
        {"Hytopia", Level(0x2, false, 9)},
        {"Hytopia Shops", Level(0x3, false, 9)},
        {"Hytopia Castle", Level(0x1, false, 9)},
        {"Coliseum", Level(0x4, false, 10)}
    };

    const std::map<std::string, Level> Level::levelMap =
    {
        {"Deku Forest", Level(0xA, false, 0, {0xD, 0xE, 0x4})},
        {"Buzz Blob Cave", Level(0xB, false, 0, {0x6, 0xE, 0xF})},
        {"Moblin Base", Level(0xC, false, 0, {0x6, 0xC, 0x3})},
        {"Forest Temple", Level(0xD, false, 0, {0x8, 0xE, 0xD})},
        {"Secret Fortress", Level(0x14, false, 1, {0x6, 0x12, 0x15})},
        {"Abyss of Agony", Level(0x15, false, 1, {0x16, 0x4, 0xE})},
        {"Cove of Transition", Level(0x16, false, 1, {0x16, 0x15, 0xE})},
        {"Water Temple", Level(0x17, false, 1, {0xD, 0x12, 0x3})},
        {"Blazing Trail", Level(0x1E, false, 2, {0x6, 0x15, 0x3})},
        {"Hinox Mine", Level(0x1F, false, 2, {0xE, 0x6, 0x7})},
        {"Den of Flames", Level(0x20, false, 2, {0xD, 0x13, 0xE})},
        {"Fire Temple", Level(0x21, false, 2, {0xE, 0x15, 0xA})},
        {"Frozen Plateau", Level(0x28, false, 3, {0x17, 0x6, 0x4})},
        {"Snowball Ravine", Level(0x29, false, 3, {0x8, 0xD, 0x3})},
        {"Silver Shrine", Level(0x2A, false, 3, {0x9, 0x16, 0x15})},
        {"Ice Temple", Level(0x2B, false, 3, {0xD, 0x17, 0x3})},
        {"Sealed Gateway", Level(0x32, false, 4, {0x2, 0x15, 0x16})},
        {"Bomb Storage", Level(0x33, false, 4, {0x9, 0x3, 0x11})},
        {"Training Ground", Level(0x34, false, 4, {0xE, 0x16, 0x2})},
        {"The Lady's Lair", Level(0x35, false, 4, {0x15, 0xD, 0xA})},
        {"Infinity Dunes", Level(0x3C, false, 5, {0x8, 0x4, 0xD})},
        {"Stone Corridors", Level(0x3D, false, 5, {0xE, 0x15, 0x11})},
        {"Gibdo Mausoleum", Level(0x3E, false, 5, {0xE, 0x14, 0xA})},
        {"Desert Temple", Level(0x3F, false, 5, {0xD, 0x7, 0xE})},
        {"Illusory Mansion", Level(0x46, false, 6, {0xD, 0x11, 0xE})},
        {"Palace Noir", Level(0x47, false, 6, {0x6, 0x16, 0x3})},
        {"Lone Labyrinth", Level(0x48, false, 6, {0x2, 0x4, 0x8})},
        {"Grim Temple", Level(0x49, false, 6, {0xD, 0x11, 0xE})},
        {"Floating Garden", Level(0x50, false, 7, {0x9, 0x15, 0x11})},
        {"Deception Castle", Level(0x51, false, 7, {0xE, 0x11, 0x7})},
        {"Dragon Citadel", Level(0x52, false, 7, {0x13, 0x16, 0x11})},
        {"Sky Temple", Level(0x53, false, 7, {0xE, 0x10, 0xD})}
    };

    const std::map<std::string, Level> Level::DoTLevelMap =
    {
        {"DoT Warp Room", Level(0x59, false, 8)},
        {"Forest Zone", Level(0x5A, false, 8)},
        {"Flooded Zone", Level(0x5B, false, 8)},
        {"Scorching Zone", Level(0x5C, false, 8)},
        {"Frozen Zone", Level(0x5D, false, 8)},
        {"Fortified Zone", Level(0x5E, false, 8)},
        {"Desert Zone", Level(0x5F, false, 8)},
        {"Shadow Zone", Level(0x60, false, 8)},
        {"Baneful Zone", Level(0x61, false, 8)}
    };

    const StringVector Level::hytopiaShopsStageList =
    {
        "Miiverse Gallery",
        "Madame Couture's",
        "Daily Riches"
    };

    const StringVector Level::hytopiaCastleStageList =
    {
        "Entrance",
        "Single Player Lobby",
        "Multiplayer Lobby",
        "Throne Room",
        "Coliseum Hallway",
        "Coliseum Lobby"
    };

    const StringVector Level::worldList =
    {
        "Woodlands",
        "Riverside",
        "Volcano",
        "Ice Cavern",
        "Fortress",
        "The Dunes",
        "The Ruins",
        "Sky Realm"
    };

    const StringVector Level::arenaList =
    {
        "Woodlands Arena",
        "Riverside Arena",
        "Volcano Arena",
        "Ice Cavern Arena",
        "Fortress Arena",
        "Dunes Arena",
        "Ruins Arena",
        "Sky Realm Arena"
    };

    // Combines all Level data into a single map
    std::map<std::string, Level> Level::getAllLevels(void)
    {
        std::map<std::string, Level> allLevels;

        allLevels.insert(nonLevelMap.begin(), nonLevelMap.end());
        allLevels.insert(levelMap.begin(), levelMap.end());
        allLevels.insert(DoTLevelMap.begin(), DoTLevelMap.end());

        return allLevels;
    }

    // Returns level names from a specified region/world
    StringVector Level::getLevelNamesFromWorld(int worldID) // TODO: check eberything that calls this for new index
    {
        switch (worldID)
        {
            case 0:
                return Level::buildLevelNameList(Level::levelMap, World::Woodlands);
            case 1:
                return Level::buildLevelNameList(Level::levelMap, World::Riverside);
            case 2:
                return Level::buildLevelNameList(Level::levelMap, World::Volcano);
            case 3:
                return Level::buildLevelNameList(Level::levelMap, World::Ice);
            case 4:
                return Level::buildLevelNameList(Level::levelMap, World::Fortress);
            case 5:
                return Level::buildLevelNameList(Level::levelMap, World::Dunes);
            case 6:
                return Level::buildLevelNameList(Level::levelMap, World::Ruins);
            case 7:
                return Level::buildLevelNameList(Level::levelMap, World::Sky);
            case 8:
                return Level::buildLevelNameList(Level::DoTLevelMap, World::DoT);
            case 9:
                return Level::buildLevelNameList(Level::nonLevelMap, World::Hytopia);
            case 10:
            default:
                return StringVector();
        }
    }

    // Returns a vector containing the level names from a specified world's map
    StringVector Level::buildLevelNameList(const std::map<std::string, Level> &levelMap, World world)
    {
        StringVector levelNames;
        std::vector<std::pair<std::string, Level>> levelEntry;

        // entry.first = key (string) | entry.second = value (Level)
        for (const auto& entry : levelMap)
        {
            if (entry.second.getWorld() == static_cast<int>(world))
                levelEntry.push_back(entry);
        }

        // maps are auto-sorted by key, not value -> manually sort by levelID (or name if Hytopia)
        if (world == World::Hytopia)
        {
            std::sort(levelEntry.begin(), levelEntry.end(), [](const std::pair<std::string, Level> &firstLevel, const std::pair<std::string, Level> &secondLevel)
            {
                return firstLevel.first < secondLevel.first;
            });
        }
        else
        {
            std::sort(levelEntry.begin(), levelEntry.end(), [](const std::pair<std::string, Level>& firstLevel, const std::pair<std::string, Level>& secondLevel)
            {
                return firstLevel.second.getLevelID() < secondLevel.second.getLevelID();
            });
        }

        for (const auto &level : levelEntry)
            levelNames.push_back(level.first);

        return levelNames;
    }

    // Helper function that returns a location category
    int Level::selCategory(void)
    {
        StringVector locationCategories =
        {
            "Hytopia",
            "Coliseum",
            "Levels",
            "Den of Trials"
        };

        Keyboard chooseCategory("Category Selection", "Select a location category.");
        chooseCategory.Populate(locationCategories);

        return chooseCategory.Open();
    }

    // Helper function that returns Drabland world selection
    int Level::selDrablandsWorld(bool useDoT)
    {
        StringVector worldSelectionList = Level::worldList;

        if (useDoT)
            worldSelectionList.push_back("Den of Trials");

        Keyboard chooseWorld("World Selection", "Select a world.");
        chooseWorld.Populate(worldSelectionList);

        return chooseWorld.Open();
    }


    // Helper function that returns level from world
    std::pair<std::string, Level> Level::selLevel(int world)
    {
        int index;
        std::string chosenLevel = "";
        StringVector levelChoices = Level::getLevelNamesFromWorld(world);

        if (world == 10)
            return std::make_pair("Coliseum", getAllLevels().at("Coliseum"));

        // choose level via name...
        Keyboard chooseLevel("Level Selection", "Select a level.");
        chooseLevel.Populate(levelChoices);

        index = chooseLevel.Open();

        if (index >= 0)
        {
            chosenLevel = levelChoices[index];
            return std::make_pair(chosenLevel, getAllLevels().at(chosenLevel));
        }
        return {};
    }

    // Helper function that returns stage ID selection
	int Level::selStage(u8 levelID)
	{
        StringVector availableStages;
        StringVector basicStages =
        {
			"Stage 1",
			"Stage 2",
			"Stage 3",
			"Stage 4",
			"Treasure Room"
		};

        // handle no-stage levels first...
        if (levelID == Level::levelIDFromName("Hytopia") || levelID == Level::levelIDFromName("DoT Warp Room"))
            return 1;

        // custom stage lists...
        switch (levelID)
        {
            case 1:
                availableStages = Level::hytopiaCastleStageList;
                break;
            case 3:
                availableStages = Level::hytopiaShopsStageList;
                break;
            case 4:
                availableStages = Level::arenaList;
                break;
            default:
                availableStages = basicStages;
                break;
        }

        Keyboard chooseStage("Stage Selection", "Select a stage.");
        chooseStage.Populate(availableStages);

        // stages are not 0-indexed...
        return chooseStage.Open() + 1;
    }

    // Helper function that sets the current area's challenge, if applicable | Note: assumes valid level data inputted
    int Level::selChallenge(std::array<u8, 3> challenges)
    {
        StringVector availableChals;
        Keyboard challenge("Challenge Selection", "Choose a Drablands Challenge to enable.\n\nNote: in order to change the active challenge in the current level, you must enter a different level before attempting to re-enter the current level.");

        availableChals.push_back(GameData::getChallengeName(0x0)); // No challenge
        availableChals.push_back(GameData::getChallengeName(challenges[0]));
        availableChals.push_back(GameData::getChallengeName(challenges[1]));
        availableChals.push_back(GameData::getChallengeName(challenges[2]));

        challenge.Populate(availableChals);
        return challenge.Open();
    }

    // Retrieves world name given the ID | Note: only uses base levels
    std::string Level::worldIDToStr(int worldID)
    {
        return Level::worldList[worldID];
    }

    // Retrieves level ID given external name
	u8 Level::levelIDFromName(std::string name)
	{
        const std::map<std::string, Level> &levels = getAllLevels();
        for (const auto &mapEntry : levels)
        {
            if (mapEntry.first == name)
                return mapEntry.second._ID;
        }
        return 0xFF; // wasn't found
    }

    // Gets all Level data via search by levelID
    const std::pair<const std::string, Level> *Level::getLevelByID(u8 levelID)
    {
        const std::map<std::string, Level> &levels = getAllLevels();
        for (auto &entry : levels)
        {
            if (entry.second.getLevelID() == levelID)
                return &entry;
        }
        return nullptr;
    }

    // Retrieves the ID of the previous location
	u8 Level::getPrevLevel(void)
	{
		u8 levelID;
		Process::Read8(AddressList::getAddress("PreviousLevelID"), levelID);

		return levelID;
	}

    // Retrieves the ID of the current location
    u8 Level::getCurrLevel(void)
	{
		u8 levelID;
		Process::Read8(AddressList::getAddress("CurrLevelID"), levelID);

		return levelID;
	}

    // Retrieves the ID of the current stage
    u8 Level::getCurrStage(void)
	{
		u8 stageID;
		Process::Read8(AddressList::getAddress("CurrStageID"), stageID);

		return stageID;
	}

    // Retrieves the ID of the current challenge, if any
    u8 Level::getCurrChallenge(void)
	{
		u8 chalID;
		Process::Read8(AddressList::getAddress("ChallengeID"), chalID);

		return chalID;
	}

    // Retrieves the elapsed time since the level was started
    u32 Level::getElapsedTime(void)
	{
		u32 elapsedTime;
		Process::Read32(AddressList::getAddress("TimeElapsed"), elapsedTime);

		return elapsedTime;
	}

    // Overrides the current location ID
	void Level::setCurrLevel(u8 levelID)
	{
		Process::Write8(AddressList::getAddress("CurrLevelID"), levelID);
	}

    // Overrides the current stage ID
    void Level::setCurrStage(u8 stageID)
	{
		Process::Write8(AddressList::getAddress("CurrStageID"), stageID);
	}

    // Overrides the current challenge ID
    void Level::setCurrChal(u8 chalID)
	{
		Process::Write8(AddressList::getAddress("ChallengeID"), chalID);
	}

    // Checks if the current location belongs to the set of base levels/DoT
    bool Level::isInDrablands(u8 optionalLevel)
	{
		u8 level = optionalLevel == 0x0 ? getCurrLevel() : optionalLevel;
		return (level >= levelIDFromName("Deku Forest")) && (level <= levelIDFromName("Baneful Zone"));
	}

    // Checks if the current location belongs to DoT
    bool Level::isInDoT(bool includeWarpRoom, u8 optionalLevel)
    {
        u8 level = optionalLevel == 0x0 ? getCurrLevel() : optionalLevel;
        u8 minimumRange = includeWarpRoom ? levelIDFromName("DoT Warp Room") : levelIDFromName("Forest Zone");
        return (level >= minimumRange) && (level <= levelIDFromName("Baneful Zone"));
    }

    // Checks if the stage has finished loading
    bool Level::hasStageBegan(void)
	{
		return Level::getElapsedTime() >= 0x0;
	}

    // Checks if a specified amount of time has passed since the level was started
	bool Level::hasCertainTimeElapsed(int time)
	{
		return Level::getElapsedTime() >= static_cast<u32>(time);
	}
}