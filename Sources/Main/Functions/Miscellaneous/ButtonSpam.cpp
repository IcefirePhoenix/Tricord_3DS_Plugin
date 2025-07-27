#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    u16 spamHotkeys = 0;
    float interval = 0.01;
    Clock timer;

    /* ------------------ */

    // Allows press-hold button spamming for specific buttons at specified intervals
    void Miscellaneous::buttonSpammer(MenuEntry *entry)
    {
        // documentation in spreadsheet; A B X Y L R
        u8 keyBits[] = {0x1, 0x2, 0x8, 0x10, 0x20, 0x40};
        u8 ABXY_status, othersStatus, forceClearStatus = 0;

        if (entry->WasJustActivated())
            timer.Restart();

        if (timer.HasTimePassed(Seconds(interval)))
        {
            Process::Read8(AddressList::getAddress("ABXY"), ABXY_status);
            Process::Read8(AddressList::getAddress("StartSelLRTouch"), othersStatus);

            // clear per-button status if spam enabled...
            for (int i = 0; i < 6; i++)
            {
                if (Controller::IsKeyDown(keys[i]) && (spamHotkeys & (BIT(i))))
                {
                    Controller::ClearKeys(); // to emulate button releases

                    if (i < 4)
                        ABXY_status &= ~keyBits[i];
                    else
                        othersStatus &= ~keyBits[i];
                }
            }

            Process::Write8(AddressList::getAddress("ABXY"), ABXY_status);
            Process::Write8(AddressList::getAddress("StartSelLRTouch"), othersStatus);
            timer.Restart();
        }
    }

    // Set input spam interval based on preset timers
    void Miscellaneous::selSpamInterval(MenuEntry *entry)
    {
        StringVector intervals =
        {
            "10 ms",
            "50 ms",
            "100 ms",
            "150 ms"
        };

        Keyboard timeSel("Buttom Spammer Timer", "Select the Button Spammer's timer interval.");
        timeSel.Populate(intervals);

        int choice = timeSel.Open();
        switch (choice)
        {
            case 0:
                interval = .01;
                entry->SetName("Set custom timer interval: 10 ms");
                break;
            case 1:
                interval = .05;
                entry->SetName("Set custom timer interval: 50 ms");
                break;
            case 2:
                interval = .1;
                entry->SetName("Set custom timer interval: 100 ms");
                break;
            case 3:
                interval = .15;
                entry->SetName("Set custom timer interval: 150 ms");
                break;
            default:
                break;
        }
    }

    // Menu interface for selecting desired keys to enable spam inputs
    void Miscellaneous::selectSpamKeys(MenuEntry *entry)
    {
        std::string title;
        StringVector bottomScreenOptions;
        Keyboard kbd("Button Spammer Hotkey Modifier", "Use the toggles below to choose which keys to spam.");

        kbd.CanAbort(false);
        bool loop = true;

        while (loop)
        {
            bottomScreenOptions.clear();
            bottomScreenOptions.push_back("Save changes");
            bottomScreenOptions.push_back(std::string(FONT_A) << " " << (spamHotkeys & BUTTON_A ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string(FONT_B) << " " << (spamHotkeys & BUTTON_B ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string(FONT_X) << " " << (spamHotkeys & BUTTON_X ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string(FONT_Y) << " " << (spamHotkeys & BUTTON_Y ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string(FONT_L) << " " << (spamHotkeys & BUTTON_L ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string(FONT_R) << " " << (spamHotkeys & BUTTON_R ? ENABLED_SLIDER : DISABLED_SLIDER));

            kbd.Populate(bottomScreenOptions);

            switch (kbd.Open())
            {
                case 0:
                    loop = false;
                    break;
                case 1:
                    spamHotkeys ^= BUTTON_A;
                    break;
                case 2:
                    spamHotkeys ^= BUTTON_B;
                    break;
                case 3:
                    spamHotkeys ^= BUTTON_X;
                    break;
                case 4:
                    spamHotkeys ^= BUTTON_Y;
                    break;
                case 5:
                    spamHotkeys ^= BUTTON_L;
                    break;
                case 6:
                    spamHotkeys ^= BUTTON_R;
                    break;
                default:
                    break;
            }
        }
    }
}