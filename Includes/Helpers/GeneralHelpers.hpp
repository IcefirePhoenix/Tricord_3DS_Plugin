#ifndef GENERALHELPER_HPP
#define GENERALHELPER_HPP

#define TID_USA 		0x0004000000176F00
#define TID_EUR 		0x0004000000177000
#define TID_JPN 		0x0004000000176E00
#define PLAYER_OFFSET 	0x10000

#define ENABLED_SLIDER 		Color::LimeGreen << "\u2282\u25CF"
#define DISABLED_SLIDER 	Color::Red << "\u25CF\u2283"

#include "CTRPF.hpp"

namespace CTRPluginFramework
{
	class GeneralHelpers
	{
	public:
		static std::string getLinkColorAsStr(int ID);

		static int chooseLink(void);
		static int chooseShadowLink(void);
		static int getCurrLink(void);
		static int chooseSword(void);
		static int getHP(void);

		static bool showMsgKbd(std::string text, DialogType digtype);
		static bool isNullPointer(u32 address);

		static void managePlayerLock(bool isLocked);
		static void forceDoppelStatus(bool enable);
		static void setCurrLink(int playerID);
		static void setPlayerCam(int playerID);

		static bool isTitleScreen(void);
		static bool isSinglePlayer(void);
		static bool isLoadingScreen(void);
		static bool isPauseScreen(void);
		static bool isSoloActiveGameplay(void);
	};
}
#endif