#include "Main/Item.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    float currStrafingSpeeds[6];

    /* ------------------ */

    // Helper function that backs up current strafing speeds
    void readCurrStrafingSpeeds(void)
    {
        Item::initItemAddresses();

        for (int iterator = 0; iterator < 6; iterator++)
            Process::ReadFloat(Item::addresses[iterator], currStrafingSpeeds[iterator]);
    }

    // Helper function that resets all strafing speeds
    void resetAllStrafingSpeeds(void)
    {
        float defaultStrafingSpeed = 1.0;

        for (int iterator = 0; iterator < 6; iterator++)
            Process::WriteFloat(Item::addresses[iterator], defaultStrafingSpeed);
    }

    // Force-sets strafing speeds
    void Item::strafingSpeedSet(MenuEntry *entry)
    {
        float result = 1.0;

        const StringVector items =
        {
            "Reset all speeds",
            "Bow",
            "Fire Gloves",
            "Water Rod",
            "Gust Jar",
            "Gripshot",
            "Hammer"
        };

        readCurrStrafingSpeeds();

        std::string msg = "Select an item to change its strafing speed.\nDefault speed for all items: 1.0";
        msg.append("\nBow: " + std::to_string(currStrafingSpeeds[0]));
        msg.append("\nFire Gloves: " + std::to_string(currStrafingSpeeds[1]));
        msg.append("\nWater Rod: " + std::to_string(currStrafingSpeeds[2]));
        msg.append("\nGust Jar: " + std::to_string(currStrafingSpeeds[3]));
        msg.append("\nGripshot: " + std::to_string(currStrafingSpeeds[4]));
        msg.append("\nHammer: " + std::to_string(currStrafingSpeeds[5]));

        Keyboard speed(msg);
        speed.Populate(items);

        int choice = speed.Open();

        if (choice == 0)
            resetAllStrafingSpeeds();
        else if (choice > 0)
        {
            result = setSpeedKB();
            Process::WriteFloat(Item::addresses[choice - 1], result);
        }
    }

    // Menu interface for inputting strafing speed values
    float setSpeedKB(void)
    {
        float result;

        Keyboard speedKB("Set strafing speed:");
        speedKB.IsHexadecimal(false);

        if (speedKB.Open(result) == 0)
        {
            if (result < 0.0)
                MessageBox(Color::Gainsboro << "Error", "Strafing speed cannot be negative.")();
            else
                return result;
        }
        return 1.0;
    }
}