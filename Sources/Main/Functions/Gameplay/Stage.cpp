#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    u8 cutsceneComplete = 0x40;

    // Prevents the timer from decreasing in timed challenges
    void Gameplay::infTime(MenuEntry *entry)
    {
        // 0xEA61 --> 1000:0(1)
        // If the rightmost seconds digit is visible, it always flickers whenever the game tries to tick the timer down
        // However, we can avoid this flicker by pushing this digit offscreen, by setting the minutes to have 4 digits
        Process::Write16(AddressList::getAddress("TimeLeft"), 0xEA61);
    }

    // Sets the status of all progression flags in the current area
    void Gameplay::setAllProgressionFlags(MenuEntry *entry)
    {
        if (entry->Hotkeys[0].IsPressed())
            Process::Write64(AddressList::getAddress("StageProgressionFlags"), UINT64_MAX);
    }

    // Sets the status of specified progression flags in the current area
    void setSpecificProgressionFlags(u64 flags)
    {
        Process::Write64(AddressList::getAddress("StageProgressionFlags"), flags);
    }

    // Allows boss introduction cutscenes to be skipped
    void Gameplay::skipBossIntro(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
        {
            Process::Patch(AddressList::getAddress("BossIntroInit"), 0xE1A00000); // NOP
            Process::Patch(AddressList::getAddress("BossCamCutsceneInit"), 0xE1A00000); // NOP
            Process::Patch(AddressList::getAddress("BossBGMCutsceneInit"), 0xE1A00000); // NOP

            Process::Patch(AddressList::getAddress("BossIntroInit") + 0x4, 0xEA000000); // B, no offset
            Process::Patch(AddressList::getAddress("BossCamCutsceneInit") + 0x4, 0xEA000000); // B, no offset
            Process::Patch(AddressList::getAddress("BossBGMCutsceneInit") + 0x4, 0xEA000007); // B
        }
        else if (!entry->IsActivated())
        {
            Process::Patch(AddressList::getAddress("BossIntroInit"), 0xE3500000); // CMP R0, 0x0
            Process::Patch(AddressList::getAddress("BossCamCutsceneInit"), 0xE3500000); // CMP R0, 0x0
            Process::Patch(AddressList::getAddress("BossBGMCutsceneInit"), 0xE3500000); // CMP R0, 0x0

            Process::Patch(AddressList::getAddress("BossIntroInit") + 0x4, 0x0A000056); // BEQ
            Process::Patch(AddressList::getAddress("BossCamCutsceneInit") + 0x4, 0x0A000053); // BEQ
            Process::Patch(AddressList::getAddress("BossBGMCutsceneInit") + 0x4, 0x0A000007); // BEQ
        }
    }

    // Allows Lady Maud dialogue cutscenes to be skipped
    void Gameplay::skipLadyDialogue(MenuEntry *entry)
    {
        if (GeneralHelpers::isLoadingScreen(true))
        {
            bool isMidgame = Level::getCurrLevel() == Level::levelIDFromName("The Lady's Lair");
            bool isEndgame = Level::getCurrLevel() == Level::levelIDFromName("Sky Temple");

            if ((isMidgame || isEndgame) && Level::getCurrStage() == 4)
            {
                Process::Write8(AddressList::getAddress("LadyMidgameCutsceneStatus"), cutsceneComplete);
                Process::Write8(AddressList::getAddress("LadyEndgameCutsceneStatus"), cutsceneComplete);

                Rendering::toggleHUD(false); // note: one-time write; will not conflict with HideHUD if enabled
            }
        }
    }
}