#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include "CTRPF.hpp"
#include "Cheats.hpp"
#include "Helpers.hpp"
#include "Unicode.h"

namespace CTRPluginFramework
{
    extern MenuEntry* healthMaxAuto;

    extern MenuEntry* physicsEditAutoG;
    extern MenuEntry* physicsEditAutoB;
    extern MenuEntry* physicsEditAutoR;
    extern MenuEntry* physicsSelG;
    extern MenuEntry* physicsSelB;
    extern MenuEntry* physicsSelR;
    extern MenuEntry* ascentSpeedSet;
    extern MenuEntry* descentSpeedSet;

    extern MenuEntry* moonJumpEntry;
    extern MenuEntry* flightEntry;

    extern MenuEntry* reWarp;
    extern MenuEntry* doppelEnableAuto;
    extern MenuEntry* challengeEditAuto;

    extern MenuEntry* controlAllAuto;

    std::string physicsSelectMenu(int Link);
    void writePhysicsChanges(int player);
    float setHeightAdjustmentSpeed(float originalSpeed);

    void manageEnemy(bool keepAlive);
    void startWarp(void);

    std::string warpSelLevel(StringVector locNames);
    int warpGetLevel(int locCategory);
    int warpGetStage(int levelID);
    int warpSelStage(StringVector stageNames);
    int warpSelChallenge(void);

    void promptHP_Refill(int maxHP);
    void preventOverflowHP(int maxHP);

    bool canApplyYSpeed(int player);

    class Gameplay
    {
    public:
        static void infEnergy(MenuEntry* entry);
        static void maxEnergySet(MenuEntry* entry);
        static void energyConsumeMultiplier(MenuEntry* entry);
        static void useLargeEnergyGauge(MenuEntry* entry);

        static void maxHealthSet(MenuEntry* entry);
        static void writeMaxHealth(MenuEntry* entry);
        static void infHealth(MenuEntry* entry);
        static void infFairy(MenuEntry* entry);

        static void autoKillEnemy(MenuEntry* entry);
        static void enemyInvinci(MenuEntry* entry);

        static void controlAllPlayers(MenuEntry* entry);

        static void changePhysics(MenuEntry* entry);
        static void writePhysicsChanges(MenuEntry* entry);

        static void moonJumpAndFlight(MenuEntry *entry);
        static void forceHover(MenuEntry* entry);
        static void selAscentDescentEdit(MenuEntry *entry);
        static void adjustLateralSpeed(MenuEntry *entry);

        static void infTime(MenuEntry* entry);

        static void instantWarp(MenuEntry* entry);
        static void stageWarp(MenuEntry* entry);
        static void warpAgain(MenuEntry* entry);
        static void resetRoom(MenuEntry *entry);
        static void midWarpDoppelEnable(MenuEntry* entry);
        static void writeChallengeEdit(MenuEntry* entry);
        static void writePlayerControl(MenuEntry* entry);

        static void customSpeed(MenuEntry* entry);
    };
}
#endif