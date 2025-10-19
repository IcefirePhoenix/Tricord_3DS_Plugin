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
		static bool registerNewAddress(std::string addrLabel, const std::array<u32, 3> &addr);

		static std::map<std::string, u32> addresses;
	};
}
