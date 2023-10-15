#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
	void itemOpt(MenuEntry* entry) {
		// TODO: 
		// 1) call chooseLink()
		// 2) set item
	}

	void shadowItemOpt(MenuEntry* entry) {
		// TODO: similar to itemOpt()
		// Note: need to figure out the pointers for Shadow Link data

	}
	void strafingSpeedSet(MenuEntry* entry) {
		// TODO:
		// 1) open combo box with item names (only 6 items have strafing speeds)
		// 2) when item is selected, open a numerical float keyboard
		// 3) set float input to chosen item's strafing address
	}

	void upgradeItemAlways(MenuEntry* entry) {
		// TODO: 
		// 1) finish documenting costume attributes from executable
		// 2) simple memory writes -> trigger upgrades for ALL items 
	}
}