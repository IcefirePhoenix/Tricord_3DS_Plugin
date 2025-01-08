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

    void shiftCamNorth(void);
    void shiftCamSouth(void);
    void shiftCamEast(void);
    void shiftCamWest(void);
    void zoomCamIn(void);
    void zoomCamOut(void);
    void raiseCam(void);
    void lowerCam(void);
    void rotateCamXCounter(void);
    void rotateCamXClockwise(void);
    void rotateCamZCounter(void);
    void rotateCamZClockwise(void);

    void lockCamera(void);
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