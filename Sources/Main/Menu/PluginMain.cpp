#include <3ds.h>

#include "Helpers.hpp"
#include "Cheats.hpp"
#include "CTRPF.hpp"

constexpr std::array<u32, 3> starTagAddrs = STAR_TAG_ADDRS;
constexpr std::array<u32, 3> multiplayerInfo = MULTIPLAYER_INFO;

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

    void PatchProcess()
    {
    }

    void ManageTFH_Settings(void)
    {
        if (Preferences::IsEnabled(Preferences::QoL_Patch))
        {
            pretendoOnlinePatchManager->Enable();
            rotationOffsetManager->Enable();
            doppelCostumeStageResetManager->Enable();
        }
        else
        {
            pretendoOnlinePatchManager->Disable();
            rotationOffsetManager->Disable();
            doppelCostumeStageResetManager->Disable();
        }
    }

    void InitHeroPanelTag(void)
    {
        AddressList::registerNewAddress("MultiplayerInfo", multiplayerInfo);

        if (AddressList::registerNewAddress("StarTag", starTagAddrs))
            Process::Write32(AddressList::getAddress("StarTag"), STAR_TAG);
    }

    void LoadSavedAndCustomEntryData(void)
    {
        Rendering::loadCustomNameColors();
        Gameplay::restoreBookmarks();
        FaceExprEditor::initSeq();
    }

    void RetrieveAddressArrays(void)
    {
        Item::initItemAddresses();
        Emotes::initEmoteAddresses();
        Rendering::initHeroPanelBasePtrs();
    }

    void InitSequence(FwkSettings &settings)
    {
        AddressList::InitMemoryRange();
        AddressList::InitAddresses();
        DescUtils::InitDescriptions();
        FAQ_Parse::GetContent();

        if (Preferences::IsEnabled(Preferences::HIDToggle))
            settings.UseGameHidMemory = true;

        PatchProcess();
        ToggleTouchscreenForceOn();

        RetrieveAddressArrays();
        InitHeroPanelTag();
    }

    int main(void)
    {
        PluginMenu *menu = new PluginMenu();

        InitSequence(FwkSettings::Get());
        CreateMenu(*menu);

        menu->SynchronizeWithFrame(true);
        menu->OnReady = LoadSavedAndCustomEntryData;
        menu->OnNewFrame = ToggleMenuChange;
        menu->OnClosing = ManageTFH_Settings;

#ifdef DEV_BUILD
        menu->SpecifyDevBuild();
#endif
        menu->Run();
        return (0);
    }
}
