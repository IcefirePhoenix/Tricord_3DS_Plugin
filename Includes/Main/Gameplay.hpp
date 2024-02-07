#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include "CTRPluginFramework.hpp"
#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Unicode.h"

namespace CTRPluginFramework {
    void manageEnemy(bool keepAlive);

    class Gameplay {
    public:
        static void infEnergy(MenuEntry* entry);
        static void maxEnergySet(MenuEntry* entry);
        static void energyConsumeMultiplier(MenuEntry* entry);
        static void infHealth(MenuEntry* entry);
        static void noHealth(MenuEntry* entry);
        static void infFairy(MenuEntry* entry);
        static void autoKillEnemy(MenuEntry* entry);
        static void enemyInvinci(MenuEntry* entry);
        static void controlAllPlayers(MenuEntry* entry);
        static void icePhysicsEverywhere(MenuEntry* entry);
        static void infTIme(MenuEntry* entry);
    }; 
}
#endif