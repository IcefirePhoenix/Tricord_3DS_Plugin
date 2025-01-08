#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // TODO: duplicate code... combine both to use common logic...
    // Force-sets Coliseum Win Count to custom amount
    void Save::heroPointCountSet(MenuEntry *entry)
    {
        u32 result;
        Keyboard heroPointInput("Set Hero Point count:");
        heroPointInput.IsHexadecimal(false);

        heroPointInput.OnKeyboardEvent([](Keyboard &kb, KeyboardEvent &event)
        {
            if (event.type == KeyboardEvent::CharacterAdded)
            {
                std::string &input = kb.GetInput();
                int value = std::stoi(input);

                if (value > 999)
                    input = "999";
                else if (value < 0)
                    input = "0";
            }
        });

        // display edits in menu
        if (heroPointInput.Open(result) == 0)
        {
            Process::Write32(AddressList::HeroPointCount.addr, result);
            entry->SetName("Edit Hero Point count: " + std::to_string(result));
        }
    }

    // Force-sets Coliseum Win Count to custom amount
    void Save::coliseumWinCountSet(MenuEntry *entry)
    {
        u32 result;
        Keyboard coliseumWinInput("Set Coliseum Win count:");
        coliseumWinInput.IsHexadecimal(false);

        coliseumWinInput.OnKeyboardEvent([](Keyboard &kb, KeyboardEvent &event)
        {
            if (event.type == KeyboardEvent::CharacterAdded)
            {
                std::string &input = kb.GetInput();
                int value = std::stoi(input);

                if (value > 999)
                    input = "999";
                else if (value < 0)
                    input = "0";
            }
        });

        // display edits in menu...
        if (coliseumWinInput.Open(result) == 0)
        {
            Process::Write32(AddressList::ColiseumWinCount.addr, result);
            entry->SetName("Edit Coliseum Win count: " + std::to_string(result));
        }
    }

    // TODO: name changer using custom keyboard
    // TODO: rupee amount changer
}