#include "Helpers.hpp"


namespace CTRPluginFramework {

	PlayerAnimation::PlayerAnimation(u8 ID, std::string name) :
		_animID(ID), _animName(name)
	{
	}

	// using the most generic name, if multiple are used by the same ID
	const PlayerAnimation PlayerAnimation::animList[30] = {
		PlayerAnimation(0x0, "Link_Wait"),
		PlayerAnimation(0x1, "Link_Walk"),
		PlayerAnimation(0x3, "Link_PDashStagger"),
		PlayerAnimation(0x4, "Link_Slipice"),
		PlayerAnimation(0x8, "Link_Rock"),
		PlayerAnimation(0x9, "Link_CutRF"),
		PlayerAnimation(0xA, "Link_CutRFtoKeep"),
		PlayerAnimation(0xB, "Link_KeepWait"),
		PlayerAnimation(0xC, "Link_DamB"),
		PlayerAnimation(0xD, "Link_CutTurn"),
		PlayerAnimation(0xE, "Link_CutTurnW"),
		PlayerAnimation(0x11, "Link_GrabP"),
		PlayerAnimation(0x13, "Link_GrabUp"),
		PlayerAnimation(0x14, "Link_GrabThrow"),
		PlayerAnimation(0x17, "Link_DamF"),
		PlayerAnimation(0x1A, "Link_Drown"),
		PlayerAnimation(0x1D, "Link_FallBallon"),
		PlayerAnimation(0x1E, "Link_DamBiri"),
		PlayerAnimation(0x21, "Link_BoomCarried"),
		PlayerAnimation(0x23, "Link_DamBoyon"),
		PlayerAnimation(0x46, "Link_Die"),
		PlayerAnimation(0x4C, "Link_ItemGetShort"),
		PlayerAnimation(0x4E, "Link_ItemGetA"),
		PlayerAnimation(0x4F, "Link_PDashReady"),
		PlayerAnimation(0x50, "Link_PDash"),
		PlayerAnimation(0x51, "Link_PDashBrake"),
		PlayerAnimation(0x52, "Link_PDashCrash"),
		PlayerAnimation(0x61, "Link_SwimWait"),
		PlayerAnimation(0x67, "Link_SwimScrew"),
		PlayerAnimation(0x6A, "Link_CarryAway")
	};

	u8 PlayerAnimation::getIDFromName(const std::string& name)
	{
		for (int iterator = 0; iterator < 30; iterator++)
		{
			if (animList[iterator]._animName == name)
				return animList[iterator]._animID;
		}
		return -1; // wasn't found
	}

	u8 PlayerAnimation::getAnim(int playerID, bool getPrevAnim)
	{
		u8 anim;
		u32 addressOffset = playerID * GameData::playerAddressOffset;
		u32 animAddress = getPrevAnim ? AddressList::PreviousAnim.addr : AddressList::CurrentAnim.addr;

		Process::Read8(animAddress + addressOffset, anim);
		return anim;
	}
}