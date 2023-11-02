#ifndef PLAYERHELPER_HPP
#define PLAYERHELPER_HPP

#include <CTRPluginFramework.hpp>
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"

#include "Helpers.hpp"
#include "Unicode.h"
#include "AddressList.hpp"
#include <3ds.h>

namespace CTRPluginFramework {
	extern const StringVector universalCostumeList;
	extern const StringVector customCostumeList;
	extern const StringVector swordList;


	u32 chooseLink(void);
	u8 chooseSword(void);

	bool showMsgKbd(std::string text, DialogType digtype);
}
#endif