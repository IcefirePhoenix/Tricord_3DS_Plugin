#include "Helpers/DataHelpers/Address.hpp"
#include "CTRPluginFramework/System/Process.hpp"

namespace CTRPluginFramework {
	Address::Address(u32 usa, u32 eur, u32 jpn, u32 kor) {
		switch (Process::GetTitleID()) {

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

	Address::Address(u32 address) {
		addr = address;
	};

	Address::Address(void) {
		addr = 0;
	};

}