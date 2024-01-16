#ifndef GENERALHELPER_HPP
#define GENERALHELPER_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	class GeneralHelpers {
	public:
		static std::string enabledSlider;
		static std::string disabledSlider;
		static std::string getLinkColorAsStr(int ID);

		static int getCurrLink(void);

		static u32 chooseLink(void);
		static u32 chooseShadowLink(void);

		static u8 chooseSword(void);

		static bool showMsgKbd(std::string text, DialogType digtype);
		static bool isNullPointer(u32 address);

		static void managePlayerLock(bool isLocked);
		static void forceEnableDoppels(void);
		
		static bool isSinglePlayer(void);
		static bool isLoadingScreen(void);
		static bool isPauseScreen(void);
	};
}
#endif