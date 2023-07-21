#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include <CTRPluginFramework.hpp>

#define TID_USA 		0x0004000000176F00
#define TID_EUR 		0x0004000000177000
#define TID_JPN 		0x0004000000176E00
#define TID_KOR 		0x0004000000182B00

namespace CTRPluginFramework {
	enum CurrRegion {
		INVALID = 0,
		USA,
		EUR,
		JPN,
		KOR,
	};

	extern CurrRegion c_Region;

	extern std::string regionName;

	class Address {
	public:
		Address(u32 usa, u32 eur, u32 jpn, u32 kor);

		Address(u32 address);
		
		Address(void);

		u32 addr;

		template <typename T, class ...Args>
		T Call(Args ...args) {
			return((T(*)(Args...))(addr))(args...);
		};
	
	};
}
#endif