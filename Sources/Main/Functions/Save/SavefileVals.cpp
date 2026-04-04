#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    bool promptSaveFileCommonVal(std::string title, std::string desc, u32& result)
    {
        Keyboard prompt(title, desc);

        prompt.IsHexadecimal(false);
        prompt.DisableSignKey();

        prompt.OnKeyboardEvent([](Keyboard &kb, KeyboardEvent &event)
        {
            if (event.type == KeyboardEvent::CharacterAdded)
            {
                GeneralHelpers::clampIntInput(kb.GetInput(), 0, 999);
            }
        });

        if (prompt.Open(result, 30) == 0)
            return true;
        else
            return false;
    }

    // Force-sets Coliseum Win Count to custom amount
    void Save::heroPointCountSet(MenuEntry *entry)
    {
        u32 result;

        if (promptSaveFileCommonVal("Hero Point Modifier", "Input a new Hero Point count.", result))
        {
            Process::Write32(AddressList::getAddress("HeroPointCount"), result);
            entry->SetName("Edit Hero Point count: " + std::to_string(result));
        }
    }

    // Force-sets Coliseum Win Count to custom amount
    void Save::coliseumWinCountSet(MenuEntry *entry)
    {
        u32 result;

        if (promptSaveFileCommonVal("Coliseum Win Count Modifier", "Input a new Coliseum Win count.", result))
        {
            Process::Write32(AddressList::getAddress("ColiseumWinCount"), result);
            entry->SetName("Edit Coliseum Win count: " + std::to_string(result));
        }
    }

    // TODO: name changer using custom keyboard
    // TODO: rupee amount changer
}