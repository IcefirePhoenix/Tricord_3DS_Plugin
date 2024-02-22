#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP

#include "CTRPluginFramework.hpp"

namespace CTRPluginFramework {
	class GameData {
	public:
		static const StringVector universalCostumeList;
		static const StringVector customCostumeList;

		static const StringVector swordList;
		static const StringVector voiceList;

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

		static const StringVector itemList;
		static const StringVector lobbyBallSongs;

		static const int generalPlayerIDs[];
		static const std::vector<u8> emoteIDs;

		static const u32 playerAddressOffset;

		static int getPlayerIDFromColor(std::string color);
		static std::string getSwordNameFromID(int ID);
		static std::string getCostumeNameFromID(int ID);
		static StringVector getWorldNamesfromID(int ID);
		static int selWorld(bool useDoT);
		static std::string worldIDToStr(int worldID);
		static std::string getVoiceAsStr(u8 ID);
		static u32 getLobbyBallDataAddress(void);
	};
}
#endif