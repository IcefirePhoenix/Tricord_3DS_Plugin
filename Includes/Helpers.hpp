#pragma once

#include "AddressList.hpp"

#include "Helpers/GeneralHelpers.hpp"
#include "Helpers/MenuHelper.hpp"

#include "General/char16.h"
#include "General/csvc.h"
#include "General/types.h"
#include "General/Unicode.h"

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

#include "csvc.h"
#include "3ds.h"

#include <vector>
#include <functional>
#include <string>
#include <array>
#include <bitset>

namespace CTRPluginFramework
{
	using StringVector = std::vector<std::string>;
}


