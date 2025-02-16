#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "CTRPF.hpp"

namespace CTRPluginFramework
{
	class Collision
	{
	public:
		Collision(u16 ID, std::string colType);

        static const Collision colList[];

        static u16 colIDFromName(std::string name);
        static u16 getCurrCol(int player);
		static u16 getLastCol(int player);
        static void setCurrCol(int player, u16 targetCol);

	private:
		u16				_ID;
		std::string		_colType;
    };
}
#endif
