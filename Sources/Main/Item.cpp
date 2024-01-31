#include "Helpers.hpp"
#include "Main/Item.hpp"

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework
{
	u8 currItems[3];
	StringVector shadowItemList = GameData::itemList;

	void saveCurrItems(void) {
		for (int iterateThruPlayers = 0; iterateThruPlayers < 3; ++iterateThruPlayers) {
			u32 playerOffset = iterateThruPlayers * GameData::playerAddressOffset;

			Process::Read8(AddressList::CurrentItem.addr + playerOffset, currItems[iterateThruPlayers]);
		}
	}

	void Item::freezeCurrItems(MenuEntry* entry) {
		if (entry->WasJustActivated()) saveCurrItems();

		for (int iterateThruPlayers = 0; iterateThruPlayers < 3; ++iterateThruPlayers) {
			u32 playerOffset = iterateThruPlayers * GameData::playerAddressOffset;

			Process::Write8(AddressList::CurrentItem.addr + playerOffset, currItems[iterateThruPlayers]);
		}
	}

	std::string readCurrItems(u32 address, bool useShadow) {
		StringVector itemList = GameData::itemList;
		std::string str = "";
		u8 currItems[3];

		u32 indivOffset = useShadow ? 0x4 : 0x10000;

		if (useShadow)
			initShadowItemList();

		for (int checkItems = 0; checkItems < 3; ++checkItems) {
			Process::Read8(address + (indivOffset * checkItems), currItems[checkItems]);

			char letter = 'A' + checkItems;
			std::string prefix = useShadow ? "\nShadow Link " : "\nPlayer ";

			str.append(prefix + letter + ": " + itemList[currItems[checkItems] - 1]);
		}
		return str;
	}

	void Item::itemOpt(MenuEntry* entry) {
		u32 link = GeneralHelpers::chooseLink();

		if (link >= 0) {
			u32 playerOffset = link * GameData::playerAddressOffset;

			std::string intro = "Select an item.\n\nCurrent items:\n";
			intro.append(readCurrItems(AddressList::CurrentItem.addr, false));

			Keyboard item(intro);
			item.Populate(GameData::itemList);

			int choice = item.Open();

			if (choice >= 0) {
				choice++;
				Process::Write8(AddressList::CurrentItem.addr + playerOffset, choice);
				currItems[static_cast<int>(link)] = choice;
			}
		}
	}

	void initShadowItemList(void) {
		shadowItemList.pop_back();
		shadowItemList.push_back("Randomly switch between items :)");
	}

	u32 getShadowItemAddress(void) {
		u32 shadowItemAddress;
		Process::Read32(AddressList::ShadowLinkItemPointer.addr, shadowItemAddress);

		return shadowItemAddress;
	}

	void Item::shadowItemOpt(MenuEntry* entry) {
		u32 indivShadowOffset = GeneralHelpers::chooseShadowLink() * 0x4;
		u32 shadowItemPointerOffset = 0x20;

		if (indivShadowOffset >= 0) {
			initShadowItemList();

			u32 shadowDataAddress = getShadowItemAddress(); // get dynamic address via pointer

			bool isInBaneful = Level::getCurrLevel() == Level::getIDFromName("Baneful Zone");
			std::string msg = isInBaneful ? "Select an item." : "Error\n\nThe current level is not Baneful Zone. Please enter\nBaneful Zone and try again.";

			if (isInBaneful) {
				msg.append("\n\nCurrent Shadow Link items:\n");
				msg.append(readCurrItems(shadowDataAddress, true));

				Keyboard item(msg);
				item.Populate(shadowItemList);

				int choice = item.Open();
				if (choice >= 0)
					Process::Write8(shadowDataAddress + indivShadowOffset + shadowItemPointerOffset, choice + 1);
			}
			else {
				const StringVector temp = { "Exit" };
				Keyboard item(msg);
				item.Populate(temp);

				int tempChoice = item.Open();
			}
		}	
	}

	// Old impl; these go unused
	u32 addresses[6] = {
		AddressList::BowStrafe.addr,
		AddressList::FireGloveStrafe.addr,
		AddressList::WaterRodStrafe.addr,
		AddressList::GustStrafe.addr,
		AddressList::RoboStrafe.addr,
		AddressList::HammerStrafe.addr
	};

	float currStrafingSpeeds[6] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };

	// this is the alterative fix
	float bowStrafe, fireStrafe, waterStrafe, gustStrafe, roboStrafe, hammerStrafe;

	void readCurrStrafingSpeeds(void) {
		// Putting Addresses and/or u32 values in an array to be accessed later
		// causes strange issues where trying to pull them results in 0x00000000
		// rather than the actual array element's stored value
				
		/*
		for (int iterator = 0; iterator < 6; ++iterator) {
			Process::ReadFloat(addresses[iterator], currStrafingSpeeds[iterator]);
		}
		*/
		
		// this impl works, but isn't as concise as the loop:
		Process::ReadFloat(AddressList::BowStrafe.addr, bowStrafe);
		Process::ReadFloat(AddressList::FireGloveStrafe.addr, fireStrafe);
		Process::ReadFloat(AddressList::WaterRodStrafe.addr, waterStrafe);
		Process::ReadFloat(AddressList::GustStrafe.addr, gustStrafe);
		Process::ReadFloat(AddressList::RoboStrafe.addr, roboStrafe);
		Process::ReadFloat(AddressList::HammerStrafe.addr, hammerStrafe);
	}

	void resetAllStrafingSpeeds(void) {
		Process::WriteFloat(AddressList::BowStrafe.addr, 1.0);
		Process::WriteFloat(AddressList::FireGloveStrafe.addr, 1.0);
		Process::WriteFloat(AddressList::WaterRodStrafe.addr, 1.0);
		Process::WriteFloat(AddressList::GustStrafe.addr, 1.0);
		Process::WriteFloat(AddressList::RoboStrafe.addr, 1.0);
		Process::WriteFloat(AddressList::HammerStrafe.addr, 1.0);
	}

	void Item::strafingSpeedSet(MenuEntry* entry) {
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
		msg.append("\n\nCurrent speeds:");
		msg.append("\nBow: " + std::to_string(bowStrafe));
		msg.append("\nFire Gloves: " + std::to_string(fireStrafe));
		msg.append("\nWater Rod: " + std::to_string(waterStrafe));
		msg.append("\nGust Jar: " + std::to_string(gustStrafe));
		msg.append("\nGripshot: " + std::to_string(roboStrafe));
		msg.append("\nHammer: " + std::to_string(hammerStrafe));

		// Old impl (0-5, per item)
		/* msg.append("\nBow: " + std::to_string(currStrafingSpeeds[0])); */

		Keyboard speed(msg);
		speed.Populate(items);
		int choice = speed.Open();

		// Old impl:
		/*
		if (choice >= 0) {
			float result = setSpeedKB();
			Process::WriteFloat(address[choice], result);
		}
		*/

		// Alt impl... again not very concise
		float result = 1.0;

		if (choice > 0)
			result = setSpeedKB();

		switch (choice) {
		case 0:
			resetAllStrafingSpeeds();
			break;
		case 1: 
			Process::WriteFloat(AddressList::BowStrafe.addr, result);
			break;
		case 2:
			Process::WriteFloat(AddressList::FireGloveStrafe.addr, result);
			break;
		case 3:
			Process::WriteFloat(AddressList::WaterRodStrafe.addr, result);
			break;
		case 4:
			Process::WriteFloat(AddressList::GustStrafe.addr, result);
			break;
		case 5:
			Process::WriteFloat(AddressList::RoboStrafe.addr, result);
			break;
		case 6:
			Process::WriteFloat(AddressList::HammerStrafe.addr, result);
			break;
		default: 
			break;
		}
	}

	float setSpeedKB(void) {
		float result;

		Keyboard speedKB("Set strafing speed:");
		speedKB.IsHexadecimal(false);
		
		if (speedKB.Open(result) == 0) {
			if (result < 0.0)
				MessageBox(Color::Gainsboro << "Error", "Strafing speed cannot be negative.")();
			else
				return result;
		}

		return 1.0;
	}

	void Item::upgradeItemAlways(MenuEntry* entry) {
		// TODO: 
		// 1) finish documenting costume attributes from executable
		// 2) simple memory writes -> trigger upgrades for ALL items 
	}
}