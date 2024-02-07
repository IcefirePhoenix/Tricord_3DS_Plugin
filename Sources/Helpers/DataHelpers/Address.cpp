#include "Helpers/DataHelpers/Address.hpp"
#include "CTRPluginFramework/System/Process.hpp"

namespace CTRPluginFramework {
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
				addr = 0;
				OSD::Notify("The current title is not a supported, retail version of TFH!", Color::Red);
				OSD::Notify("Supported TFH regions include NA, EU/AUS, JP, and KOR.", Color::Red);
				OSD::Notify("Addresses cannot be initialized.", Color::Red);
				break;
		}
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

	// TODO: review this
	void Address::InitMemoryRange(void)
	{
		// Address startAddress (0x30000000);
		// u32 dataRange = 0x10000000;

		// _memOffset = Process::CheckRegion(startAddress.addr, dataRange) ? 0x0 : 0x14000000;
	}
}