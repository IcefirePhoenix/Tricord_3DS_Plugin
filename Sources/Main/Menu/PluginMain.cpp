#include <3ds.h>

#include "Helpers.hpp"
#include "Cheats.hpp"
#include "CTRPF.hpp"

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
        s64     textTotalRoundedSize = 0;
        s64     startAddress = 0;
        u32*    found;

        if (R_FAILED(svcOpenProcess(&processHandle, 16)))
            return;

        svcGetProcessInfo(&textTotalRoundedSize, processHandle, 0x10002);
        svcGetProcessInfo(&startAddress, processHandle, 0x10005);
        if (R_FAILED(svcMapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, processHandle, (u32)startAddress, textTotalRoundedSize, static_cast<MapExFlags>(0))))
            goto exit;

        found = (u32*)Utils::Search<u32>(0x14000000, (u32)textTotalRoundedSize, pattern);

        if (found != nullptr)
        {
            original = found[13];
            patchAddress = (u32*)PA_FROM_VA((found + 13));
            found[13] = 0xE1A00000; // NOP operation
        }

        svcUnmapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, textTotalRoundedSize);
    exit:
        svcCloseHandle(processHandle);
    }

    void PatchProcess(void)
    {
    }

    void ManageTFH_Settings(void)
    {
        // TODO: is there a better way of doing this
        if (Preferences::PretendoPatch)
            pretendoOnlinePatchManager->Enable();
        else
            pretendoOnlinePatchManager->Disable();

        if (Preferences::DisableMoveOffset)
            rotationOffsetManager->Enable();
        else
            rotationOffsetManager->Disable();

        if (Preferences::DoppelStageCostumeReset)
            rotationOffsetManager->Enable();
        else
            rotationOffsetManager->Disable();
    }

    void InitSequence(FwkSettings &settings)
    {
        AddressList::InitMemoryRange();
        AddressList::InitAddresses();
        DescUtils::InitDescriptions();

        if (Preferences::IsEnabled(Preferences::HIDToggle))
            settings.UseGameHidMemory = true;

        // settings.CachedDrawMode = true;

        PatchProcess();
        ToggleTouchscreenForceOn();

        //autoBeamCooldown->Enable(); // why is this enabled here?
    }

    void OnProcessExit(void)
    {
        ToggleTouchscreenForceOn();
        Process::ReturnToHomeMenu(); // force exit to home menu might fix old system crashes...
    }

    // This function only runs once at plugin startup
    int main(void)
    {
        std::string title = "An advanced utility plugin made for\nThe Legend of Zelda: Tri Force Heroes packed with QoL features, solo and online-compatible cheats, and customizable gameplay options.\n\nForked from the original CTRPluginFramework\nblank template repository.";
        PluginMenu *menu = new PluginMenu("Tricord", 0, 5, 0, title);

        InitSequence(FwkSettings::Get());

        menu->SynchronizeWithFrame(true);
        menu->OnNewFrame = ToggleMenuChange;
        menu->OnClosing = ManageTFH_Settings;

        CreateMenu(*menu);
        menu->Run();

        // end of plugin lifetime
        delete menu;
        OnProcessExit();

        // should not reach here -> force home menu = NORETURN
        return (0);
    }
}
