#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
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
        if (entry->Hotkeys[0].IsPressed() && GeneralHelpers::isSoloActiveGameplay())
            manageEnemy(false);

        Sleep(Milliseconds(500));
    }

    // Forces all enemies in the current area to have the maximum amount of possible HP
    void Gameplay::enemyInvinci(MenuEntry *entry)
    {
        if (entry->Hotkeys[0].IsPressed() && GeneralHelpers::isSoloActiveGameplay())
            manageEnemy(true);

        Sleep(Milliseconds(500));
    }
}