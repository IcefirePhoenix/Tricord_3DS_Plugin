#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Main/AltModes/Chaos.hpp"

namespace CTRPluginFramework
{

	void openMenu(MenuEntry* entry)
	{
		// show all options sorted into menus
		// and separated by group for organzation purposes
	}

	void initRNG(void)
	{
		// if any of the following were not chosen : reset all + view all active
		// add all selected options into an array
	}


	// any options chosen here should be added to a list whose values are 
	// randomly selected from when a stage is started
	void selOptions(MenuEntry* entry)
	{

	}

	// randomly choosing an effect from the list of user-chosen options
	void randomEffect(void)
	{

	}

	// set the effect either by calling the effect function in the Chaos class or calling a 
	// function from another class
	// effects should be grouped up for organization and readability purposes
	void setEffect(int effectID)
	{
		// this means some effect IDs will be moved around 
		// shouldn't matter since this doesn't affect the percent chances
		// group 1:
		// group 2:
	
	}

	void getAllActiveEffects(void)
	{
		
	}

}