#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    // Prevents the timer from decreasing in timed challenges
    void Gameplay::infTime(MenuEntry *entry)
    {
        // 0xEA61 --> 1000:0(1)
        // If the rightmost seconds digit is visible, it always flickers whenever the game tries to tick the timer down
        // However, we can avoid this flicker by pushing this digit offscreen, by setting the minutes to have 4 digits
        Process::Write16(AddressList::getAddress("TimeLeft"), 0xEA61);
    }

    // Sets the status of all progression flags in the current area
    void setAllProgressionFlags(void)
    {
        Process::Write64(AddressList::getAddress("StageProgressionFlags"), UINT64_MAX);
    }

    // Sets the status of certain progression flags in the current area
    void setSpecificProgressionFlags(u64 flags)
    {
        Process::Write64(AddressList::getAddress("StageProgressionFlags"), flags);
    }
}