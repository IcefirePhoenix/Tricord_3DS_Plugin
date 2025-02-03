#include "Address_Collection.json.h"
#include "Helpers.hpp"

namespace CTRPluginFramework
{
	u32 memOffset = 0x0;
	std::map<std::string, u32> AddressList::addresses;

	/* ------------------ */

	// Initializes the address list
	void AddressList::InitAddresses(void)
	{
		std::string addressFile(reinterpret_cast<char *>(Address_Collection_json), Address_Collection_json_len);
		nlohmann::json JSON_content = nlohmann::json::parse(addressFile);

		// TODO: not the best -- we can't put hex values in JSON so they are currently strs to allow us to skip converting to int... check for alternative implementations?
		std::string addrStr = "";
		u32 addrVal = 0x0;

		if (JSON_content.empty())
		{
			OSD::Notify("ERROR: Address list cannot be loaded.", Color::Red);
			AddressList::cancelAddrInit();
		}
		else
		{
			// parse JSON...
			for (const auto &addressEntry : JSON_content["addresses"])
			{
				std::string addrReference = addressEntry["name"];
				const auto &addrValues = addressEntry["regionAddrs"];

				if (addrValues.size() == 3)
				{
					switch (Process::GetTitleID())
					{
						case TID_USA:
							addrStr = addrValues["usaAddress"].get<std::string>();
							break;

						case TID_EUR:
							addrStr = addrValues["eurAddress"].get<std::string>();
							break;

						case TID_JPN:
							addrStr = addrValues["jpnAddress"].get<std::string>();
							break;

						default:
							OSD::Notify("The current title is not a supported retail version of TFH!", Color::Red);
							OSD::Notify("Supported TFH regions include NA, EU/AUS, and JPN.", Color::Red);
							AddressList::cancelAddrInit();
					}

					// remove 0x prefix and convert to numerical val...
					if (addrStr.substr(0, 2) == "0x")
						addrStr = addrStr.substr(2);


					addrVal = std::stoi(addrStr, nullptr, 16);

					// adjust all addresses within LINEAR mem range...
					if (addrVal >= 0x14000000)
						addrVal -= memOffset;

					// create new address reference...
					AddressList::addresses[addrReference] = addrVal;
				}
				else
				{
					OSD::Notify("[ERROR] Invalid address data for " << addrReference << ".", Color::Yellow);
					AddressList::cancelAddrInit();
				}
			}
		}
	}

	// Adjusts the working memory range depending on the current memory allocation scheme
	void AddressList::InitMemoryRange(void)
	{
		u32 startAddress = 0x30000000;
		u32 temp;

		memOffset = Process::CheckRegion(startAddress, temp) ? 0x0 : 0x1C000000;

		if (memOffset == 0x1C000000)
			OSD::Notify("Alternate address set loaded.");
	}

	// Force-terminates the plugin
	void AddressList::cancelAddrInit(void)
	{
		OSD::Notify("Addresses cannot be initialized. Plugin will now abort.", Color::Red);
		svcSleepThread(150);
		abort();
	}

	// Retrieves address value via reference key
	u32 AddressList::getAddress(std::string addrName)
	{
		u32 addr = AddressList::addresses[addrName];

		if (addr == 0x0)
			OSD::Notify("[ERROR] Address reference cannot be found for " << addrName << ".", Color::Red);

		return addr;
	}
}