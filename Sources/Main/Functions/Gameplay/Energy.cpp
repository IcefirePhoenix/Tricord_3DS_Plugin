#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    const float normalGaugeMax = 600.0;
    const float largeEnergyGaugeMax = 900.0;
    const float generalItemDefaultCost = 0.25;
    const float bowSwimBoostDefaultCost = 0.125;
    const float bombDefaultCost = 0.5;
    const float fireShieldDefaultCost = 0.75;

    // Set current stamina/energy level to max value, which can also accommodate large energy gauges as well
    void Gameplay::infEnergy(MenuEntry *entry)
    {
        Process::WriteFloat(AddressList::getAddress("EnergyCurrent"), largeEnergyGaugeMax);

        // Apply to player 2 and player 3 if in multiplayer
        if (!GeneralHelpers::isSinglePlayer())
        {
            Process::WriteFloat(AddressList::getAddress("EnergyCurrent") + PLAYER_OFFSET, largeEnergyGaugeMax);
            Process::WriteFloat(AddressList::getAddress("EnergyCurrent") + 2 * PLAYER_OFFSET, largeEnergyGaugeMax);
        }
    }

    // Sets max stamina/energy value
    void Gameplay::maxEnergySet(MenuEntry *entry)
    {
        float newMaxEnergy;
        std::string maxEnergyIntro = "Enter a new value to update the capacity of the energy gauge.\n\nThe default value for the normal gauge is 600.0.\nThe default value for the large gauge is 900.0.";

        if (entry->Name() == "Set maximum energy")
        {
            Keyboard editMaxEnergy("Maximum Energy Setter", maxEnergyIntro);
            editMaxEnergy.IsHexadecimal(false);

            if (editMaxEnergy.Open(newMaxEnergy, normalGaugeMax) == 0)
            {
                entry->SetName("Disable custom maximum energy edits");

                Process::WriteFloat(AddressList::getAddress("EnergyMax"), newMaxEnergy);
                Process::WriteFloat(AddressList::getAddress("EnergyMaxPitRecovery"), newMaxEnergy);

                // update the height of the energy bar graphic...
                if (newMaxEnergy < normalGaugeMax)
                    Process::WriteFloat(AddressList::getAddress("EnergyBarStretch"), normalGaugeMax / newMaxEnergy);
            }
        }
        else
        {
            entry->SetName("Set maximum energy");

            // reset all values...
            Process::WriteFloat(AddressList::getAddress("EnergyMax"), normalGaugeMax);
            Process::WriteFloat(AddressList::getAddress("EnergyMaxPitRecovery"), normalGaugeMax);
            Process::WriteFloat(AddressList::getAddress("EnergyBarStretch"), 1.0);
        }
    }

    // Sets energy/stamina consumption multiplier for item/action use
    void Gameplay::energyConsumeMultiplier(MenuEntry *entry)
    {
        // note: energy consumption varies by item/action, based on a percentage of the NORMAL energy bar maximum...
        float newMultiplier;

        if (entry->Name() == "Set energy consumption multiplier")
        {
            Keyboard editEnergyConsumeMultiplier("Energy Consumption Multiplier Setter", "Input a new multiplier value. Using 1.0 will result in no changes.\n\nNote: The multiplier cannot be negative.");
            editEnergyConsumeMultiplier.IsHexadecimal(false);

            if (editEnergyConsumeMultiplier.Open(newMultiplier, 1.0) == 0)
            {
                if (newMultiplier > 0.0)
                {
                    entry->SetName("Disable custom energy consumption edits");
                    Process::WriteFloat(AddressList::getAddress("EnergyMaxMultiplicand"), normalGaugeMax * newMultiplier);
                }
                else
                    MessageBox("Error", "Energy multiplier cannot be negative.")();
            }
        }
        else
        {
            entry->SetName("Set energy consumption multiplier");
            Process::WriteFloat(AddressList::getAddress("EnergyMaxMultiplicand"), normalGaugeMax);
        }
    }

    // Toggles the use of the large energy gauge
    void Gameplay::useLargeEnergyGauge(MenuEntry *entry)
    {
        if (entry->Name() == "Use large energy gauge graphic")
        {
            Process::Patch(AddressList::getAddress("LargeEnergyGauge"), 0xEA000004);
            entry->SetName("Reset energy gauge graphic");
        }
        else
        {
            Process::Patch(AddressList::getAddress("LargeEnergyGauge"), 0x0A000004);
            entry->SetName("Use large energy gauge graphic");
        }
    }

    // Set energy costs by item / action
    void Gameplay::setEnergyCostByAction(MenuEntry* entry)
    {
        int actionChoice;
        float defaultCost, newCost;
        u32 address;

        StringVector actionList =
        {
            "Bow",
            "Bombs",
            "Hammer",
            "Gripshot",
            "Water Rod",
            "Gust Jar",
            "Boomerang",
            "Fire Gloves (Fireballs)",
            "Fire Gloves (Shield)",
            "Swim Boost",
            "Reset all to default"
        };

        Keyboard chooseAction("Item / Action Selection", "Choose an item or action to edit the energy cost of.");
        chooseAction.Populate(actionList);
        actionChoice = chooseAction.Open();

        switch (actionChoice)
        {
            case 0:
                defaultCost = bowSwimBoostDefaultCost;
                address = AddressList::getAddress("EnergyCostBow");
                break;
            case 1:
                defaultCost = bombDefaultCost;
                address = AddressList::getAddress("EnergyCostBombs");
                break;
            case 2:
                defaultCost = generalItemDefaultCost;
                address = AddressList::getAddress("EnergyCostHammer");
                break;
            case 3:
                defaultCost = generalItemDefaultCost;
                address = AddressList::getAddress("EnergyCostGripshot");
                break;
            case 4:
                defaultCost = generalItemDefaultCost;
                address = AddressList::getAddress("EnergyCostWaterRod");
                break;
            case 5:
                defaultCost = generalItemDefaultCost;
                address = AddressList::getAddress("EnergyCostGustJar");
                break;
            case 6:
                defaultCost = generalItemDefaultCost;
                address = AddressList::getAddress("EnergyCostBoomerang");
                break;
            case 7:
                defaultCost = generalItemDefaultCost;
                address = AddressList::getAddress("EnergyCostFireGlovesMain");
                break;
            case 8:
                defaultCost = fireShieldDefaultCost;
                address = AddressList::getAddress("EnergyCostFireGlovesShield");
                break;
            case 9:
                defaultCost = bowSwimBoostDefaultCost;
                address = AddressList::getAddress("EnergyCostSwimBoost");
                break;
            case 10:
                // Reset all to default values and return
                Process::WriteFloat(AddressList::getAddress("EnergyCostBow"), bowSwimBoostDefaultCost);
                Process::WriteFloat(AddressList::getAddress("EnergyCostBombs"), bombDefaultCost);
                Process::WriteFloat(AddressList::getAddress("EnergyCostHammer"), generalItemDefaultCost);
                Process::WriteFloat(AddressList::getAddress("EnergyCostGripshot"), generalItemDefaultCost);
                Process::WriteFloat(AddressList::getAddress("EnergyCostWaterRod"), generalItemDefaultCost);
                Process::WriteFloat(AddressList::getAddress("EnergyCostGustJar"), generalItemDefaultCost);
                Process::WriteFloat(AddressList::getAddress("EnergyCostBoomerang"), generalItemDefaultCost);
                Process::WriteFloat(AddressList::getAddress("EnergyCostFireGlovesMain"), generalItemDefaultCost);
                Process::WriteFloat(AddressList::getAddress("EnergyCostFireGlovesShield"), fireShieldDefaultCost);
                Process::WriteFloat(AddressList::getAddress("EnergyCostSwimBoost"), bowSwimBoostDefaultCost);
            default:
                return;
        }

        std::string title = "Energy Consumption - " + actionList[actionChoice];
        std::string desc = "Input a new energy cost value.\n\nFormat: Decimal percentage of a normal-sized gauge. Ex. The bow's default is 0.125 (12.5%)\n\nThe Fire Shield drains energy by the set amount once per frame.";

        Keyboard energyByActionKB(title, desc);
        energyByActionKB.IsHexadecimal(false);

        if (energyByActionKB.Open(newCost, defaultCost) == 0)
            Process::WriteFloat(address, newCost);
    }
}