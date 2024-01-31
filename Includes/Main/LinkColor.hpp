#ifndef LINKCOLOR_HPP
#define LINKCOLOR_HPP

#include "CTRPluginFramework.hpp"
#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Unicode.h"

namespace CTRPluginFramework {
	int selCustomLinkColor(void);

	class LinkColor {
	public:
		// Link Color functions
		static void customColor(MenuEntry* entry);
		static void recolorLevelTex(MenuEntry* entry);
	};
}
#endif