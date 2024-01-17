#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/GeneralHelpers.hpp"

#include "3ds.h"

namespace CTRPluginFramework
{
	void customColor(MenuEntry* entry) {
		/*
		int playerID = GeneralHelpers::chooseLink();
		int colorChoice = selCustomLinkColor();
		*/
		
		// see recolorLevelTex note
		// Unsure how much memory is allocated for costume files; it doesn't seem like much
		// It'll be incredibly difficult to pack in more than 5-6 colors per costume file
		// especially for the larger files... for now, this will be placed on hold
	}

	int selCustomLinkColor(void) {
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

		Keyboard keyboard("Choose a color:");
		keyboard.Populate(list);

		return keyboard.Open();
	}

	void recolorLevelTex(MenuEntry* entry) {
		// A little bit hesitant on implementing this since we would have to assume
		// all of the available color mods are available for us to use with permission 
		// AND that the game has enough memory to accommodate the additional textures at 
		// any time AND that no vital addresses are shifted around as a result
		
		// TODO: 
		// 1) repack levels that have color-specific textures (like the colored platforms in Ruins) with custom textures
		// 2) locate texture pointers in memory
		// 3) figure out correct edits
		// 4) if currently in __ level, set edits
	}
}