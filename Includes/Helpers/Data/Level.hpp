#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "CTRPF.hpp"

namespace CTRPluginFramework
{
	class Level
	{
	public:
		Level();
		Level(u8 levelID, bool dummyStatus, int world, std::array<u8, 3> challenges = {0xFF, 0xFF, 0xFF});

		enum class World : int
		{
			Woodlands = 0,
			Riverside = 1,
			Volcano = 2,
			Ice = 3,
			Fortress = 4,
			Dunes = 5,
			Ruins = 6,
			Sky = 7,
			DoT = 8,
			Hytopia = 9,
			Coliseum = 10
		};

		static const std::map<std::string, Level> nonLevelMap;
		static const std::map<std::string, Level> levelMap;
		static const std::map<std::string, Level> DoTLevelMap;

		static const StringVector hytopiaShopsStageList;
		static const StringVector hytopiaCastleStageList;
		static const StringVector arenaList;
		static const StringVector worldList;

		// level map(s) functions
		static StringVector getLevelNamesFromWorld(int worldID);
		static StringVector buildLevelNameList(const std::map<std::string, Level> &levelMap, World world);
		static std::string worldIDToStr(int worldID);
		static std::map<std::string, Level> getAllLevels(void);
		static std::pair<std::string, Level> selLevel(int world);
		static u8 levelIDFromName(std::string name);
		static const std::pair<const std::string, Level> *getLevelByID(u8 levelID);

		// level menu selection helpers
		static int selDrablandsWorld(bool useDoT);
		static int selCategory(void);
		static int selStage(u8 levelID);
		static int selChallenge(std::array<u8, 3> challenges);

		// real-time level data
		static u8 getTargetLevel(void);
		static u8 getTargetStage(void);
		static u8 getCurrLevel(void);
		static u8 getCurrStage(void);
		static u8 getCurrChallenge(void);
		static u32 getElapsedTime(void);

		static void setCurrLevel(u8 levelID);
		static void setCurrStage(u8 stageID);
		static void setCurrChal(u8 chalID);

		static bool isInDrablands(u8 optionalLevel = 0x0);
		static bool isInDoT(bool includeWarpRoom, u8 optionalLevel = 0x0);
		static bool hasStageBegan(void);
		static bool hasCertainTimeElapsed(int time);

		// Level object helpers
		u8 getLevelID(void) const { return _ID; };
		int getWorld(void) const { return _world; };
		std::array<u8, 3> getChallenges(void) const { return _challenges; };

	private:
		u8 _ID;
		bool _isDummy;
		int _world;
		std::array<u8, 3> _challenges;
	};
}
#endif