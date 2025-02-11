#include "Helpers.hpp"
#include "Main/Player.hpp"

namespace CTRPluginFramework
{
    MenuEntry *rotationOffsetManager;

    double maxOffset = 0xFFFFFFFF;
    float newOffset = 0.0;

    /* ------------------ */

    // Disables the rotation offset, which is originally set to 5.0 degrees
    void Player::disableOffset(MenuEntry *entry)
    {
        u32 currOffset;

        if (entry->WasJustActivated()) // one-time immediate write
            overwriteOffset(0.0);
        else
        {
            if (GeneralHelpers::isLoadingScreen())
            {
                Process::Read32(AddressList::getAddress("RotationOffset"), currOffset);

                // maintenance writes
                if (currOffset != 0x00000000)
                    overwriteOffset(0.0);
            }
        }

        if (!entry->IsActivated())
            overwriteOffset(5.0);
    }

    // Immediately resets the rotation offset according to user preference -> for use with Freecam/Chaos
    void Player::resetOffset(void)
    {
        if (Preferences::IsEnabled(Preferences::DisableMoveOffset))
            overwriteOffset(0.0);   // custom -> no-offset
        else
            overwriteOffset(5.0);   // vanilla
    }

    // Helper function to write rotation offset edits to memory | Note: accepts doubles for readability!
    void Player::overwriteOffset(double adjustment)
    {
        u32 currOffset;
        u32 adjustmentAsInt;
        double adjustPercent;

        Process::Read32(AddressList::getAddress("RotationOffset"), currOffset);

        // convert adjustment to u32 int...
        adjustPercent = adjustment / 360.0;
        adjustmentAsInt = static_cast<u32>(std::round(adjustPercent * maxOffset));

        // saves on write repetitions...
        if (currOffset != adjustmentAsInt)
            Process::Write32(AddressList::getAddress("RotationOffset"), adjustmentAsInt);
    }
}
