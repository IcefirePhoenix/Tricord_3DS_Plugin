#include "Helpers/Address.hpp"

namespace CTRPluginFramework {
	CurrRegion c_Region = INVALID;

	std::string regionName = "";


	Address::Address(u32 usa, u32 eur, u32 jpn, u32 kor) {
		switch (c_Region) {

			case CurrRegion::USA: addr = usa; 
				break;

			case CurrRegion::EUR: addr = eur; 
				break;

			case CurrRegion::JPN: addr = jpn; 
				break;

			case CurrRegion::KOR: addr = kor; 
				break;

			case CurrRegion::INVALID: addr = 0; 
				break;

			default: addr = 0; 
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