#include "Helpers.hpp"
#include "AddressList.hpp"

#include <CTRPluginFramework.hpp>

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
		Level(0x59, "Den of Trials", "AbyssPortal", false),
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

	u8 Level::getIDFromName(const std::string& name) 
	{
		for (int iterator = 0; iterator < 45; ++iterator) 
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

	u32 Level::getElapsedTime(void)
	{
		u32 elapsedTime;
		Process::Read32(AddressList::TimeElapsed.addr, elapsedTime);

		return elapsedTime;
	}

	bool Level::isInDrablands(void) 
	{
		u8 level = getCurrLevel();
		return (level >= getIDFromName("Deku Forest")) && (level <= getIDFromName("Baneful Zone"));
	}

	bool Level::hasStageBegan(void) 
	{
		return Level::getElapsedTime() >= 0x0;
	}

	bool Level::hasCertainTimeElapsed(int time)
	{
		return Level::getElapsedTime() >= static_cast<u32>(time);	
	}
}