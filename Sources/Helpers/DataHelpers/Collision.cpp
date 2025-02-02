#include "Helpers.hpp"

namespace CTRPluginFramework
{
	Collision::Collision(u16 ID, std::string colName) :
		_ID(ID), _colType(colName)
	{
	}

	const Collision Collision::colList[8] = {
		Collision(0xA, "Fall_plane"),
        Collision(0x121, "Ice"),
		Collision(0x167, "Lava"),
		Collision(0x187, "Water"),
		Collision(0xC1, "Quicksand_plane"),
		Collision(0xC7, "Quicksand_stream"),
		Collision(0x364, "Colored_platform"),
		Collision(0x1F, "Air")
    };

    u16 Collision::colIDFromName(std::string name)
	{
		for (int iterator = 0; iterator < 10; iterator++)
		{
			if (colList[iterator]._colType == name)
				return colList[iterator]._ID;
		}
		return 0xFFFF; // wasn't found
	}

    u16 Collision::getCurrCol(int player)
	{
		u16 colID;
        u32 addressOffset = player * PLAYER_OFFSET;

		Process::Read16(AddressList::CollisionCurrent.addr + addressOffset, colID);
		return colID;
	}

    void Collision::setCurrCol(int player, u16 targetCol)
	{
		u32 addressOffset = player * PLAYER_OFFSET;
		Process::Write16(AddressList::CollisionCurrent.addr + addressOffset, targetCol);
	}
}