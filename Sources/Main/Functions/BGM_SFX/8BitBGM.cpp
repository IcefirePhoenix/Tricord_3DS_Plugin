#include "Helpers.hpp"
#include "Main/BGM_SFX.hpp"

namespace CTRPluginFramework
{
    // Force-toggle normal or 8-bit BGMs
    void BGM_SFX::forceNormal8bit(MenuEntry *entry)
    {
        const StringVector BGMTypeOptions =
        {
            "Reset",
            "Normal BGM",
            "8-bit BGM"
        };

        Keyboard forceBGMType("Force Normal or 8-bit BGM, or\nReset to restore the Timeless Tunic check.");
        forceBGMType.Populate(BGMTypeOptions);

        switch (forceBGMType.Open())
        {
            case 0:
                // Default BEQ
                Process::Patch(AddressList::getAddress("BGMType"), 0x0A00003B);
                break;
            case 1:
                // Unconditional branch
                Process::Patch(AddressList::getAddress("BGMType"), 0xEA00003B);
                break;
            case 2:
                // NOP
                Process::Patch(AddressList::getAddress("BGMType"), 0xE320F000);
                break;
        }
    }

    // Force-toggle Timeless Tunic voice filter
    void BGM_SFX::disable8bitVoice(MenuEntry *entry)
    {
        if (entry->Name() == "Disable Timeless Tunic voice filter")
        {
            Process::Patch(AddressList::getAddress("Voice8bit"), 0xEA000006);
            entry->SetName("Enable Timeless Tunic voice filter");
        }
        else
        {
            Process::Patch(AddressList::getAddress("Voice8bit"), 0x1A000006);
            entry->SetName("Disable Timeless Tunic voice filter");
        }
    }

    // Force-toggle Timeless Tunic's sword SFX
    void BGM_SFX::force8bitSword(MenuEntry *entry)
    {
        if (entry->Name() == "Force 8-bit sword SFX")
        {
            Process::Patch(AddressList::getAddress("SwordSwipe8bit"), 0xEA00000B);
            Process::Patch(AddressList::getAddress("SwordCharge8bit"), 0xEA00000B);
            Process::Patch(AddressList::getAddress("SwordSpin8bit"), 0xEA000021);
            entry->SetName("Reset sword SFX");
        }
        else
        {
            Process::Patch(AddressList::getAddress("SwordSwipe8bit"), 0x0A00000B);
            Process::Patch(AddressList::getAddress("SwordCharge8bit"), 0x0A00000B);
            Process::Patch(AddressList::getAddress("SwordSpin8bit"), 0x0A000021);
            entry->SetName("Force 8-bit sword SFX");
        }
    }
}