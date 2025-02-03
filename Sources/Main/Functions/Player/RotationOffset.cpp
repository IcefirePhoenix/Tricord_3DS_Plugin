#include "Helpers.hpp"
#include "Main/Player.hpp"

namespace CTRPluginFramework
{
    MenuEntry *rotationOffsetManager;

    u32 maxOffset = 0xFFFFFFFF;
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
    }

    // Immediately resets the rotation offset according to user preference -> for use with Freecam/Chaos
    void Player::resetOffset(void)
    {
        if (Preferences::DisableMoveOffset)
            overwriteOffset(0.0);   // custom -> no-offset
        else
            overwriteOffset(5.0);   // vanilla
    }

    // Allows custom rotation offsets
    void Player::adjustOffset(MenuEntry* entry)
    {
        std::string offsetAsStr = "None";

        // TODO: keyboard

        offsetAsStr = (newOffset == 0.0) ? "None" : std::to_string(newOffset) << "degrees";

        overwriteOffset(1.0);
        entry->SetName("Set rotation offset: " << offsetAsStr);
    }

    // Helper function to write rotation offset edits to memory
    // Note: accepts floats for readability!
    void Player::overwriteOffset(float adjustment)
    {
        float adjustPercent;
        u32 currOffset, adjustmentAsInt;

        Process::Read32(AddressList::getAddress("RotationOffset"), currOffset);

        // convert adjustment to hex-based u32...
        adjustPercent = adjustment / 360.0;
        adjustmentAsInt = static_cast<u32>(adjustPercent) * maxOffset;

        // saves on write repetitions...
        if (currOffset != adjustmentAsInt)
            Process::Write32(AddressList::getAddress("RotationOffset"), adjustmentAsInt);
    }
}
