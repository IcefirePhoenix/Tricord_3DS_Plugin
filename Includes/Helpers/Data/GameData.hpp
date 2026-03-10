#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP

#include "CTRPF.hpp"

namespace CTRPluginFramework
{
	class GameData
	{
	public:
		static StringVector universalCostumeList;

		static const StringVector swordList;
		static const StringVector voiceList;

		static const StringVector itemList;
		static const StringVector lobbyBallSongs;

		static const int generalPlayerIDs[];
		static const std::vector<u8> emoteIDs;

		static const std::unordered_map<std::string, std::string> actorNames;
		static const std::unordered_map<u8, std::string> challengeList;

		static int maxCostumeCount;

		static int getPlayerIDFromColor(std::string color);
		static int getCostumeIDFromName(std::string costumeName);
		static std::string getSwordNameFromID(int ID);
		static std::string getChallengeName(int chalID);
		static std::string getCostumeNameFromID(int ID);
		static std::string getVoiceAsStr(u8 ID);
		static u32 getLobbyBallDataAddress(void);
	};
}
#endif