#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    MenuEntry *healthMaxAuto;

    u8 defaultMaxHP = 0x24;
    u8 customMaxHealth = 0x24;

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

                healthMaxAuto->Enable();
                entry->SetName("Reset to 9 heart containers");

                promptHP_Refill(customMaxHealth);
                preventOverflowHP(customMaxHealth);

                // First immediate write before the auto code handles loading screens
                Process::Write8(AddressList::getAddress("HealthMax"), customMaxHealth);
                Process::Write8(AddressList::getAddress("HealthMaxCostumeEffect"), 0x0);
            }
        }
        else
        {
            healthMaxAuto->Disable();

            // restore max HP value...
            Process::Write8(AddressList::getAddress("HealthMax"), defaultMaxHP);
            preventOverflowHP(defaultMaxHP);

            entry->SetName("Set maximum heart containers");
        }
    }

    // Helper function to refill HP after setting custom maximum HP value
    void promptHP_Refill(int maxHP)
    {
        u8 currentHealth = GeneralHelpers::getHP();

        if (currentHealth < maxHP)
        {
            if (GeneralHelpers::showMsgKbd("Refill hearts?", DialogType::DialogYesNo))
                Process::Write8(AddressList::getAddress("HealthCurrent"), maxHP);
        }
    }

    // Helper function to prevent current HP value from surpassing custom maximum HP value
    void preventOverflowHP(int maxHP)
    {
        u8 currentHealth = GeneralHelpers::getHP();

        if (currentHealth > maxHP)
            Process::Write8(AddressList::getAddress("HealthCurrent"), maxHP);
    }

    // Sets maximum HP value in memory
    void Gameplay::writeMaxHealth(MenuEntry *entry)
    {
        if (GeneralHelpers::isLoadingScreen(false))
        {
            Process::Write8(AddressList::getAddress("HealthMax"), customMaxHealth);
            Process::Write8(AddressList::getAddress("HealthMaxCostumeEffect"), 0x0);
        }
        if (Level::getElapsedTime() == 0)
        {
            preventOverflowHP(customMaxHealth);
        }
    }

    // Freezes current HP value to the maximum value
    void Gameplay::infHealth(MenuEntry *entry)
    {
        u8 maxHealth, maxHealthCostumeEffect;
        Process::Read8(AddressList::getAddress("HealthMax"), maxHealth);
        Process::Read8(AddressList::getAddress("HealthMaxCostumeEffect"), maxHealthCostumeEffect);
        u8 healthToWrite = maxHealth + (s8)maxHealthCostumeEffect;
        Process::Write8(AddressList::getAddress("HealthCurrent"), healthToWrite);
    }

    // Freezes current number of fairies to 9 (0xA because 0 fairies starts at 0x1)
    void Gameplay::infFairy(MenuEntry *entry)
    {
        Process::Write8(AddressList::getAddress("FairiesCurrent"), 0xA);
    }
}