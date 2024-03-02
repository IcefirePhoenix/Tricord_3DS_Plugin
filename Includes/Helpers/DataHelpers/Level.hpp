#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "CTRPluginFramework.hpp"

namespace CTRPluginFramework {
	class Level {
	public:
		Level(u8 ID, std::string externalName, std::string internalName, bool dummyStatus);

		static const Level levelList[];
	
		static const StringVector hytopiaLevelList;
		static const StringVector hytopiaShopsStageList;
		static const StringVector hytopiaCastleStageList;

		static const StringVector worldList;
		static const StringVector woodlandLevelList;
		static const StringVector riversideLevelList;
		static const StringVector volcanoLevelList;
		static const StringVector iceLevelList;
		static const StringVector fortressLevelList;
		static const StringVector dunesLevelList;
		static const StringVector ruinsLevelList;
		static const StringVector skyLevelList;
		static const StringVector challengeList;
		static const StringVector dotZoneList;
		static const StringVector arenaList;

		static StringVector getWorldNamesfromID(int ID, bool useNonLevels);
		static std::string worldIDToStr(int worldID);
		static std::string levelNameFromID(u8 levelID);

		static int selWorld(bool useDoT, bool useNonLevels);
		static int selBasicStage(void);

		static u8 levelIDFromName(std::string name);
		static u8 getPrevLevel(void);
		static u8 getCurrLevel(void);
		static u8 getCurrStage(void);
		static u8 getCurrChallenge(void);
		static u32 getElapsedTime(void);

		static void setCurrLevel(u8 levelID); 
		static void setCurrStage(u8 stageID); 
		static void setCurrChal(u8 chalID); 

		static bool isInDrablands(void);
		static bool hasStageBegan(void);
		static bool hasCertainTimeElapsed(int time);

	private:
		u8				_levelID;
		std::string		_extName;
		std::string		_intName;
		bool			_isDummy;
	};
}
#endif