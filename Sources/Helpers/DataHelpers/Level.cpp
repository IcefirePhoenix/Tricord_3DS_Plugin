#include "Helpers.hpp"

namespace CTRPluginFramework
{
	Level::Level(u8 ID, std::string externalName, std::string internalName, bool dummyStatus) :
		_levelID(ID), _extName(externalName), _intName(internalName), _isDummy(dummyStatus)
	{
	}

	const Level Level::levelList[45] = {
		Level(0x1, "Hytopia Castle", "StartCastle", false),
		Level(0x2, "Hytopia", "StartVillage", false),
		Level(0x3, "Hytopia Shops", "IndoorVillage", false),
		Level(0x4, "Coliseum", "Colosseum", false),
		Level(0xA, "Deku Forest", "GrassTango", false),
		Level(0xB, "Buzz Blob Cave", "GrassEcho", false),
		Level(0xC, "Moblin Base", "GrassAlpha", false),
		Level(0xD, "Forest Temple", "GrassIndia", false),
		Level(0x14, "Secret Fortress", "WaterQuebec", false),
		Level(0x15, "Abyss of Agony", "WaterIndia", false),
		Level(0x16, "Cove of Transition", "WaterNovember", false),
		Level(0x17, "Water Temple", "WaterAlpha", false),
		Level(0x1E, "Blazing Trail", "FireAlpha", false),
		Level(0x1F, "Hinox Mine", "FireEcho", false),
		Level(0x20, "Den of Flames", "FireKilo", false),
		Level(0x21, "Fire Temple", "FireFoxtrot", false),
		Level(0x28, "Frozen Plateau", "IceHotel", false),
		Level(0x29, "Snowball Ravine", "IceKilo", false),
		Level(0x2A, "Silver Shrine", "IceIndia", false),
		Level(0x2B, "Ice Temple", "IceLima", false),
		Level(0x32, "Sealed Gateway", "FortFoxtrot", false),
		Level(0x33, "Bomb Storage", "FortEcho", false),
		Level(0x34, "Training Ground", "FortAlpha", false),
		Level(0x35, "The Lady's Lair", "FortGolf", false),
		Level(0x3C, "Infinity Dunes", "SandQuebec", false),
		Level(0x3D, "Stone Corridors", "SandNovember", false),
		Level(0x3E, "Gibdo Mausoleum", "SandRomeo", false),
		Level(0x3F, "Desert Temple", "SandOscar", false),
		Level(0x46, "Illusory Mansion", "DarkHotel", false),
		Level(0x47, "Palace Noir", "DarkAlpha", false),
		Level(0x48, "Lone Labyrinth", "DarkTango", false),
		Level(0x49, "Grim Temple", "DarkNovember", false),
		Level(0x50, "Floating Garden", "SkyKilo", false),
		Level(0x51, "Deception Castle", "SkyQuebec", false),
		Level(0x52, "Dragon Citadel", "SkyHotel", false),
		Level(0x53, "Sky Temple", "SkyAlpha", false),
		Level(0x59, "DoT Warp Room", "AbyssPortal", false),
		Level(0x5A, "Forest Zone", "AbyssAlpha", false),
		Level(0x5B, "Flooded Zone", "AbyssBravo", false),
		Level(0x5C, "Scorching Zone", "AbyssCharlie", false),
		Level(0x5D, "Frozen Zone", "AbyssDelta", false),
		Level(0x5E, "Fortified Zone", "AbyssEcho", false),
		Level(0x5F, "Desert Zone", "AbyssFoxtrot", false),
		Level(0x60, "Shadow Zone", "AbyssGolf", false),
		Level(0x61, "Baneful Zone", "AbyssHotel", false)

		// Developer levels, Test levels, and Dummy levels are not added here
		// Memory(?) issues with Citra when this array exceeded 0x5E elements...
	};

	const StringVector Level::hytopiaLevelList = {
        "Hytopia",
        "Hytopia Shops",
        "Hytopia Castle"
    };

    const StringVector Level::hytopiaShopsStageList = {
        "Miiverse Gallery",
        "Madame Couture's",
        "Daily Riches"
    };

    const StringVector Level::hytopiaCastleStageList = {
        "Entrance",
        "Single Player Lobby",
        "Multiplayer Lobby",
        "Throne Room",
        "Coliseum Hallway",
        "Coliseum Lobby"
    };

    const StringVector Level::worldList = {
        "Woodlands",
        "Riverside",
        "Volcano",
        "Ice Cavern",
        "Fortress",
        "The Dunes",
        "The Ruins",
        "Sky Realm"
    };

    const StringVector Level::woodlandLevelList = {
        "Deku Forest",
        "Buzz Blob Cave",
        "Moblin Base",
        "Forest Temple"
    };

    const StringVector Level::riversideLevelList = {
        "Secret Fortress",
        "Abyss of Agony",
        "Cove of Transition",
        "Water Temple"
    };

    const StringVector Level::volcanoLevelList = {
        "Blazing Trail",
        "Hinox Mine",
        "Den of Flames",
        "Fire Temple"
    };

    const StringVector Level::iceLevelList = {
        "Frozen Plateau",
        "Snowball Ravine",
        "Silver Shrine",
        "Ice Temple"
    };

    const StringVector Level::fortressLevelList = {
        "Sealed Gateway",
        "Bomb Storage",
        "Training Ground",
        "The Lady's Lair"
    };

    const StringVector Level::dunesLevelList = {
        "Infinity Dunes",
        "Stone Corridors",
        "Gibdo Mausoleum",
        "Desert Temple"
    };

