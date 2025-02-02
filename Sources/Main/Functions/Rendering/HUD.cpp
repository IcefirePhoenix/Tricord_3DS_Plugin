#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    bool showChestContents, isScrollTextDisabled = false;

    /* ------------------ */

    // TODO: check -- is this impl necessary?
    // Wrapper for MenuEntry compatiblity; toggles HUD status
    void Rendering::triggerHideHUD(MenuEntry *entry)
    {
        hideHUD();
    }

    // Forces HUD to remain in its dormant animation frame, forcing it off-screen
    void hideHUD(void)
    {
        u8 disableOffScrArrow = 0x10;
        u32 mainHUDoffset = 0x18;
        u32 ActionButtonBGOffset = 0x75D0;
        u32 ActionButtonTextOffset = 0x78B0;
        u32 HUDPointerCheck;

        Process::Read32(AddressList::HUDPointer.addr, HUDPointerCheck);

        if (!GeneralHelpers::isNullPointer(HUDPointerCheck))
        {
            Process::Write32((HUDPointerCheck + mainHUDoffset), 0x7F000004);
            Process::Write32((HUDPointerCheck + ActionButtonBGOffset), 0x00000010);
            Process::Write32((HUDPointerCheck + ActionButtonTextOffset), 0x00FFFF00);
        }

        Process::Write8(AddressList::OffScreenLocation.addr, disableOffScrArrow);
    }

    // Forces top-screen scrolling event text to be transparent
    void Rendering::disableScrollingText(MenuEntry *entry)
    {
        isScrollTextDisabled = !isScrollTextDisabled;

        if (isScrollTextDisabled)
        {
            Process::WriteFloat(AddressList::ScrollingTextOpacity.addr, 0.0);
            entry->SetName("Enable top screen scrolling text");
        }
        else
        {
            Process::WriteFloat(AddressList::ScrollingTextOpacity.addr, 1.0);
            entry->SetName("Disable top screen scrolling text");
        }
    }

    // TODO: save changes to disk for persistent edits
    // Allows player names' colors to be edited within top-screen scrolling event text
    void Rendering::editLiveMsgColor(MenuEntry *entry)
    {
        u32 targetaddr, result, newColor;

        switch (GeneralHelpers::chooseLink())
        {
            case 0:
                targetaddr = AddressList::LiveNameColorG.addr;
                break;
            case 1:
                targetaddr = AddressList::LiveNameColorB.addr;
                break;
            case 2:
                targetaddr = AddressList::LiveNameColorR.addr;
                break;
            default:
                return;
        }

        Keyboard HexColor("Enter a 6-digit RGB hex code:");
        HexColor.IsHexadecimal(true);

        if (HexColor.Open(result) == 0)
        {
            u8 r = result >> 16;
            u8 g = result >> 8;
            u8 b = result;

            newColor = r + (g << 8) + (b << 16) + (0xFF << 24);
            Process::Write32(targetaddr, newColor);
        }
    }

    // Display custom player respawn indicator arrows
    void Rendering::respawnIndicator(MenuEntry *entry)
    {
        float respawnCoords[3][3] =
        {
            {0.0, 0.0, 0.0}, // green
            {0.0, 0.0, 0.0}, // blue
            {0.0, 0.0, 0.0}  // red
        };

        u32 arrowAddresses[3] =
        {
            AddressList::ArrowIndicatorColorTop.addr,
            AddressList::ArrowIndicatorColorMid.addr,
            AddressList::ArrowIndicatorColorBot.addr
        };

        u8 visibilityOffset = 0x4, indivCoordinateOffset = 0x4;
        u8 coordinateOffsetStart = 0x3C;
        u32 forceVisibility = 0x10FFFF00;

        // grab respawn coords...
        for (int iterateColor = 0; iterateColor < 3; iterateColor++)
        {
            u32 offset = iterateColor * PLAYER_OFFSET;
            Process::ReadFloat((AddressList::RespawnPositionX.addr + offset), respawnCoords[iterateColor][0]);
            Process::ReadFloat((AddressList::RespawnPositionY.addr + offset), respawnCoords[iterateColor][1]);
            Process::ReadFloat((AddressList::RespawnPositionZ.addr + offset), respawnCoords[iterateColor][2]);
        }

        for (int iterateEdits = 0; iterateEdits < 3; iterateEdits++)
        {
            Process::Write8(arrowAddresses[iterateEdits], GameData::generalPlayerIDs[iterateEdits]); // set arrow color
            Process::Write32(arrowAddresses[iterateEdits] + visibilityOffset, forceVisibility);      // force arrow visibility
        }

        for (int iterateArray = 0; iterateArray < 3; iterateArray++)
        {
            Process::WriteFloat((arrowAddresses[0] + (iterateArray * 0x4) - coordinateOffsetStart), respawnCoords[0][iterateArray]);
            Process::WriteFloat((arrowAddresses[1] + (iterateArray * 0x4) - coordinateOffsetStart), respawnCoords[1][iterateArray]);
            Process::WriteFloat((arrowAddresses[2] + (iterateArray * 0x4) - coordinateOffsetStart), respawnCoords[2][iterateArray]);
        }
    }

    // Force visibility of chest contents -> Linebeck's Uniform costume ability
    void Rendering::seeChestContents(MenuEntry *entry)
    {
        showChestContents = !showChestContents;

        if (showChestContents)
        {
            Process::Patch(AddressList::SeeChestContents.addr, 0xEA000017);
            entry->SetName("Disable forced-visibility of Treasure Chest contents");
        }
        else
        {
            Process::Patch(AddressList::SeeChestContents.addr, 0x0A000017);
            entry->SetName("Force visibility of Treasure Chest contents");
        }
    }
}