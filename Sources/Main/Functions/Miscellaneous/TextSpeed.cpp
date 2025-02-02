#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    MenuEntry *instantTextDisplay;
    MenuEntry *pretendoOnlinePatchManager;

    bool useInstantText;

    /* ------------------ */

    // Force-changes text display speed
    void Miscellaneous::manageInstantText(MenuEntry* entry)
    {
        useInstantText = !useInstantText;

        // not using absolute max of 0xFF to avoid graphical bugs... 0x2D is fast enough
        u8 textSpeed = useInstantText ? 0x2D : 0x1;
        std::string entryName = useInstantText ? "Disable instant text boxes" : "Force instant text boxes";

        Process::Write8(AddressList::TextBoxSpeed.addr, textSpeed);
        entry->SetName(entryName);
    }

    // Aligns the MSBT text display timing tags between regions to avoid cross-region online cutscene desyncs
    void Miscellaneous::applyPretendoPatch(MenuEntry* entry)
    {
        /* Thanks to Dimitri for figuring out the file patches required to make Pretendo online multiplayer gameplay fully compatible cross-region! */

        // TODO: msbt parsing and patching...
    }
}