#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP

#include "CTRPF.hpp"

namespace CTRPluginFramework {
	class GameData {
	public:
		static const StringVector universalCostumeList;
		static const StringVector customCostumeList;

		static const StringVector swordList;
		static const StringVector voiceList;

		static const StringVector itemList;
		static const StringVector lobbyBallSongs;

		static const int generalPlayerIDs[];
		static const std::vector<u8> emoteIDs;

		static const u32 playerAddressOffset;

		static const std::map<std::string, std::string> actorNames;

		static int getPlayerIDFromColor(std::string color);
		static int getCostumeIDFromName(std::string costumeName);
		static std::string getSwordNameFromID(int ID);
		static std::string getCostumeNameFromID(int ID);
		static std::string getVoiceAsStr(u8 ID);
		static u32 getLobbyBallDataAddress(void);
	};
}
#endif