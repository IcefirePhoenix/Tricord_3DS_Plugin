#ifndef SAVE_HPP
#define SAVE_HPP

#include "CTRPluginFramework.hpp"
#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Unicode.h"

namespace CTRPluginFramework {
    extern MenuEntry* merchantA;
    extern MenuEntry* merchantB;
    extern MenuEntry* merchantC;
    extern MenuEntry* merchantD;
    extern MenuEntry* merchantE;

    std::string openMerchantMatMenu(u8 slotNumber);
    int selPlayMode(void);

    class Save {
    public:
        static void useVoiceless(MenuEntry* entry);
        static void mainVoice(MenuEntry* entry);
        static void heroPointCountSet(MenuEntry* entry);
        static void coliseumWinCountSet(MenuEntry* entry);
        static void selMerchantSlot(MenuEntry* entry);
        static void resetMerchant(MenuEntry* entry);
        static void selLevelCompletion(MenuEntry* entry);
    };
}
#endif