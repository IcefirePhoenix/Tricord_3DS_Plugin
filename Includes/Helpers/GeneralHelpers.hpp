#ifndef GENERALHELPER_HPP
#define GENERALHELPER_HPP

#define PLAYER_OFFSET 0x10000

#define ENABLED_SLIDER "\u2282\u25CF"
#define DISABLED_SLIDER "\u25CF\u2283"

#include "CTRPF.hpp"

namespace CTRPluginFramework
{
	class GeneralHelpers
	{
	public:
		static std::string getLinkColorAsStr(int ID);
		static std::string getPlayerAsStr(int ID);

		static int chooseLink(void);
		static int getCurrLink(void);
		static int chooseSword(void);
		static int getHP(void);

		static bool showMsgKbd(std::string title, std::string text, DialogType digtype);
		static bool isNullPointer(u32 address);

		static void managePlayerLock(bool isLocked);
		static void forceDoppelStatus(bool enable);
		static void setCurrLink(int playerID);
		static void setPlayerCam(int playerID);

		static bool isTitleScreen(void);
		static bool isSinglePlayer(void);
		static bool isLoadingScreen(bool checkInitStateOnly);
		static bool isPauseScreen(void);
		static bool isSoloActiveGameplay(void);
	};
}
#endif