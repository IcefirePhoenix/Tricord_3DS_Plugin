#ifndef MISCELLANEOUS_HPP
#define MISCELLANEOUS_HPP

#include "CTRPluginFramework.hpp"
#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Unicode.h"

namespace CTRPluginFramework {
    extern MenuEntry* autoBeamCooldown;
    extern MenuEntry* instantTextDisplay;
    extern MenuEntry* autoWriteCameraStatus;
    extern MenuEntry* autoDisableCamShutter;

    void showPhotoBtnIntroMsg(bool showMsg);
    void displayPhoto(bool photoCheck);
    void setCameraEdits(bool useCamera);
    bool getCameraEdits(void);

    class Miscellaneous {
    public:
        static void buttonSpammer(MenuEntry* entry);
        static void managePhotoDisp(MenuEntry* entry);
        static void instantText(MenuEntry* entry);
        static void selectLinkBeam(MenuEntry* entry);
        static void setBeamCooldown(MenuEntry* entry);
        static void toggleCameraButton(MenuEntry* entry);
        static void writeCameraEdits(MenuEntry* entry);
        static void setLobbyBallCounter(MenuEntry* entry);
        static void toggleCameraShutter (MenuEntry* entry);
        static void writeShutterDisable(MenuEntry* entry);
    };
}
#endif