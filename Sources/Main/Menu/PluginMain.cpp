#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // Patch NFC disabling the touchscreen when scanning an amiibo
    static void ToggleTouchscreenForceOn(void)
    {
        static u32 original = 0;
        static u32* patchAddress = nullptr;

        if (patchAddress && original)
        {
            *patchAddress = original;
            return;
        }

        static const std::vector<u32> pattern =
        {
            0xE59F10C0, 0xE5840004, 0xE5841000, 0xE5DD0000,
            0xE5C40008, 0xE28DD03C, 0xE8BD80F0, 0xE5D51001,
            0xE1D400D4, 0xE3510003, 0x159F0034, 0x1A000003
        };

        Result  res;
        Handle  processHandle;
        s64     textTotalSize = 0;
        s64     startAddress = 0;
        u32*    found;

        if (R_FAILED(svcOpenProcess(&processHandle, 16)))
            return;

        svcGetProcessInfo(&textTotalSize, processHandle, 0x10002);
        svcGetProcessInfo(&startAddress, processHandle, 0x10005);
        if (R_FAILED(svcMapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, processHandle, (u32)startAddress, textTotalSize)))
            goto exit;

        found = (u32*)Utils::Search<u32>(0x14000000, (u32)textTotalSize, pattern);

        if (found != nullptr)
        {
            original = found[13];
            patchAddress = (u32*)PA_FROM_VA((found + 13));
            found[13] = 0xE1A00000; // NOP operation
        }

        svcUnmapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, textTotalSize);
    exit:
        svcCloseHandle(processHandle);
    }

    void PatchProcess(void)
    {
        // Disable Doppel-between-stage-costume reset
        Process::Patch(AddressList::DoppelStageResetA.addr, 0xE320F000);
        Process::Patch(AddressList::DoppelStageResetB.addr, 0xE320F000);
        Process::Patch(AddressList::DoppelStageResetC.addr, 0xE320F000);
    }

    void InitSequence(FwkSettings &settings)
    {
        Address::InitMemoryRange();
        AddressList::InitAddresses();

        if (Preferences::IsEnabled(Preferences::HIDToggle))
            settings.UseGameHidMemory = true;

        // settings.CachedDrawMode = true;

        PatchProcess();
        ToggleTouchscreenForceOn();

        // autoBeamCooldown->Enable(); // why is this enabled here?
    }

    void OnProcessExit(void)
    {
        ToggleTouchscreenForceOn();
    }

    // This function only runs once at plugin startup
    int main(void)
    {
        std::string title = "An advanced, region-free cheat plugin made for\nThe Legend of Zelda: Tri Force Heroes.\n\nForked from the original CTRPluginFramework\nblank template repository.";
        PluginMenu *menu = new PluginMenu("Tricord", 0, 5, 0, title);

        menu->SynchronizeWithFrame(true);
        menu->OnNewFrame = ToggleMenuChange;

        CreateMenu(*menu);
        InitSequence(FwkSettings::Get());

        menu->Run();

        // Exit plugin
        delete menu;
        OnProcessExit();
        return (0);
    }
}
