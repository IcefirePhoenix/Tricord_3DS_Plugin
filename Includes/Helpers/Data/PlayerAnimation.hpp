#ifndef PLAYERANIMATION_HPP
#define PLAYERANIMATION_HPP

#include "CTRPF.hpp"

namespace CTRPluginFramework
{
	class PlayerAnimation
	{
	public:
		PlayerAnimation (u8 ID, std::string name);

		static const PlayerAnimation animList[];

		static u8 getIDFromName(const std::string& name);
		static u8 getAnim(int playerID, bool getPrevAnim);

	private:
		u8				_animID;
		std::string		_animName;
	};
}
#endif