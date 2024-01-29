#ifndef PLAYERANIMATION_HPP
#define PLAYERANIMATION_HPP

#include "CTRPluginFramework.hpp"

namespace CTRPluginFramework {
	class PlayerAnimation {
	public:
		// may add a field for anim speed
		// add option for multiple animation names 
		PlayerAnimation (u8 ID, std::string name);

		static const PlayerAnimation animList[];

		static u8 getIDFromName(const std::string& name);
		static u8 getCurrAnim(int playerID);
		static u8 getPrevAnim(int player ID);

	private:
		u8				_animID;
		std::string		_animName;
	};
}
#endif