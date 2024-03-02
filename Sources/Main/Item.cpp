#include "Helpers.hpp"
#include "Main/Item.hpp"

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework
{
	u8 currItems[3];
	u32 addresses[6];

	float currStrafingSpeeds[6];
	StringVector shadowItemList;

	void initItemAddresses(void)
    {
        Address temp[6] = {
			AddressList::BowStrafe, AddressList::FireGloveStrafe, AddressList::WaterRodStrafe,
			AddressList::GustStrafe, AddressList::GripshotStrafe, AddressList::HammerStrafe
		};

        for (int iterator = 0; iterator < 6; ++iterator) 
		{
            addresses[iterator] = temp[iterator].addr;
        }
    }
	
	void saveCurrItems(void) 
	{
		for (int iterateThruPlayers = 0; iterateThruPlayers < 3; ++iterateThruPlayers) 
		{
			u32 playerOffset = iterateThruPlayers * GameData::playerAddressOffset;
			Process::Read8(AddressList::CurrentItem.addr + playerOffset, currItems[iterateThruPlayers]);
		}
	}

	void Item::manageItems(MenuEntry* entry) 
	{
		if (entry->WasJustActivated()) 
			saveCurrItems();

		u8 noItem = 0x9;
		for (int iterateThruPlayers = 0; iterateThruPlayers < 3; ++iterateThruPlayers) 
		{
			u32 playerOffset = iterateThruPlayers * GameData::playerAddressOffset;
			u8 item = (entry->Name() == "Remove current items") ? noItem : currItems[iterateThruPlayers];

			Process::Write8(AddressList::CurrentItem.addr + playerOffset, item);
		}
	}

	std::string readCurrItems(u32 address, bool useShadow) 
	{
		StringVector itemList = GameData::itemList; 
		std::string str = "";
		u8 currItems[3];

		u32 indivOffset = useShadow ? 0x4 : 0x10000;

		if (useShadow) 
		{
			initShadowItemList();
			itemList = shadowItemList;
		}

		for (int checkItems = 0; checkItems < 3; ++checkItems) 
		{
			Process::Read8(address + (indivOffset * checkItems), currItems[checkItems]);

			char letter = 'A' + checkItems;
			std::string prefix = useShadow ? "\nShadow Link " : "\nPlayer ";

			if (useShadow)
			{
				std::string itemIndex = (currItems[checkItems] == 0x9) ?  itemList[0] : itemList[currItems[checkItems]]; 
				str.append(prefix + letter + ": " + itemIndex);
			}
			else
				str.append(prefix + letter + ": " + itemList[currItems[checkItems] - 1]);
		}
		return str;
	}

	void Item::itemOpt(MenuEntry* entry) 
	{
		int link = GeneralHelpers::chooseLink();

		if (link >= 0) 
		{
			u32 playerOffset = static_cast<u32>(link) * GameData::playerAddressOffset;

			std::string intro = "Select an item.\n\nCurrent items:\n";
			intro.append(readCurrItems(AddressList::CurrentItem.addr, false));

			Keyboard item(intro);
			item.Populate(GameData::itemList);

			int choice = item.Open();
			if (choice >= 0) 
			{
				choice++; // since the list isn't 0-indexed
				Process::Write8(AddressList::CurrentItem.addr + playerOffset, choice);
				currItems[static_cast<int>(link)] = choice;
			}
		}
	}

	void initShadowItemList(void) 
	{
		shadowItemList = GameData::itemList;

		shadowItemList.pop_back();
		shadowItemList.insert(shadowItemList.begin(), "Randomly switch items :)");
	}

	u32 getShadowItemAddress(void) 
	{
		u32 shadowItemAddress;
		Process::Read32(AddressList::ShadowLinkItemPointer.addr, shadowItemAddress);

		return shadowItemAddress;
	}

	void Item::shadowItemOpt(MenuEntry* entry) 
	{
		int shadowLinkChoice = GeneralHelpers::chooseShadowLink();

		if (shadowLinkChoice >= 0)
		{
			u32 indivShadowOffset = static_cast<u32>(shadowLinkChoice * 4);
			u32 shadowItemPointerOffset = 0x20;

			initShadowItemList();

			u32 shadowDataAddress = getShadowItemAddress() + shadowItemPointerOffset; // get dynamic address via pointer

			bool isInBaneful = (Level::getCurrLevel() == Level::levelIDFromName("Baneful Zone")) && (Level::getCurrStage() == 4);
			std::string msg = isInBaneful ? "Select an item." : "Error\n\nThe current level is not Baneful Zone - 4. Please\nenter Baneful Zone and try again.";

			if (isInBaneful) 
			{
				msg.append("\n\nCurrent Shadow Link items:\n");
				msg.append(readCurrItems(shadowDataAddress, true));
				msg.append("\n\nBe sure to re-load the stage for changes to\ntake effect.");

				Keyboard item(msg);
				item.Populate(shadowItemList);

				int choice = item.Open();
				if (choice == 0)
					Process::Write8(shadowDataAddress + indivShadowOffset, 0x9); // random items are the last ID despite being the first list item
				else if (choice > 0)
					Process::Write8(shadowDataAddress + indivShadowOffset, choice);

			}
			else 
			{
				const StringVector temp = { "Exit" };
				Keyboard item(msg);
				item.Populate(temp);

				int tempChoice = item.Open();
			}
		}	
	}

	void readCurrStrafingSpeeds(void) 
	{
		initItemAddresses();

		for (int iterator = 0; iterator < 6; ++iterator)
			Process::ReadFloat(addresses[iterator], currStrafingSpeeds[iterator]);
	}

	void resetAllStrafingSpeeds(void) 
	{
		for (int iterator = 0; iterator < 6; ++iterator) 
			Process::WriteFloat(addresses[iterator], 1.0);
	}

	void Item::strafingSpeedSet(MenuEntry* entry) 
	{
		float result = 1.0;

		const StringVector items = {
			"Reset all speeds",
			"Bow",
			"Fire Gloves",
			"Water Rod",
			"Gust Jar",
			"Gripshot",
			"Hammer"
		};

		readCurrStrafingSpeeds();

		std::string msg = "Select an item to change its strafing speed.\nDefault speed for all items: 1.0";
		msg.append("\nBow: " + std::to_string(currStrafingSpeeds[0]));
		msg.append("\nFire Gloves: " + std::to_string(currStrafingSpeeds[1]));
		msg.append("\nWater Rod: " + std::to_string(currStrafingSpeeds[2]));
		msg.append("\nGust Jar: " + std::to_string(currStrafingSpeeds[3]));
		msg.append("\nGripshot: " + std::to_string(currStrafingSpeeds[4]));
		msg.append("\nHammer: " + std::to_string(currStrafingSpeeds[5]));

		Keyboard speed(msg);
		speed.Populate(items);

		int choice = speed.Open();

		if (choice == 0)
			resetAllStrafingSpeeds();
		else if (choice > 0){
			result = setSpeedKB();
			Process::WriteFloat(addresses[choice - 1], result);
		}
	}

	float setSpeedKB(void) 
	{
		float result;

		Keyboard speedKB("Set strafing speed:");
		speedKB.IsHexadecimal(false);
		
		if (speedKB.Open(result) == 0) 
		{
			if (result < 0.0)
				MessageBox(Color::Gainsboro << "Error", "Strafing speed cannot be negative.")();
			else
				return result;
		}

		return 1.0;
	}

	void Item::upgradeItemAlways(MenuEntry* entry) 
	{
		// TODO: 
		// 1) finish documenting costume attributes from executable
		// 2) simple memory writes -> trigger upgrades for ALL items 
	}
}