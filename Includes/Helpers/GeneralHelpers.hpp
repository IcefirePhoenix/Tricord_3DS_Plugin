#ifndef GENERALHELPER_HPP
#define GENERALHELPER_HPP

#include "CTRPluginFramework.hpp"

namespace CTRPluginFramework {
	class GeneralHelpers {
	public:
		static std::string enabledSlider;
		static std::string disabledSlider;
		static std::string getLinkColorAsStr(int ID);

		static int getCurrLink(void);
		static int chooseLink(void);
		static int chooseShadowLink(void);
		static int chooseSword(void);

		static bool showMsgKbd(std::string text, DialogType digtype);
		static bool isNullPointer(u32 address);

		static void managePlayerLock(bool isLocked);
		static void forceDoppelStatus(bool enable);
		
		static bool isTitleScreen(void);
		static bool isSinglePlayer(void);
		static bool isLoadingScreen(void);
		static bool isPauseScreen(void);
	};
}
#endif