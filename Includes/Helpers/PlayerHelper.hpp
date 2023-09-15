#ifndef PLAYERHELPER_HPP
#define PLAYERHELPER_HPP

#include <CTRPluginFramework.hpp>
#include "Helpers.hpp"
#include "Unicode.h"
#include "AddressList.hpp"
#include <3ds.h>

namespace CTRPluginFramework {
	extern const StringVector universalCostumeList;
	extern const StringVector customCostumeList;

	int chooseLink(void);

	bool showMsgKbd(std::string text, DialogType digtype);
}
#endif