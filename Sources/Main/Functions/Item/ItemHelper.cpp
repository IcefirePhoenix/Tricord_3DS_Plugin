#include "Main/Item.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
	StringVector Item::shadowItemList = {};
	u8 Item::currItems[3] = {0};
	u32 Item::addresses[6] = {0};

	/* ------------------ */

	// Retrieve addresses for item strafing speeds
	void Item::initItemAddresses(void)
    {
        const char* names[6] =
		{
			"BowStrafe", "FireGloveStrafe", "WaterRodStrafe",
			"GustStrafe", "GripshotStrafe", "HammerStrafe"
		};

        for (int iterator = 0; iterator < 6; iterator++)
		{
			Item::addresses[iterator] = AddressList::getAddress(names[iterator]);
		}
    }

	// Gets current item data for normal players / Shadow Links and returns them as strings
	std::string readCurrItems(u32 address, bool useShadow)
	{
		StringVector itemList = GameData::itemList;
		std::string str = "";
		u8 currItems[3];

		// Shadow link data -> adjacent memory locations...
		u32 indivOffset = useShadow ? 0x4 : PLAYER_OFFSET;
		if (useShadow)
		{
			initShadowItemList();
			itemList = Item::shadowItemList;
		}

		// Read data and convert to strings...
		for (int checkItems = 0; checkItems < 3; checkItems++)
		{
			Process::Read8(address + (indivOffset * checkItems), currItems[checkItems]);

			// Menu interface -> players are labeled via suffixes A, B, or C...
			char letter = 'A' + checkItems;
			std::string prefix = useShadow ? "\nShadow Link " : "\nPlayer ";

			// Append the current item name to the player labels...
			if (useShadow)
			{
				/**
				 * The custom Shadow Link item list includes a special entry for randomizing the current item.
				 * This entry is located at the beginning of the list defined in the plugin, but actually uses
				 * the last data ID (0x9) in-game...
				 */
				std::string itemIndex = (currItems[checkItems] == 0x9) ? itemList[0] : itemList[currItems[checkItems]];
				str.append(prefix + letter + ": " + itemIndex);
			}
			else
				str.append(prefix + letter + ": " + itemList[currItems[checkItems] - 1]);
		}
		return str;
	}
}