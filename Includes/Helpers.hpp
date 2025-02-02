#pragma once

#include <3ds.h>

#include "General/Unicode.h"
#include "General/char16.h"
#include "General/csvc.h"
#include "General/json.hpp"
#include "General/types.h"

#include "Helpers/GeneralHelpers.hpp"

#include "Helpers/MenuHelpers/DescriptionUtils.hpp"
#include "Helpers/MenuHelpers/FolderEntries.hpp"

#include "Helpers/BuiltIn/HoldKey.hpp"
#include "Helpers/BuiltIn/KeySequence.hpp"
#include "Helpers/BuiltIn/MenuEntryHelpers.hpp"
#include "Helpers/BuiltIn/OSDManager.hpp"
#include "Helpers/BuiltIn/QuickMenu.hpp"
#include "Helpers/BuiltIn/Strings.hpp"
#include "Helpers/BuiltIn/Wrappers.hpp"

#include "Helpers/Interfaces/FaceSelMenu.hpp"
#include "Helpers/Interfaces/LevelStatusEditor.hpp"

#include "Helpers/DataHelpers/Address.hpp"
#include "Helpers/DataHelpers/Collision.hpp"
#include "Helpers/DataHelpers/GameData.hpp"
#include "Helpers/DataHelpers/Level.hpp"
#include "Helpers/DataHelpers/Material.hpp"
#include "Helpers/DataHelpers/PlayerAnimation.hpp"

#include "AddressList.hpp"

#include <array>
#include <bitset>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace CTRPluginFramework
{
	using StringVector = std::vector<std::string>;
}


