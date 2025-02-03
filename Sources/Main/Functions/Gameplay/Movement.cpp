#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    MenuEntry *controlAllAuto;

    bool controlAuto = false;

    /* ------------------ */

    // Sets custom movement speed
    void Gameplay::customSpeed(MenuEntry *entry)
    {
        float newSpeed, defaultSpeed = 0.095;
        std::string speedIntro = "Enter a custom movement speed value.\n\nThe default value is 0.095.\nNegative values will invert movement.";

        if (entry->Name() == "Set custom movement speed")
        {
            Keyboard editSpeed(speedIntro);
            editSpeed.IsHexadecimal(false);

            if (editSpeed.Open(newSpeed) == 0)
            {
                entry->SetName("Disable custom movement speed edits");
                Process::WriteFloat(AddressList::getAddress("SpeedMultiplierAlt"), newSpeed);
            }
        }
        else
        {
            entry->SetName("Set custom movement speed");
            Process::WriteFloat(AddressList::getAddress("SpeedMultiplierAlt"), defaultSpeed);
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
        /// TODO: is bool impl necessary?
        if (controlAuto)
        {
            GeneralHelpers::setCurrLink(GameData::getPlayerIDFromColor("All"));
        }
    }
}