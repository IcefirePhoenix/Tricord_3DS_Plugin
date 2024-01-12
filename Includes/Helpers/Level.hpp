#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "CTRPluginFramework.hpp"

namespace CTRPluginFramework {
	class Level {
	public:
		Level(u8 ID, std::string externalName, std::string internalName, bool dummyStatus);

		static const Level levelList[];

		static u8 getIDFromName(const std::string& name);
		static u8 getCurrLevel(void);
		static u8 getCurrStage(void);
		static u32 getElapsedTime(void);

		static bool isInDrablands(void);
		static bool hasStageBegan(void);

	private:
		u8				_levelID;
		std::string		_extName;
		std::string		_intName;
		bool			_isDummy;
	};
}
#endif