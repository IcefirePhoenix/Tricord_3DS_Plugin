#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/GeneralHelpers.hpp"

#include "3ds.h"

namespace CTRPluginFramework
{
    bool isScrollTextDisabled = false;

    void triggerHideHUD(MenuEntry* entry) {
        hideHUD();
    }

    void hideHUD(void) {
        u32 HUDPointerCheck;
        Process::Read32(AddressList::HUDPointer.addr, HUDPointerCheck);

        u8 disableOffScrArrow = 0x10;
        u32 mainHUDoffset = 0x00000018;
        u32 ActionButtonBGOffset = 0x000075D0;
        u32 ActionButtonTextOffset = 0x000078B0;

        if (!GeneralHelpers::isNullPointer(HUDPointerCheck)) {
            Process::Write32((HUDPointerCheck + mainHUDoffset), 0x7F000004);
            Process::Write32((HUDPointerCheck + ActionButtonBGOffset), 0x00000010);
            Process::Write32((HUDPointerCheck + ActionButtonTextOffset), 0x00FFFF00);
        }

        Process::Write8(AddressList::OffScreenLocation.addr, disableOffScrArrow);
    }

    // does this reset itself? 
    void disableFog(MenuEntry* entry) {
        // eventually it would be nice to have proper labels for these addresses + values
        // once enough info is known
         
        Process::Write32((AddressList::FogA.addr), 0xFF700FFF);
        Process::Write32((AddressList::FogB.addr), 0x453B8000);
        Process::Write32((AddressList::FogC.addr), 0x459C4000);
    }
    
    void disableScrollingText(MenuEntry* entry) {
        isScrollTextDisabled = !isScrollTextDisabled;

        if (isScrollTextDisabled) {
            Process::Write32(AddressList::ScrollingTextOpacity.addr, 0x00000000);
            entry->SetName("Enable top screen scrolling text");
        }
        else {
            Process::Write32(AddressList::ScrollingTextOpacity.addr, 0x3F800000);
            entry->SetName("Disable top screen scrolling text");
        }
    }
}