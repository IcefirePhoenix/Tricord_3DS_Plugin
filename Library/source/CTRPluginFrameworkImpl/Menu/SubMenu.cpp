#include "CTRPluginFrameworkImpl/Menu/SubMenu.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFrameworkImpl/System/Events.hpp"
#include "CTRPluginFramework/Sound.hpp"

namespace CTRPluginFramework
{
    SubMenu::SubMenu(const StringVector &options) :
        _options(options), _selector{ 0 }, _isOpen{ false }, _action{ false }
    {
    }

    SubMenu::~SubMenu()
    {
    }

    int     SubMenu::operator()(void)
    {
        bool action = _action;

        _action = false;
        return action ? _selector : -1;
    }

    void    SubMenu::Draw(void)
    {
        if (!_isOpen)
            return;

        int posY = 25;

        static IntRect background(233, 20, 137, 200);
        static IntRect border(235, 22, 133, 196);

        const Color    &black = Color::Black;
        const Color    &darkgrey = Color::DarkGrey;
        const Color    &gainsboro = Color::Gainsboro;
        const Color    &textcolor = Preferences::Settings.MainTextColor;

        Renderer::SetTarget(TOP);

        // Draw background
        Renderer::DrawRect2(background, Preferences::Settings.BackgroundMainColor, Preferences::Settings.BackgroundSecondaryColor);
        Renderer::DrawRect(border, Preferences::Settings.BackgroundBorderColor, false);


        const int xx = Renderer::DrawGameFontString("Options", 243, posY, 340, textcolor);
        Renderer::DrawLine(243, posY, xx - 205, textcolor);

        posY = 46;

        if (_options.empty())
            return;

        const IntRect selRect = IntRect(241, 45 + _selector * 16, 120, 20);

        for (u32 i = 0; i < _options.size(); i++)
        {
            const char *str = _options[i].c_str();

            if (i == _selector)
            {
                // If an action is selected, create a little effect (yeah, very little)
                if (!_fadeClock.HasTimePassed(Seconds(0.2f)))
                {
                    Renderer::DrawRect(selRect, gainsboro);
                    Renderer::DrawRect(selRect, darkgrey, false);
                    Renderer::DrawGameFontString(str, 243, posY, 370, black);
                    continue;
                }
                else
                {
                    // Draw selector
                    Renderer::DrawRect(selRect, darkgrey, false);
                }
            }
            Renderer::DrawGameFontString(str, 243, posY, 370, textcolor);
        }
    }

    void    SubMenu::ProcessEvent(const Event &event)
    {
        if (!_isOpen)
        {
            if (event.type == event.KeyPressed && event.key.code == Key::X)
                _isOpen = true;
            return;
        }

        if (event.type == Event::EventType::KeyPressed)
        {
            switch (event.key.code)
            {
            case Key::DPadUp:
                _selector = std::max((int)_selector - 1, (int)0);
                SoundEngine::PlayMenuSound(SoundEngine::Event::CURSOR);
                break;
            case Key::DPadDown:
                _selector = std::min((u32)_selector + 1, (u32)_options.size() - 1);
                SoundEngine::PlayMenuSound(SoundEngine::Event::CURSOR);
                break;
            case Key::A:
                _action = true;
                SoundEngine::PlayMenuSound(SoundEngine::Event::ACCEPT);
                _fadeClock.Restart();
                break;
            case Key::B:
                _isOpen = false;
                SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);
                break;
            default:
                break;
            }
        }
    }

    bool    SubMenu::IsOpen(void)
    {
        return _isOpen;
    }

    void    SubMenu::ChangeOptions(const StringVector &options)
    {
        _selector = 0;
        _options.clear();
        _options = options;
    }

    u32     SubMenu::OptionsCount(void)
    {
        return _options.size();
    }
}
