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
		static const StringVector worldList;
		static const StringVector itemList;
		static const StringVector lobbyBallSongs;

		static const int generalPlayerIDs[];
		static const std::vector<u8> emoteIDs;

		static u32 playerAddressOffset;

		static int GameData::getPlayerIDFromColor(std::string color);
	};
}
#endif