#ifndef FREECAM_HPP
#define FREECAM_HPP

#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    extern MenuEntry* menuFreecam;
    extern MenuEntry* editFreecamControls;
    extern MenuEntry* editFreecamSen;
    extern MenuEntry* swapZoom;

    enum cameraMode
    {
        GAMEPLAY,
        DYNAMIC,
        CUTSCENE,
        CAM_ANIMATION
    };

    bool setCameraType(cameraMode mode);
    void shiftCamZ(bool south);
    void shiftCamX(bool west);
    void adjustCamZoom(bool zoomIn);
    void adjustCamHeight(bool isRaise);
    void rotateCamX(bool counterclockwise);
    void rotateCamY(bool counterclockwise);
    void adjustRotationMoveOffset(void);
    void manageDynamicCamShifts(bool reset);
    void manageY_AxisReturnShift(bool disableGradualShifts);
    void manageFreecamPlayerLock(void);
    float setSensitivity(std::string message);

    class Freecam
    {
    public:
        static void useFreecam(MenuEntry* entry);
        static void editHotkeys(MenuEntry* entry);
        static void editSensitivity(MenuEntry* entry);
        static void setZoomType(MenuEntry* entry);
    };
}
#endif