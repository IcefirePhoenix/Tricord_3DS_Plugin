#ifndef ICONMNGR_HPP
#define ICONMNGR

#include "CTRPF.hpp"

namespace CTRPluginFramework
{
    enum class CompressionFormat {
        B4G4R4A4_UNORM = 0x4,
        LA8 = 0x5,
        LA4 = 0x9,
        ETC1_A4 = 0xD
    };

    enum class IconPtrSet {
        SealCommonCTPK = 0x0,
        SealRegionalCTPK,
        MaterialCTPK,
        ItemCTPK,
        ChallengeCTPK,
        WeaponCTPK
    };

    class IconMngr
    {
    public:
        static bool overrideGraphic(u32 iconBaseAddr, u32 replacementIconTexPtr, u8 height, u8 width, CompressionFormat format);
        static bool setGraphicOrientation(u32 iconAddr, u8 orientation);
        static bool toggleVisibility(u32 iconBaseAddr, bool visible);
        static u32 retrieveTexPtr(IconPtrSet setName, int index, u16 texSize);
        static u32 retrieveTexPtr(std::string iconName);
    };
}
#endif