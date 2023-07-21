#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{

	void customColorG(MenuEntry* entry) {

		int colorChoice = selectLinkColor();
		// TODO: use the result of colorChoice() to set the texture names at texture name
		// address (see File Contents in the spreadsheet) and force the game to use a certain texture color

		// Optional TODO: set a Message Box confirmation
		// or use an OSD notification...
	}


	void customColorB(MenuEntry* entry) {

		int colorChoice = selectLinkColor();
		//TODO: see customcolorG()
	}


	void customColorR(MenuEntry* entry) {

		int colorChoice = selectLinkColor();
		//TODO: see customcolorG()
	}


	int selectLinkColor(void) {

		// This opens up a Combo Box, which is part of the Keyboard class.
		// A Combo Box basically a list of options on the bottom screen.

		// This string is displayed on the top screen:
		Keyboard keyboard("Choose a color:");
		// Populate the bottom screen with options:
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
		// 2) locate in memory where these texture names are found
		// 3) figure out correct edits
		// 4) if currently in __ level, set edits

		// No need for a confirmation message since this is a checkbox entry
	}
}