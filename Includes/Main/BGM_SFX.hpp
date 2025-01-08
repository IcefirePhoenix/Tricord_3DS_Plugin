#ifndef BGM_SFX_HPP
#define BGM_SFX_HPP

#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    extern MenuEntry* masterVol;
    extern MenuEntry* BGMVol;
    extern MenuEntry* voiceVol;
    extern MenuEntry* lowHPVol;
    extern MenuEntry* emoteVol;
    extern MenuEntry* lobbyBallAuto;

    class BGM_SFX
    {
    public:
        static void bgmSet(MenuEntry* entry);
        static void volSet(MenuEntry* entry);

        static void lobbyBallSong(MenuEntry* entry);
        static void writeLobbyBallSel(MenuEntry* entry);
        static int selLobbyBallSong(void);

        static std::string getPercentAsStr(int selection);
        static int selVolPreset(void);
        static float getFloatFromPercentSel(int selection);
        static void forceNormal8bit(MenuEntry* entry);
        static void disable8bitVoice(MenuEntry* entry);
        static void force8bitSword(MenuEntry* entry);
    };
}
#endif