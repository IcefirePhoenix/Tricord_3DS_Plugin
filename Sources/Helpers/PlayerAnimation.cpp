#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/PlayerAnimation.hpp"
#include "Helpers/GameData.hpp"

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {

	PlayerAnimation::PlayerAnimation(u8 ID, std::string name) :
		_animID(ID), _animName(name)
	{
	}

	const PlayerAnimation PlayerAnimation::animList[1] = {
		PlayerAnimation(0x0, "")
	};

	u8 PlayerAnimation::getIDFromName(const std::string& name) {
		for (int iterator = 0; iterator < 45; ++iterator) {
			if (animList[iterator]._animName == name) {
				return animList[iterator]._animName;
			}
		}
		return -1; // wasn't found
	}

	u8 PlayerAnimation::getCurrAnim(int playerID) {
		u8 anim;
		u32 addressOffset = playerID * GameData::playerAddressOffset;

		Process::Read8(AddressList::CurrentAnim.addr + addressOffset, anim);
		return anim;
	}

	u8 PlayerAnimation::getPrevAnim(int playerID) {
		u8 anim;
		u32 addressOffset = playerID * GameData::playerAddressOffset;

		Process::Read8(AddressList::PreviousAnim.addr + addressOffset, anim);
		return anim;
	}
}