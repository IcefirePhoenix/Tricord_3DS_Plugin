#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    u16 bossHP_Offset = 0x660;
    u32 blockOffset = 0x10000;

    Clock execCooldownTimer;
    Clock bossCooldownTimer;

    /* ------------------ */

    // Freezes enemies' HP values or sets them to zero
    void manageEnemy(bool keepAlive)
    {
        u32 healthEdit = keepAlive ? 0x7FFFFFFF : 0x0;
        std::map<int, Actor> currActors = MapActorMngr::retrieveActorList();

        if ((Freecam::getCameraType() > CameraMode::DYNAMIC))
            return;

        if (!currActors.empty())
        {
            for (auto &mapEntry : currActors)
            {
                auto &actorInstance = mapEntry.second;
                if (actorInstance.isType(ActorType::ENEMY))
                    actorInstance.setHealth(healthEdit);
            }
        }
    }

    // Toggles the boss intro cutscene
    void toggleEnemyIntro(bool skipIntro)
    {
        Process::Patch(AddressList::getAddress("BossIntroCutsceneInit"), skipIntro ? 0xEA000000 : 0x0A0000056);
        Process::Patch(AddressList::getAddress("BossIntroCameraInit"), skipIntro ? 0xEA000000 : 0x0A0000053);
        Process::Patch(AddressList::getAddress("BossIntroBGMInit"), skipIntro ? 0xEA000007 : 0x0A000007);
    }

    // Toggles the boss defeat sequence by force-setting its status flag
    // When Margoma and Arrghus are defeated early, some stage progression flags aren't triggered; handle manually
    void setBossDefeatFlag(bool defeat)
    {
        u8 levelID = Level::getCurrLevel();
        if (levelID == Level::levelIDFromName("Forest Temple") || levelID == Level::levelIDFromName("Water Temple"))
        {
            if (defeat)
            {
                toggleEnemyIntro(true);
                setAllProgressionFlags(); // to open stone gate + lower water level, respectively
            }
            else
                toggleEnemyIntro(false);
        }

        Process::Write32(AddressList::getAddress("BossDefeatFlagSet"), defeat ? 0xC3A00001 : 0xC3A00000);
        bossCooldownTimer.Restart();
    }

    // Sets the boss defeat flag; any fights with multiple phases are handled a bit differently to prevent softlocks
    void tryKillBosses(void)
    {
        std::array<u8, 3> phaseOrder{};
        u8 levelID = Level::getCurrLevel();
        u32 bossStartAddress = 0x0;
        u32 bossHP = 0x0;

        Process::Read32(AddressList::getAddress("BossDataPointer"), bossStartAddress);

        if (GeneralHelpers::isNullPointer(bossStartAddress))
            return;

        // can't call Level::levelIDFromName() as case labels here
        switch (levelID)
        {
            case 0x35:
                phaseOrder = {3, 2, 1}; // Lady's Lair
                break;
            case 0x53:
                phaseOrder = {2, 1, 3}; // Sky Temple
                break;
            case 0x61:
                phaseOrder = {1, 2, 3}; // Baneful Zone
                break;
            default:
                break;
        }

        // multiphase logic
        if (phaseOrder[0] != 0)
        {
            for (u8 phase : phaseOrder)
            {
                u32 finalAddr = bossStartAddress + ((phase - 1) * blockOffset) + bossHP_Offset;
                Process::Read32(finalAddr, bossHP);

                if (bossHP != 0x0) // write to first phase encountered that isn't defeated
                {
                    Process::Write32(finalAddr, 0x00000000);

                    // write to remaining phases if Shadow Links
                    if (levelID != Level::levelIDFromName("Baneful Zone"))
                        return;
                }
            }
        }
        else
            setBossDefeatFlag(true);
    }

    // Instantly kills all enemies in the current area
    void Gameplay::autoKillEnemy(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
        {
            execCooldownTimer.Restart();
            bossCooldownTimer.Restart();
        }

        if (execCooldownTimer.HasTimePassed(Milliseconds(500)) && entry->Hotkeys[0].IsPressed())
        {
            if (GeneralHelpers::isSoloActiveGameplay())
            {
                manageEnemy(false);

                if (Level::isInBossLevel() && Level::getCurrStage() == 0x4)
                    tryKillBosses();
            }
            execCooldownTimer.Restart();
        }

        if (bossCooldownTimer.HasTimePassed(Milliseconds(100)))
            setBossDefeatFlag(false);
    }

    // Forces all enemies in the current area to have the maximum amount of possible HP
    void Gameplay::enemyInvinci(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
            execCooldownTimer.Restart();

        if (execCooldownTimer.HasTimePassed(Milliseconds(500)) && entry->Hotkeys[0].IsPressed())
        {
            if (GeneralHelpers::isSoloActiveGameplay())
                manageEnemy(true);

            execCooldownTimer.Restart();
        }
    }

    void Gameplay::disableColiWallmaster(MenuEntry* entry)
    {
        if (entry->Name() == "Disable Coliseum Wallmaster")
        {
            Process::Patch(AddressList::getAddress("ColiseumWallmasterSpawn"), 0xEA000011);
            entry->SetName("Enable Coliseum Wallmaster");
        }
        else
        {
            Process::Patch(AddressList::getAddress("ColiseumWallmasterSpawn"), 0x1A000011);
            entry->SetName("Disable Coliseum Wallmaster");
        }
    }
}