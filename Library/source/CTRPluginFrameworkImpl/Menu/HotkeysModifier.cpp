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
        R, A, B, X, Y,
        ZL, CPadUp, CPadLeft, CPadRight, CPadDown,
        ZR, CStickUp, CStickLeft, CStickRight, CStickDown,
        Start, Select
    };

    static const char *stable[12] =
    {
        FONT_L, FONT_R,
        FONT_A, FONT_B, FONT_X, FONT_Y,
        "C-Stick Up", "C-Stick Left", "C-Stick Right", "C-Stick Down",
        "Start", "Select"
    };

    static int GetIndex(int code)
    {
        for (int i = 0; i < 22; ++i)
            if (ktable[i] == code)
                return (i);

        return (0); // seems to be a lazy placeholder = returns Key::L in case of failure... will not be editing this
    }

    HotkeysModifier::HotkeysModifier(u32 &keys, const std::string &message) :
    _keys(keys), _message(message)
    {
        FwkSettings &settings = FwkSettings::Get();

        for (int i = 0, posY = 48; i < 5; ++i, posY += 25)
        {
            Button b(Button::Icon | Button::Toggle, IntRect(35, posY, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(b);
        }
        for (int i = 0, posY = 48; i < 5; ++i, posY += 25)
        {
            Button b(Button::Icon | Button::Toggle, IntRect(85, posY, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(b);
        }
        for (int i = 0, posY = 48; i < 5; ++i, posY += 25)
        {
            Button b(Button::Icon | Button::Toggle, IntRect(135, posY, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(b);
        }
        for (int i = 0, posY = 48; i < 5; ++i, posY += 25)
        {
            Button b(Button::Icon | Button::Toggle, IntRect(185, posY, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(b);
        }
        for (int i = 0, posX = 100; i < 2; ++i, posX += 70)
        {
            Button b(Button::Icon | Button::Toggle, IntRect(posX, 175, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(b);
        }
        for (int i = 0; i < 32; ++i)
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

    void HotkeysModifier::operator()(void)
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

            for (int i = 0; i < 22; i++)
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

        if (sleepClose)
            _keys = oldKeys;

        SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);
    }

    void HotkeysModifier::_DrawTop(void) const
    {
        Renderer::SetTarget(TOP);
        Window::TopWindow.Draw("Hotkey Modifier");

        int posY = 55;
        Renderer::DrawGameFontStringReturn(_message.c_str(), 40, posY, 345, Preferences::Settings.MainTextColor);
    }

    void HotkeysModifier::_DrawBottom(void)
    {
        int originalPosY = 48;
        Renderer::SetTarget(BOTTOM);
        Window::BottomWindow.Draw();

        // Draw checkboxes
        for (auto it = _checkboxes.begin(); it != _checkboxes.end(); it++)
            (*it).Draw();

        // Draw TEXT labels, rows 2 and 4 (rows 1 and 3 are almost entirely icon-based)
        Renderer::DrawGameFontString(stable[0], 55, originalPosY, 300, Preferences::Settings.MainTextColor);

        for (int i = 1, posY = 48; i < 6; ++i, posY += 9)
            Renderer::DrawGameFontString(stable[i], 105, posY, 290, Preferences::Settings.MainTextColor);
        for (int i = 6, posY = 73; i < 10; ++i, posY += 9)
            Renderer::DrawGameFontString(stable[i], 205, posY, 300, Preferences::Settings.MainTextColor);

        int startY = 175;
        int selY = 175;

        Renderer::DrawGameFontString(stable[10], 118, startY, 290, Preferences::Settings.MainTextColor);
        Renderer::DrawGameFontString(stable[11], 190, selY, 290, Preferences::Settings.MainTextColor);

        // Draw hotkey icons (D-Pad, C-Pad, L/R, ZL/ZR)
        Icon::DrawDPad(55, 73); // row 1 start
        Icon::DrawDPad(55, 98, Icon::ROT_270);
        Icon::DrawDPad(55, 123, Icon::ROT_90);
        Icon::DrawDPad(55, 148, Icon::ROT_180);

        Icon::DrawZL(155, 48); // row 3 start
        Icon::DrawCPad(155, 73);
        Icon::DrawCPad(155, 98, Icon::ROT_270);
        Icon::DrawCPad(155, 123, Icon::ROT_90);
        Icon::DrawCPad(155, 148, Icon::ROT_180);

        Icon::DrawZR(205, 48); // row 4, icon oddball
    }

    void HotkeysModifier::_Update(void)
    {
        bool isTouched = Touch::IsDown();
        IntVector touchPos(Touch::GetPosition());

        for (auto it = _checkboxes.begin(); it != _checkboxes.end(); it++)
            (*it).Update(isTouched, touchPos);

        Window::BottomWindow.Update(isTouched, touchPos);
    }
}
