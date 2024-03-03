#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include "CTRPluginFramework.hpp"

namespace CTRPluginFramework {
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

		static void InitMemoryRange(void);
	};
}
#endif