#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
	void openItemOptions(MenuEntry* entry) {
		// TODO: when enabled, cause the 3 link color options to appear in the menu
	}

	void gSetItem(MenuEntry* entry) {
		// TODO: open Combo Box and set chosen Item to Green's item address
	}

	void bSetItem(MenuEntry* entry) {
		// TODO: see gSetItem()
	}

	void rSetItem(MenuEntry* entry) {
		// TODO: see gSetItem()
	}

	void openShadowItemOptions(MenuEntry* entry) {
		// TODO: similar to openItemOptions()
	}
	void gShadowSetItem(MenuEntry* entry) {
		// TODO: similar to gSetItem()
		// TODO: we really need to figure out the pointers for Shadow Link data
	}

	void bShadowSetItem(MenuEntry* entry) {
		// TODO: similar to gSetItem()
	}

	void rShadowSetItem(MenuEntry* entry) {
		// TODO: similar to gSetItem()
	}

	void strafingSpeedSet(MenuEntry* entry) {
		// TODO:
		// 1) open combo box with item names (only 6 items have strafing speeds)
		// 2) when item is selected, open a numerical float keyboard
		// 3) set float input to chosen item's strafing address

		// Note: is opening a keyboard within a combo box doable?
	}
	void upgradeItemAlways(MenuEntry* entry) {
		// TODO: 
		// 1) finish documenting the costume attributes part in the executable
		// 2) simple memory writes modifying the executable to trigger the 
		// item upgrades
	}
}