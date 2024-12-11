#include "Helpers.hpp"

namespace CTRPluginFramework
{
    /* vars*/
    u16 spamHotkeys = 0;
    float interval = 0.01;
    Clock timer;

    /* functions*/
    void Miscellaneous::buttonSpammer(MenuEntry *entry)
    {
        u8 keyBits[] = {0x1, 0x2, 0x8, 0x10, 0x20, 0x40, 0x8, 0x80};
        u8 ABXY_status, othersStatus, forceClearStatus = 0;

        if (entry->WasJustActivated())
            timer.Restart();

        // timer update behavior...
        if (timer.HasTimePassed(Seconds(interval)))
        {
            // get current button input statuses...
            Process::Read8(AddressList::ABXY.addr, ABXY_status);
            Process::Read8(AddressList::StartSelLRTouch.addr, othersStatus);

            // clear per-button status if spam enabled...
            for (int i = 0; i < 8; i++)
            {
                if (Controller::IsKeyDown(keys[i]) && (spamHotkeys & (1 << i)))
                {
                    if (i < 4)
                        ABXY_status &= ~keyBits[i];
                    else
                        othersStatus &= ~keyBits[i];
                }
            }

            // update with new statuses...
            Process::Write8(AddressList::ABXY.addr, forceClearStatus);
            Process::Write8(AddressList::StartSelLRTouch.addr, forceClearStatus);
            timer.Restart();
        }
    }

    void Miscellaneous::selSpamInterval(MenuEntry *entry)
    {
        StringVector intervals =
        {
            "10 ms",
            "50 ms",
            "100 ms",
            "150 ms"
        };

        Keyboard timeSel("Select Button Spammer's timer interval:");
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

    void Miscellaneous::selectSpamKeys(MenuEntry *entry)
    {
        std::string title;
        StringVector bottomScreenOptions;
        Keyboard kbd("Button Spammer Hotkey Menu");

        kbd.CanAbort(false);
        bool loop = true;

        while (loop)
        {
            title = "Button Spammer Hotkey Menu\n\nUse the toggles to choose which keys to spam:\n\n";

            bottomScreenOptions.clear();
            bottomScreenOptions.push_back("Save changes");
            bottomScreenOptions.push_back(std::string(FONT_A) << " " << (spamHotkeys & BUTTON_A ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string(FONT_B) << " " << (spamHotkeys & BUTTON_B ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string(FONT_X) << " " << (spamHotkeys & BUTTON_X ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string(FONT_Y) << " " << (spamHotkeys & BUTTON_Y ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string(FONT_L) << " " << (spamHotkeys & BUTTON_L ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string(FONT_R) << " " << (spamHotkeys & BUTTON_R ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string("Start/Select ") << (spamHotkeys & BUTTON_STARTSEL ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string("Touchscreen ") << (spamHotkeys & BUTTON_TOUCH ? ENABLED_SLIDER : DISABLED_SLIDER));

            kbd.GetMessage() = title;
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
            case 7:
                spamHotkeys ^= BUTTON_STARTSEL;
                break;
            case 8:
                spamHotkeys ^= BUTTON_TOUCH;
                break;
            default:
                break;
            }
        }
    }
}