#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // TODO: duplicate code... combine both to use common logic...
    // Force-sets Coliseum Win Count to custom amount
    void Save::heroPointCountSet(MenuEntry *entry)
    {
        u32 result;
        Keyboard heroPointInput("Hero Point Modifier", "Input a new Hero Point count.");
        heroPointInput.IsHexadecimal(false);

        heroPointInput.OnKeyboardEvent([](Keyboard &kb, KeyboardEvent &event)
        {
            if (event.type == KeyboardEvent::CharacterAdded)
            {
                GeneralHelpers::clampIntInput(kb.GetInput(), 0, 999);
            }
        });

        // display edits in menu
        if (heroPointInput.Open(result, 30) == 0)
        {
            Process::Write32(AddressList::getAddress("HeroPointCount"), result);
            entry->SetName("Edit Hero Point count: " + std::to_string(result));
        }
    }

    // Force-sets Coliseum Win Count to custom amount
    void Save::coliseumWinCountSet(MenuEntry *entry)
    {
        u32 result;
        Keyboard coliseumWinInput("Coliseum Win Count Modifier", "Input a new Coliseum Win count.");
        coliseumWinInput.IsHexadecimal(false);

        coliseumWinInput.OnKeyboardEvent([](Keyboard &kb, KeyboardEvent &event)
        {
            if (event.type == KeyboardEvent::CharacterAdded)
            {
                GeneralHelpers::clampIntInput(kb.GetInput(), 0, 999);
            }
        });

        // display edits in menu...
        if (coliseumWinInput.Open(result, 100) == 0)
        {
            Process::Write32(AddressList::getAddress("ColiseumWinCount"), result);
            entry->SetName("Edit Coliseum Win count: " + std::to_string(result));
        }
    }

    // TODO: name changer using custom keyboard
    // TODO: rupee amount changer
}