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
	extern const StringVector voiceList;
	extern const StringVector worldList;

	extern const StringVector otherMats;
	extern const StringVector woodlandMats;
	extern const StringVector riversideMats;
	extern const StringVector volcanoMats;
	extern const StringVector iceMats;
	extern const StringVector fortressMats;
	extern const StringVector dunesMats;	
	extern const StringVector ruinsMats;
	extern const StringVector skyMats;

	u32 chooseLink(void);
	u8 chooseSword(void);

	bool showMsgKbd(std::string text, DialogType digtype);

	int selectMaterialIndiv(int selectedWorld);
	int selectMaterialWorld(void);
}
#endif