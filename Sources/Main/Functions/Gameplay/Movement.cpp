#include "Cheats.hpp"
#include "Main/Gameplay.hpp"
#include "Main/Costume.hpp"

namespace CTRPluginFramework
{
    MenuEntry *controlAllAuto;
    MenuEntry* fixSwordSuitDamageAuto;

    bool controlAuto = false;

    /* ------------------ */

    // Sets custom movement speed
    void Gameplay::customSpeed(MenuEntry *entry)
    {
        float newSpeed, defaultSpeed = 1.0;
        std::string speedIntro = "Enter a custom movement speed value.\n\nThe default value is 1.0.\nNegative values will invert movement.";

        if (entry->Name() == "Set custom movement speed")
        {
            Keyboard editSpeed(speedIntro);
            editSpeed.IsHexadecimal(false);

            if (editSpeed.Open(newSpeed) == 0)
            {
                entry->SetName("Disable custom movement speed edits");
                Process::WriteFloat(AddressList::getAddress("SpeedMultiplierNormal"), newSpeed);

                // Redirect all sword damage multiplier writes to go to the speed multiplier address
                // Since it writes the same speed multiplier value anyway
                Process::Write8(AddressList::getAddress("SwordDamageBoostA"), 0x41);
                Process::Write8(AddressList::getAddress("SpeedDamageUndoA"), 0x41);
                Process::Write8(AddressList::getAddress("SpeedDamageUndoB"), 0x41);

                // Manually write sword damage multiplier
                fixSwordSuitDamageAuto->Enable();
            }
        }
        else
        {
            entry->SetName("Set custom movement speed");
            Process::WriteFloat(AddressList::getAddress("SpeedMultiplierNormal"), defaultSpeed);

            // Restore normal functionality of speed/damage value application
            Process::Write8(AddressList::getAddress("SwordDamageBoostA"), 0x1B);
            Process::Write8(AddressList::getAddress("SpeedDamageUndoA"), 0x1B);
            Process::Write8(AddressList::getAddress("SpeedDamageUndoB"), 0x1B);
            fixSwordSuitDamageAuto->Disable();
        }
    }

    // Used to replace the executable's function to set player sword damage
    // A necessary workaround due to the default speed multiplier and sword damage multiplier sharing the same data address
    void Gameplay::writeSwordSuitDamageFix(MenuEntry* entry)
    {
        if (GeneralHelpers::isLoadingScreen(false))
        {
            u8 currCostume;
            for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
            {
                Process::Read8(AddressList::getAddress("CurrCostume") + iterateThruPlayers * PLAYER_OFFSET, currCostume);
                if (currCostume == 0x1 || currCostume == 0x20 || currCostume == 0x25 || Costume::forcingSwordDmgBoost)
                    Process::WriteFloat(AddressList::getAddress("CurrSwordDamageMult") + iterateThruPlayers * PLAYER_OFFSET, Costume::customSwordHammerDmgMult);
                else
                    Process::WriteFloat(AddressList::getAddress("CurrSwordDamageMult") + iterateThruPlayers * PLAYER_OFFSET, 1.0);
            }
        }
    }

    // Toggles all-player movement/input control
    void Gameplay::controlAllPlayers(MenuEntry *entry)
    {
        if (entry->Name() == "Control all players")
        {
            controlAuto = true;
            controlAllAuto->Enable();

            entry->SetName("Reset control to Player 1 (Green)");
        }
        else
        {
            controlAuto = false;
            controlAllAuto->Disable();

            GeneralHelpers::setCurrLink(GameData::getPlayerIDFromColor("Green"));
            GeneralHelpers::setPlayerCam(GameData::getPlayerIDFromColor("Green"));

            entry->SetName("Control all players");
        }
    }

    // Helper function that enables all-player control
    void Gameplay::writePlayerControl(MenuEntry *entry)
    {
        if (controlAuto)
        {
            GeneralHelpers::setCurrLink(GameData::getPlayerIDFromColor("All"));
        }
    }
}