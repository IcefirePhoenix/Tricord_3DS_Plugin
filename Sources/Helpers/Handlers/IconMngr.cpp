#include "Helpers.hpp"

namespace CTRPluginFramework
{
    // Refer to the Data Documentation Spreadsheet for details on icon data.
    u16 rotationReflectionOffset = 0x136;
    u16 dimensionsOffset = 0x168;
    u16 compressionFormatOffset = 0x16C;
    u16 texOffset = 0x15C;
    u8 visibilityOffset = 0x30;

    std::array<std::array<u32, 3>, 2> iconPtrSetStartAddrs = {{
        {0x20C81080, 0x20D16480, 0x20C31680},    // SealCommonCTPK
        {0x20300780, 0x208E2080, 0x20329C80}     // SealRegionalCTPK
    }};

    std::unordered_map<std::string, std::array<u32, 3>> standalonePtrs = {
        {"RoyalS_TMark00",  {0x2113FF80, 0x211D0180, 0x210D8B00}},
        {"StaffMark00", {0x21119280, 0x211A9480, 0x210B1E00}}
    };

    /* ------------------ */

    // Replace an overlay icon with another graphic
    bool IconMngr::overrideGraphic(u32 iconBaseAddr, u32 replacementIconTexPtr, u8 height, u8 width, CompressionFormat format)
    {
        Process::Write32(iconBaseAddr + texOffset, replacementIconTexPtr);

        // concatentate height and width
        u32 dimensions = (u32)height;
        dimensions = dimensions << 16;
        dimensions = dimensions | (u32)width;

        Process::Write32(iconBaseAddr + dimensionsOffset, dimensions);
        Process::Write8(iconBaseAddr + compressionFormatOffset, (u8)format);
        return true;
    }

    // Flips graphics, which may be required for icon swaps as textures may not be stored in the matching orientation in the RomFS
    // Orientations haven't been fully reverse-engineered; only use with known values!
    bool IconMngr::setGraphicOrientation(u32 iconBaseAddr, u8 orientation)
    {
        return Process::Write8(iconBaseAddr + rotationReflectionOffset, orientation);
    }

    // Helper function to derive a specified texture pointer, ONLY if derivable from calculation
    u32 IconMngr::retrieveTexPtr(IconPtrSet setName, int index, u16 texSize)
    {
        u32 baseAddr = 0x0;

        const std::array<u32, 3> filePtrs = iconPtrSetStartAddrs[(int)setName];
        switch (Process::GetTitleID())
        {
            case TID_USA:
                baseAddr = filePtrs[0];
                break;
            case TID_EUR:
                baseAddr = filePtrs[1];
                break;
            case TID_JPN:
                baseAddr = filePtrs[2];
                break;
            default:
                OSD::Notify("Icon Manager: Cannot determine region.", Color::Red);
                return 0x0;
        }

        // Indexes and calculation logic are documented in the Data Documentation Spreadsheet
        return baseAddr + ((index) * texSize);
    }

    // Retrieve a standalone texture pointer (that cannot be derived via calculations)
    u32 IconMngr::retrieveTexPtr(std::string iconName)
    {
        auto it = standalonePtrs.find(iconName);
        if (it != standalonePtrs.end())
        {
            const std::array<u32, 3> filePtrs = it->second;
            switch (Process::GetTitleID())
            {
                case TID_USA:
                    return filePtrs[0];
                case TID_EUR:
                    return filePtrs[1];
                case TID_JPN:
                    return filePtrs[2];
                default:
                    OSD::Notify("Icon Manager: Cannot determine region.", Color::Red);
                    return 0x0;
            }
        }
        else
            return 0x0;
    }

    bool IconMngr::toggleVisibility(u32 iconBaseAddr, bool visible)
    {
        return Process::Write8(iconBaseAddr + visibilityOffset, visible);
    }
}
