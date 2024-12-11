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

    const u16 BUTTON_A = 1 << 0;
    const u16 BUTTON_B = 1 << 1;
    const u16 BUTTON_X = 1 << 2;
    const u16 BUTTON_Y = 1 << 3;
    const u16 BUTTON_L = 1 << 4;
    const u16 BUTTON_R = 1 << 5;
    const u16 BUTTON_STARTSEL = 1 << 6;
    const u16 BUTTON_TOUCH = 1 << 7;

    const Key keys[] =
    {
        Key::A,
        Key::B,
        Key::X,
        Key::Y,
        Key::L,
        Key::R,
        Key::Start,
        Key::Select,
        Key::Touchpad
    };

    class Miscellaneous {
    public:
        static void buttonSpammer(MenuEntry* entry);
        static void selSpamInterval(MenuEntry *entry);
        static void selectSpamKeys(MenuEntry *entry);
        static void managePhotoDisp(MenuEntry *entry);
        static void manageInstantText(MenuEntry* entry);
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