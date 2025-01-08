#ifndef RENDERING_HPP
#define RENDERING_HPP

#include "CTRPF.hpp"
#include "Cheats.hpp"
#include "Helpers.hpp"
#include "Unicode.h"

namespace CTRPluginFramework
{
    void hideHUD(void);

    class Rendering
    {
    public:
        static void triggerHideHUD(MenuEntry* entry);
        static void disableFog(MenuEntry* entry);
        static void disableScrollingText(MenuEntry* entry);
        static void respawnIndicator(MenuEntry* entry);
        static void seeChestContents(MenuEntry* entry);
        static void swapLinkTexture(MenuEntry* entry);
        static void swapSPLoadingScreen(MenuEntry* entry);
        static void editLiveMsgColor(MenuEntry* entry);
        static void forcePomPom(MenuEntry* entry);
        static void forceAura(MenuEntry* entry);
        static void forceBlueSwordParticles(MenuEntry* entry);
        static void disablePlayerLight(MenuEntry* entry);
        static void editFaceExpr(MenuEntry *entry);
        static void forceCustomFaceExpr(MenuEntry *entry);
    };
}
#endif