#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    MenuEntry *healthMaxAuto;

    u8 defaultMaxHP = 0x24;
    u8 customMaxHealth = 0x24;
    bool healthAuto = false; // TODO: double-check why this is necessary

    /* ------------------ */

    // Allows users to set custom maximum HP values
    void Gameplay::maxHealthSet(MenuEntry *entry)
    {
        u8 newMaxHealth;
        std::string maxHealthIntro = "Maximum heart containers\n\nEnter a positive integer. The number of heart containers can range from 1 to 63, but only up to 18 heart containers can be displayed on the HUD.";

        if (entry->Name() == "Set maximum heart containers")
        {
            Keyboard editMaxHealth(maxHealthIntro);
            editMaxHealth.IsHexadecimal(false);

            if (editMaxHealth.Open(newMaxHealth) == 0)
            {
                // bounds limiting...
                if (newMaxHealth > 63)
                    newMaxHealth = 63;
                else if (newMaxHealth <= 0)
                    newMaxHealth = 1;

                // convert from full hearts to quarter hearts...
                customMaxHealth = (newMaxHealth) * 4;

                healthAuto = true;
                healthMaxAuto->Enable();
                entry->SetName("Reset to 9 heart containers");

                promptHP_Refill(customMaxHealth);
                preventOverflowHP(customMaxHealth);
            }
        }
        else
        {
            healthAuto = false;
            healthMaxAuto->Disable();

            // restore max HP value...
            Process::Write8(AddressList::HealthMax.addr, defaultMaxHP);
            preventOverflowHP(defaultMaxHP);

            entry->SetName("Set maximum heart containers");
        }
    }

    // Helper function to refill HP after setting custom maximumm HP value
    void promptHP_Refill(int maxHP)
    {
        u8 currentHealth = GeneralHelpers::getHP();

        if (currentHealth < maxHP)
        {
            if (GeneralHelpers::showMsgKbd("Refill hearts?", DialogType::DialogYesNo))
                Process::Write8(AddressList::HealthCurrent.addr, maxHP);
        }
    }

    // Helper function to prevent current HP value from surpassing custom maximum HP value
    void preventOverflowHP(int maxHP)
    {
        u8 currentHealth = GeneralHelpers::getHP();

        if (currentHealth > maxHP)
            Process::Write8(AddressList::HealthCurrent.addr, maxHP);
    }

    // Sets maximum HP value in memory
    void Gameplay::writeMaxHealth(MenuEntry *entry)
    {
        // TODO: does this need to be written at all times to avoid conflicting overwrites, or only once during loading zones?
        // TODO: is bool impl needed?

        if (healthAuto)
        {
            Process::Write8(AddressList::HealthMax.addr, customMaxHealth);
        }
    }

    // Freezes current HP value to the maximum value
    void Gameplay::infHealth(MenuEntry *entry)
    {
        u8 maxHealth;
        Process::Read8(AddressList::HealthMax.addr, maxHealth);
        Process::Write8(AddressList::HealthCurrent.addr, maxHealth);
    }

    // Freezes current number of fairies to 9 (0xA because 0 fairies starts at 0x1)
    void Gameplay::infFairy(MenuEntry *entry)
    {
        Process::Write8(AddressList::FairiesCurrent.addr, 0xA);
    }
}