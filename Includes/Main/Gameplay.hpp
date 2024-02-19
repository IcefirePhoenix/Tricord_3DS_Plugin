#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include "CTRPluginFramework.hpp"
#include "Helpers.hpp"
#include "Cheats.hpp"
#include "Unicode.h"

namespace CTRPluginFramework {
    void manageEnemy(bool keepAlive);
    extern MenuEntry* physicsEditAutoG;
    extern MenuEntry* physicsEditAutoB;
    extern MenuEntry* physicsEditAutoR;
    std::string physicsSelectMenu(int Link);
    void writePhysicsChanges(int Link);
    extern MenuEntry* moonJumpEntry;
    extern MenuEntry* flightEntry;

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
        static void changePhysicsG(MenuEntry* entry);
        static void changePhysicsB(MenuEntry* entry);
        static void changePhysicsR(MenuEntry* entry);
        static void physicsChangeG(MenuEntry* entry);
        static void physicsChangeB(MenuEntry* entry);
        static void physicsChangeR(MenuEntry* entry);
        static void moonJump(MenuEntry* entry);
        static void flight(MenuEntry* entry);
        static void adjustAscentSpeed(MenuEntry* entry);
        static void adjustDescentSpeed(MenuEntry* entry);
        static void adjustLateralSpeed(MenuEntry* entry);
        static void infTime(MenuEntry* entry);
    }; 
}
#endif