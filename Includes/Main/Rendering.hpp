#ifndef RENDERING_HPP
#define RENDERING_HPP

#include "CTRPluginFramework.hpp"
#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Unicode.h"

namespace CTRPluginFramework {
    void hideHUD(void);

    class Rendering {
    public:
        static void triggerHideHUD(MenuEntry* entry);
        static void disableFog(MenuEntry* entry);
        static void disableScrollingText(MenuEntry* entry);
        static void respawnIndicator(MenuEntry* entry);
        static void seeChestContents(MenuEntry* entry);
        static void swapLinkTexture(MenuEntry* entry);
        static void swapSPLoadingScreen(MenuEntry* entry);
    };
}
#endif