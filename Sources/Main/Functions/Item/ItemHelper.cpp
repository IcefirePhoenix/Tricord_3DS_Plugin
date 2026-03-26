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

			// Players are labeled via suffixes A-C or 1-3...
			if (useShadow)
			{
				str += "\nShadow Link ";
				str += static_cast<char>('A' + checkItems);
			}
			else
			{
				str += "\nPlayer ";
				str += std::to_string(checkItems + 1);
			}

			str += ": ";

			/**
			 * The custom Shadow Link item list includes a special entry for randomizing the current item.
			 * This entry is located at the beginning of the list defined in the plugin, but actually uses
			 * the last data ID (0x9) in-game...
			 */

			u8 rawItemVal = currItems[checkItems];
			u8 adjustedItemIndex = useShadow ? ((rawItemVal == 0x9) ? 0 : rawItemVal) : (rawItemVal - 1);

			if (adjustedItemIndex < itemList.size())
				str += itemList[adjustedItemIndex];
			else
			{
				MessageBox("Error", "Item indexes could not be parsed correctly.")();
				str += "Unknown";
			}
		}
		return str;
	}
}