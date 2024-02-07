#include "Helpers.hpp"
#include "Main/Rendering.hpp"

namespace CTRPluginFramework
{
    bool showChestContents, isScrollTextDisabled = false;

    void Rendering::triggerHideHUD(MenuEntry* entry) 
    {
        hideHUD();
    }

    void hideHUD(void) 
    {
        u32 HUDPointerCheck;
        Process::Read32(AddressList::HUDPointer.addr, HUDPointerCheck);

        u8 disableOffScrArrow = 0x10;
        u32 mainHUDoffset = 0x18;
        u32 ActionButtonBGOffset = 0x75D0;
        u32 ActionButtonTextOffset = 0x78B0;

        if (!GeneralHelpers::isNullPointer(HUDPointerCheck)) 
        {
            Process::Write32((HUDPointerCheck + mainHUDoffset), 0x7F000004);
            Process::Write32((HUDPointerCheck + ActionButtonBGOffset), 0x00000010);
            Process::Write32((HUDPointerCheck + ActionButtonTextOffset), 0x00FFFF00);
        }

        Process::Write8(AddressList::OffScreenLocation.addr, disableOffScrArrow);
    }

    // does this reset itself? 
    void Rendering::disableFog(MenuEntry* entry) 
    {
        // eventually it would be nice to have proper labels for these addresses + values
        // once enough info is known about these addresses
         
        Process::Write32((AddressList::FogA.addr), 0xFF700FFF);
        Process::Write32((AddressList::FogB.addr), 0x453B8000);
        Process::Write32((AddressList::FogC.addr), 0x459C4000);
    }
    
    void Rendering::disableScrollingText(MenuEntry* entry) 
    {
        isScrollTextDisabled = !isScrollTextDisabled;

        if (isScrollTextDisabled) 
        {
            Process::Write32(AddressList::ScrollingTextOpacity.addr, 0x00000000);
            entry->SetName("Enable top screen scrolling text");
        }
        else 
        {
            Process::Write32(AddressList::ScrollingTextOpacity.addr, 0x3F800000);
            entry->SetName("Disable top screen scrolling text");
        }
    }

    void Rendering::respawnIndicator(MenuEntry* entry) 
    {
        // TODO: add checks? 
        float respawnCoords[3][3] = {
            {0.0, 0.0, 0.0}, // green
            {0.0, 0.0, 0.0}, // blue
            {0.0, 0.0, 0.0}  // red
        };

        u32 arrowAddresses[3] = {
            AddressList::ArrowIndicatorColorTop.addr,
            AddressList::ArrowIndicatorColorMid.addr,
            AddressList::ArrowIndicatorColorBot.addr
        };

        // grab respawn coords
        for (int iterateColor = 0; iterateColor < 3; iterateColor++) 
        {
            u32 offset = iterateColor * GameData::playerAddressOffset;
            Process::ReadFloat((AddressList::RespawnPositionX.addr + offset), respawnCoords[iterateColor][0]);
            Process::ReadFloat((AddressList::RespawnPositionY.addr + offset), respawnCoords[iterateColor][1]);
            Process::ReadFloat((AddressList::RespawnPositionZ.addr + offset), respawnCoords[iterateColor][2]);
        }

        u32 forceVisibility = 0x10FFFF00;
        u8 coordinateOffset = 0x4;

        for (int iterateEdits = 0; iterateEdits < 3; iterateEdits++) 
        {
            Process::Write8(arrowAddresses[iterateEdits], GameData::generalPlayerIDs[iterateEdits]); // set arrow color
            Process::Write32(arrowAddresses[iterateEdits], forceVisibility);                         // force arrow visibility
            Process::WriteFloat(arrowAddresses[iterateEdits] + (iterateEdits * coordinateOffset), respawnCoords[0][iterateEdits]); // draw arrow
        }
    }

    void Rendering::seeChestContents(MenuEntry* entry) 
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