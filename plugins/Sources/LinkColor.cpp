#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/PlayerHelper.hpp"

#include "3ds.h"

namespace CTRPluginFramework
{

	void customColor(MenuEntry* entry) {

		int playerID = chooseLink();
		int colorChoice = selCustomLinkColor();

		// TODO: finish...

		// return confirmation message
	}

	int selCustomLinkColor(void) {

		Keyboard keyboard("Choose a color:");
		static const StringVector list =
		{
			"Red",
			"Yellow",
			"Green",
			"Blue",
			"Purple",
			"Pink",
			"White",
			"Hytopia Link",
			"Shadow Link"
		};
		keyboard.Populate(list);
		return keyboard.Open();
	}

	void recolorLevelTex(MenuEntry* entry) {
		// TODO: 
		// 1) repack levels that have color-specific textures (like the colored platforms in Ruins) with custom textures
		// 2) locate texture pointers in memory
		// 3) figure out correct edits
		// 4) if currently in __ level, set edits
	}
}