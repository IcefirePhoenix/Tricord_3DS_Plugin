#ifndef BGM_SFX_HPP
#define BGM_SFX_HPP

#include "CTRPluginFramework.hpp"
#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Unicode.h"

namespace CTRPluginFramework {
    extern MenuEntry* lobbyBallAuto;

    class BGM_SFX {
    public:

        static void bgmSet(MenuEntry* entry);
        static void masterVolSet(MenuEntry* entry);
        static void bgmVolSet(MenuEntry* entry);
        static void lobbyBallSong(MenuEntry* entry);
        static void storeSelSong(u32 song);
        static u32 getSelSong(void);
        static void writeLobbyBallSel(MenuEntry* entry);
        static int selLobbyBallSong(void);
        static void voiceVol(MenuEntry* entry);
        static void lowHPVol(MenuEntry* entry);
        static std::string getPercentAsStr(int selection);
        static int selVolPreset(void);
        static float getFloatFromPercentSel(int selection);
    };
}
#endif