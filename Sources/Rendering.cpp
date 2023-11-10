#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
    void hideHUD(MenuEntry* entry) {
        // TODO: add doppel location indicators

        // prep check  -- if HUD pointer is null
        u32 pointerCheck;
        Process::Read32(AddressList::HUDPointer.addr, pointerCheck);

        // define offsets for easy reference
        u32 mainHUDoffset = 0x00000018;
        u32 ActionButtonBGOffset = 0x000075D0;
        u32 ActionBUttonTextOffset = 0x000078B0;

        if (pointerCheck != 0x00000000) {
            Process::Write32((pointerCheck + mainHUDoffset), 0x7F000004);
            Process::Write32((pointerCheck + ActionButtonBGOffset), 0x00000010);
            Process::Write32((pointerCheck + ActionBUttonTextOffset), 0x00FFFF00);
        }
    }

    // does this reset itself? 
    void disableFog(MenuEntry* entry) {
        Process::Write32((AddressList::FogA.addr), 0xFF700FFF);
        Process::Write32((AddressList::FogB.addr), 0x453B8000);
        Process::Write32((AddressList::FogC.addr), 0x459C4000);
    }

    bool isScrollTextDisabled = false;
    
    void disableScrollingText(MenuEntry* entry) { // this should be a toggle
        isScrollTextDisabled = !isScrollTextDisabled;

        if (isScrollTextDisabled) {
            Process::Write32(AddressList::ScrollingTextOpacity.addr, 0x00000000);
            MessageBox(Color::Gainsboro << "Success", "Scrolling Text has been disabled.")();

        }
        else {
            Process::Write32(AddressList::ScrollingTextOpacity.addr, 0x3F800000);
            MessageBox(Color::Gainsboro << "Success", "Scrolling Text has been enabled.")();
        }
    }
}