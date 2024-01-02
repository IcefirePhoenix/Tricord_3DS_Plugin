#include "CTRPluginFrameworkImpl/Menu/HotkeysModifier.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Icon.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Window.hpp"
#include "CTRPluginFramework/System/Controller.hpp"
#include "Unicode.h"
#include "CTRPluginFramework/System/System.hpp"
#include "CTRPluginFrameworkImpl/System/SystemImpl.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFramework/Sound.hpp"

namespace CTRPluginFramework
{
    static const Key ktable[22] =
    {
        L, DPadUp, DPadLeft, DPadRight, DPadDown,
        R, X, Y, A, B,
        ZL, CPadUp, CPadLeft, CPadRight, CPadDown,
        ZR, CStickUp, CStickLeft, CStickRight, CStickDown,
        Start, Select
    };

    static const char *stable[22] =
    {
        FONT_L, FONT_DU, FONT_DL, FONT_DR, FONT_DD,
        FONT_R, FONT_X, FONT_Y, FONT_A, FONT_B,
        FONT_ZL, "\uE077 Up", "\uE077 Left", "\uE077 Right", "\uE077 Down",
        FONT_ZR, "CS Up", "CS Left", "CS Right", "CS Down",
        "Start", "Select"
    };

    static int    GetIndex(int code)
    {
        for (int i = 0; i < 18; ++i)
            if (ktable[i] == code)
                return (i);

        return (0); ///< shouldn"t happen (unsafe code, bad)
    }

    HotkeysModifier::HotkeysModifier(u32 &keys, const std::string &message) :
    _keys(keys), _message(message)
    {
        FwkSettings &settings = FwkSettings::Get();

        for (int i = 0, posY = 88; i < 5; ++i, posY += 25)
        {
            Button b(Button::Icon | Button::Toggle, IntRect(30, posY, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(b);
        }
        for (int i = 0, posY = 88; i < 5; ++i, posY += 25)
        {
            Button b(Button::Icon | Button::Toggle, IntRect(80, posY, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(b);
        }
        for (int i = 0, posY = 88; i < 5; ++i, posY += 25)
        {
            Button b(Button::Icon | Button::Toggle, IntRect(130, posY, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(b);
        }
        for (int i = 0, posY = 88; i < 5; ++i, posY += 25)
        {
            Button b(Button::Icon | Button::Toggle, IntRect(215, posY, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(b);
        }
        for (int i = 0, posX = 95; i < 2; ++i, posX += 70)
        {
            Button b(Button::Icon | Button::Toggle, IntRect(posX, 215, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(b);
        }
        for (int i = 0; i < 24; ++i)
        {
            if (keys & (1u << i))
            {
                _checkboxes[GetIndex(1u << i)].SetState(true);
            }
        }
        if (!System::IsNew3DS() || !settings.AreN3DSButtonsAvailable)
        {
            // Disable ZL/ZR/C-stick options on O3DS
            _checkboxes[10].Lock();
            _checkboxes[15].Lock();

            _checkboxes[16].Lock();
            _checkboxes[17].Lock();
            _checkboxes[18].Lock();
            _checkboxes[19].Lock();
        }
    }

    HotkeysModifier::~HotkeysModifier()
    {
    }

    void    HotkeysModifier::operator()(void)
    {
		bool mustclose = false;
        bool sleepClose = false;
        u32 oldKeys = _keys;
        while (((!Window::BottomWindow.MustClose() && !mustclose) || !_keys) && !sleepClose)
        {

            Controller::Update();
			mustclose = Controller::IsKeyPressed(Key::B);
            sleepClose = SystemImpl::IsSleeping();
            _DrawTop();
            _DrawBottom();
            Renderer::EndFrame();
            _Update();

            #define DPADX (Key::DPadLeft | Key::DPadRight)
            #define DPADY (Key::DPadDown | Key::DPadUp)

            u32 oldDpadX = _keys & (DPADX);
            u32 oldDpadY = _keys & (DPADY);

            _keys = 0;

            for (int i = 0; i < 18; i++)
            {
                if (_checkboxes[i].GetState())
                    _keys |= ktable[i];
            }

            // Only keep new DPAD keys

            if ((_keys & DPADX) && oldDpadX && (_keys & DPADX) != oldDpadX)
            {
                _keys ^= oldDpadX;

                auto& checkbox = _checkboxes[GetIndex(oldDpadX)];

                checkbox.SetState(false);
            }

            if ((_keys & DPADY) && oldDpadY && (_keys & DPADY) != oldDpadY)
            {
                _keys ^= oldDpadY;

                auto& checkbox = _checkboxes[GetIndex(oldDpadY)];

                checkbox.SetState(false);
            }
        }
        if (sleepClose) _keys = oldKeys;
        SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);
    }

    void    HotkeysModifier::_DrawTop(void) const
    {
        Renderer::SetTarget(TOP);
        Window::TopWindow.Draw("Hotkey Modifier");

        int posY = 61;
        Renderer::DrawSysStringReturn((const u8*)_message.c_str(), 40, posY, 335, Preferences::Settings.MainTextColor);
    }

    void    HotkeysModifier::_DrawBottom(void)
    {
        Renderer::SetTarget(BOTTOM);
        Window::BottomWindow.Draw();

        // Draw CheckBoxes
        for (auto it = _checkboxes.begin(); it != _checkboxes.end(); it++)
            (*it).Draw();

        // Draw labels
        for (int i = 0, posY = 88; i < 5; ++i, posY += 9)
            Renderer::DrawSysString(stable[i], 50, posY, 290, Preferences::Settings.MainTextColor);
        for (int i = 5, posY = 88; i < 10; ++i, posY += 9)
            Renderer::DrawSysString(stable[i], 100, posY, 290, Preferences::Settings.MainTextColor);
        for (int i = 10, posY = 88; i < 15; ++i, posY += 9)
            Renderer::DrawSysString(stable[i], 150, posY, 290, Preferences::Settings.MainTextColor);
        for (int i = 15, posY = 88; i < 20; ++i, posY += 9)
            Renderer::DrawSysString(stable[i], 235, posY, 300, Preferences::Settings.MainTextColor);

        int startY = 215;
        int selY = 215;

        Renderer::DrawSysString(stable[20], 113, startY, 290, Preferences::Settings.MainTextColor);
        Renderer::DrawSysString(stable[21], 185, selY, 290, Preferences::Settings.MainTextColor);
    }

    void    HotkeysModifier::_Update(void)
    {
        bool        isTouched = Touch::IsDown();
        IntVector   touchPos(Touch::GetPosition());

        for (auto it = _checkboxes.begin(); it != _checkboxes.end(); it++)
            (*it).Update(isTouched, touchPos);

        Window::BottomWindow.Update(isTouched, touchPos);
    }
}
