#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    Clock cooldownTimer;

    /* ------------------ */

    // Freezes enemies' HP values or sets them to zero
    void manageEnemy(bool keepAlive)
    {
        u32 healthEdit = keepAlive ? 0x7FFFFFFF : 0x0;
        std::map<int, Actor> currActors = MapActorMngr::retrieveActorList();

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

    // Instantly kills all enemies in the current area
    void Gameplay::autoKillEnemy(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
            cooldownTimer.Restart();

        if (cooldownTimer.HasTimePassed(Milliseconds(500)) && entry->Hotkeys[0].IsPressed())
        {
            if (GeneralHelpers::isSoloActiveGameplay())
                manageEnemy(false);

            cooldownTimer.Restart();
        }
    }

    // Forces all enemies in the current area to have the maximum amount of possible HP
    void Gameplay::enemyInvinci(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
            cooldownTimer.Restart();

        if (cooldownTimer.HasTimePassed(Milliseconds(500)) && entry->Hotkeys[0].IsPressed())
        {
            if (GeneralHelpers::isSoloActiveGameplay())
                manageEnemy(true);

            cooldownTimer.Restart();
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