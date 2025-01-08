#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    MenuEntry *instantTextDisplay;
    bool useInstantText;

    /* ------------------ */

    // Force-changes text display speed
    void Miscellaneous::manageInstantText(MenuEntry *entry)
    {
        useInstantText = !useInstantText;

        // not using absolute max of 0xFF to avoid graphical bugs... 0x2D is fast enough
        u8 textSpeed = useInstantText ? 0x2D : 0x1;
        std::string entryName = useInstantText ? "Disable instant text boxes" : "Force instant text boxes";

        Process::Write8(AddressList::TextBoxSpeed.addr, textSpeed);
        entry->SetName(entryName);
    }
}