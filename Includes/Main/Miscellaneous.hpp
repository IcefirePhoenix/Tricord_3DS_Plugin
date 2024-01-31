#ifndef MISCELLANEOUS_HPP
#define MISCELLANEOUS_HPP

#include "CTRPluginFramework.hpp"
#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Unicode.h"

namespace CTRPluginFramework {
    extern MenuEntry* autoBeamCooldown;
    extern MenuEntry* instantTextDisplay;

    void showPhotoBtnIntroMsg(bool showMsg);
    void displayPhoto(bool photoCheck);

    class Miscellaneous {
    public:
        static void buttonSpammer(MenuEntry* entry);
        static void managePhotoDisp(MenuEntry* entry);
        static void instantText(MenuEntry* entry);
        static void selectLinkBeam(MenuEntry* entry);
        static void setBeamCooldown(MenuEntry* entry);
    };
}
#endif