#include "Helpers/Address.hpp"
#include "CTRPluginFramework/System/Process.hpp"


namespace CTRPluginFramework {
	//CurrRegion c_Region = Process::GetTitleID;
	//std::string regionName = "";


	Address::Address(u32 usa, u32 eur, u32 jpn, u32 kor) {
		switch (Process::GetTitleID()) {

			case 0x0004000000176F00:
				addr = usa; 
				break;

			case 0x0004000000177000:
				addr = eur; 
				break;

			case 0x0004000000176E00:
				addr = jpn; 
				break;

			case 0x0004000000182B00:
				addr = kor; 
				break;

			default: 
				addr = 0;
				OSD::Notify("The current title is not a retail version of TFH!", Color::Red);
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