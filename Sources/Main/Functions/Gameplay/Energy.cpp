#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    // Set current stamina/energy level to max value, which can also accommodate large energy gauges as well
    void Gameplay::infEnergy(MenuEntry *entry)
    {
        float largeEnergyGaugeMax = 900.0;
        Process::WriteFloat(AddressList::getAddress("EnergyCurrent"), largeEnergyGaugeMax);
    }

    // Sets max stamina/energy value
    void Gameplay::maxEnergySet(MenuEntry *entry)
    {
        float newMaxEnergy, normalGaugeMax = 600.0;
        std::string maxEnergyIntro = "Maximum energy\n\nDefault value for normal gauge is 600\nDefault value for large gauge is 900";

        if (entry->Name() == "Set maximum energy")
        {
            Keyboard editMaxEnergy(maxEnergyIntro);
            editMaxEnergy.IsHexadecimal(false);

            if (editMaxEnergy.Open(newMaxEnergy) == 0)
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
        float newMultiplier, baseEnergyMax = 600.0;

        if (entry->Name() == "Set energy consumption multiplier")
        {
            Keyboard editEnergyConsumeMultiplier("Energy consumption multiplier");
            editEnergyConsumeMultiplier.IsHexadecimal(false);

            if (editEnergyConsumeMultiplier.Open(newMultiplier) == 0 && newMultiplier > 0.0)
            {
                entry->SetName("Disable custom energy consumption edits");
                Process::WriteFloat(AddressList::getAddress("EnergyMaxMultiplicand"), baseEnergyMax * newMultiplier);
            }
        }
        else
        {
            entry->SetName("Set energy consumption multiplier");
            Process::WriteFloat(AddressList::getAddress("EnergyMaxMultiplicand"), baseEnergyMax);
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
}