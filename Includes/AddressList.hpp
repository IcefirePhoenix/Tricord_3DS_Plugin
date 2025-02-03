#pragma once

#include "Helpers.hpp"

namespace CTRPluginFramework
{
	class AddressList
	{
	public:
		static void InitAddresses(void);
		static void InitMemoryRange(void);
		static void cancelAddrInit(void);
		static u32 getAddress(std::string addrName);

		static std::map<std::string, u32> addresses;
	};
}