    const StringVector Level::ruinsLevelList = {
        "Illusory Mansion",
        "Palace Noir",
        "Gibdo Mausoleum",
        "Grim Temple"
    };

    const StringVector Level::skyLevelList = {
        "Floating Garden",
        "Deception Castle",
        "Dragon Citadel",
        "Sky Temple"
    };

    const StringVector Level::challengeList = {
        "No challenge",
        "Challenge 1",
        "Challenge 2",
        "Challenge 3"
    };

    const StringVector Level::dotZoneList = {
        "Forest Zone",
        "Flooded Zone",
        "Scorching Zone",
        "Frozen Zone",
        "Fortified Zone",
        "Desert Zone",
        "Shadow Zone",
        "Baneful Zone"
    };

    const StringVector Level::arenaList = {
        "Woodlands Arena",
        "Riverside Arena",
        "Volcano Arena",
        "Ice Cavern Arena",
        "Fortress Arena",
        "Dunes Arena",
        "Ruins Arena",
        "Sky Realm Arena"
    };


    StringVector Level::getWorldNamesfromID(int ID, bool useNonLevels)
    {
        if (!useNonLevels)
            ID = ID + 2;

        switch (ID)
        {
            case 0:
                return Level::hytopiaLevelList;
            case 1:
                return Level::arenaList;
            case 2:
                return Level::woodlandLevelList;
            case 3:
                return Level::riversideLevelList;
            case 4:
                return Level::volcanoLevelList;
            case 5:
                return Level::iceLevelList;
            case 6:
                return Level::fortressLevelList;
            case 7:
                return Level::dunesLevelList;
            case 8:
                return Level::ruinsLevelList;
            case 9:
                return Level::skyLevelList;
            case 10:
                return Level::dotZoneList;
            default:
                return StringVector();
        }
    }

    int Level::selWorld(bool useDoT, bool useNonLevels)
    {
        StringVector worldSelectionList = Level::worldList;

        if (useNonLevels)
        {
            worldSelectionList.clear();
            worldSelectionList.insert(worldSelectionList.begin(), "Levels");
		    worldSelectionList.insert(worldSelectionList.begin(), "Coliseum");
       		worldSelectionList.insert(worldSelectionList.begin(), "Hytopia");
        }

        if (useDoT)
            worldSelectionList.push_back("Den of Trials");

        Keyboard chooseWorld("Select a world:");
        chooseWorld.Populate(worldSelectionList);

        return chooseWorld.Open();
    }

    // TODO: right now this only uses woodlands-sky
    std::string Level::worldIDToStr(int worldID)
    {
        return Level::worldList[worldID];
    }

	std::string Level::levelNameFromID(u8 levelID)
	{
		for (int iterator = 0; iterator < 45; iterator++)
		{
			if (levelList[iterator]._levelID == levelID)
				return levelList[iterator]._extName;
		}
		return ""; // wasn't found
	}

	u8 Level::levelIDFromName(std::string name)
	{
		for (int iterator = 0; iterator < 45; iterator++)
		{
			if (levelList[iterator]._extName == name)
				return levelList[iterator]._levelID;
		}
		return -1; // wasn't found
	}

	u8 Level::getPrevLevel(void)
	{
		u8 levelID;
		Process::Read8(AddressList::PreviousLevelID.addr, levelID);

		return levelID;
	}

	u8 Level::getCurrLevel(void)
	{
		u8 levelID;
		Process::Read8(AddressList::CurrLevelID.addr, levelID);

		return levelID;
	}

	u8 Level::getCurrStage(void)
	{
		u8 stageID;
		Process::Read8(AddressList::CurrStageID.addr, stageID);

		return stageID;
	}

	u8 Level::getCurrChallenge(void)
	{
		u8 chalID;
		Process::Read8(AddressList::ChallengeID.addr, chalID);

		return chalID;
	}

	u32 Level::getElapsedTime(void)
	{
		u32 elapsedTime;
		Process::Read32(AddressList::TimeElapsed.addr, elapsedTime);

		return elapsedTime;
	}

	void Level::setCurrLevel(u8 levelID)
	{
		Process::Write8(AddressList::CurrLevelID.addr, levelID);
	}

	void Level::setCurrStage(u8 stageID)
	{
		Process::Write8(AddressList::CurrStageID.addr, stageID);
	}

	void Level::setCurrChal(u8 chalID)
	{
		Process::Write8(AddressList::ChallengeID.addr, chalID);
	}

	bool Level::isInDrablands(u8 optionalLevel)
	{
		u8 level = optionalLevel == 0x0 ? getCurrLevel() : optionalLevel;
		return (level >= levelIDFromName("Deku Forest")) && (level <= levelIDFromName("Baneful Zone"));
	}

    bool Level::isInDoT(u8 optionalLevel)
    {
        u8 level = optionalLevel == 0x0 ? getCurrLevel() : optionalLevel;
        return (level >= levelIDFromName("Forest Zone")) && (level <= levelIDFromName("Baneful Zone"));
    }

    bool Level::hasStageBegan(void)
	{
		return Level::getElapsedTime() >= 0x0;
	}

	bool Level::hasCertainTimeElapsed(int time)
	{
		return Level::getElapsedTime() >= static_cast<u32>(time);
	}

	int Level::selBasicStage(void)
	{
		StringVector stages =
        {
			"Stage 1",
			"Stage 2",
			"Stage 3",
			"Stage 4",
			"Treasure Room"
		};

		Keyboard selStage("Select a stage:");
        selStage.Populate(stages);

        int result = selStage.Open();

        // stages are not 0-indexed...
        if (result >= 0)
            return result + 1;
        else
            return result; // failure
    }
}