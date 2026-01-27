#ifndef RENDERING_HPP
#define RENDERING_HPP

#include "CTRPF.hpp"
#include "Cheats.hpp"
#include "Helpers.hpp"
#include "Unicode.h"

namespace CTRPluginFramework
{
    extern MenuEntry* faceExprManager;
    extern MenuEntry* starMarkManager;
    extern MenuEntry* starMarkInit;

    extern MenuEntryLabel *colorLabel1;
    extern MenuEntryLabel *colorLabel2;
    extern MenuEntryLabel *colorLabel3;

    extern MenuEntryLabel *faceExprLabel1;
    extern MenuEntryLabel *faceExprLabel2;
    extern MenuEntryLabel *faceExprLabel3;
    extern MenuEntryLabel *faceExprLabel4;
    extern MenuEntryLabel *faceExprLabel5;
    extern MenuEntryLabel *faceExprLabel6;

    void hideHUD(void);

    class Rendering
    {
    public:
        static void initNameColorAddresses(void);
        static void triggerHideHUD(MenuEntry* entry);
        static void disableFog(MenuEntry* entry);
        static void disableScrollingText(MenuEntry* entry);
        static void respawnIndicator(MenuEntry* entry);
        static void seeChestContents(MenuEntry* entry);
        static void swapLinkTexture(MenuEntry* entry);
        static void swapSPLoadingScreen(MenuEntry* entry);
        static void editLiveMsgColor(MenuEntry* entry);
        static void loadCustomNameColors(void);
        static void restoreNameColors(MenuEntry *entry);
        static void forcePomPom(MenuEntry* entry);
        static void forceAura(MenuEntry* entry);
        static void forceBlueSwordParticles(MenuEntry* entry);
        static void disablePlayerLight(MenuEntry* entry);
        static void editFaceExpr(MenuEntry *entry);
        static void hideDoppelMasks(MenuEntry* entry);
        static void setDevID_onConnection(MenuEntry *entry);
        static void checkDevID(MenuEntry *entry);
        static void initHeroPanelBasePtrs(void);
    };
}
#endif