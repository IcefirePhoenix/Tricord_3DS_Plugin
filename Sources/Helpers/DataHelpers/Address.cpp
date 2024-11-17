#include "Helpers/DataHelpers/Address.hpp"
#include "Helpers/GeneralHelpers.hpp"
#include "CTRPluginFramework/System/Process.hpp"

namespace CTRPluginFramework {
	u32 _memOffset = 0x0;

	Address::Address(u32 usa, u32 eur, u32 jpn, u32 kor)
	{
		switch (Process::GetTitleID())
		{
			case TID_USA:
				addr = usa;
				break;

			case TID_EUR:
				addr = eur;
				break;

			case TID_JPN:
				addr = jpn;
				break;

			case TID_KOR:
				addr = kor;
				break;

			default:
				OSD::Notify("The current title is not a supported retail version of TFH!", Color::Red);
				OSD::Notify("Supported TFH regions include NA, EU/AUS, JP, and KOR.", Color::Red);
				OSD::Notify("Addresses cannot be initialized. Plugin will now abort.", Color::Red);

				svcSleepThread(150);
				abort();	// TODO: not sure?
		}

		if (addr >= 0x14000000) addr -= _memOffset;
	}

	Address::Address(u32 address)
	{
		addr = address;
	};

	// this might need to be removed
	// null addresses aren't needed for Tricord
	Address::Address(void)
	{
		addr = 0;
	};

	void Address::InitMemoryRange(void)
	{
		u32 startAddress = 0x30000000;
		u32 temp;

		_memOffset = Process::CheckRegion(startAddress, temp) ? 0x0 : 0x1C000000;

		if (_memOffset == 0x1C000000)
			OSD::Notify("Alternate address set loaded.");
	}
}