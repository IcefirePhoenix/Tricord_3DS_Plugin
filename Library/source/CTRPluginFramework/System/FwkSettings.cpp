#include "CTRPluginFramework/System/FwkSettings.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Window.hpp"

namespace CTRPluginFramework
{
    PluginHeader *FwkSettings::Header = reinterpret_cast<PluginHeader *>(0x07000000);
    FwkSettings & FwkSettings::Get(void)
    {
        return Preferences::Settings;
    }

    void    FwkSettings::SetThemeDefault(void)
    {
        FwkSettings &settings = Get();

        // UI Colors
        settings.MenuSelectedItemColor = Color::Gainsboro;
        settings.BackgroundBorderColor = settings.WindowTitleColor = settings.MainTextColor = Color::Gainsboro;
        settings.MenuUnselectedItemColor = Color::Gainsboro;
        settings.BackgroundMainColor = Color::Magenta;
        settings.BackgroundSecondaryColor = Color::Magenta;
        settings.CursorFadeValue = 0.2f;

        // Keyboard colors
        {
            auto &kb = settings.Keyboard;

            kb.KeyBackground = kb.Background = Color::Magenta;
            kb.KeyBackgroundPressed = Color::Maroon;
            kb.Input = kb.KeyTextPressed = kb.KeyText = Color::Gainsboro;
            kb.KeyTextDisabled = Color::DimGrey;
        }

        // Custom keyboard colors
        {
            auto &kb = settings.CustomKeyboard;

            kb.BackgroundMain = Color::Magenta;
            kb.BackgroundSecondary = Color::Magenta;
            kb.BackgroundBorder = Color::Maroon;
            kb.KeyBackground = Color::Maroon;
            kb.KeyBackgroundPressed = Color::LightMagenta;
            kb.KeyText = Color::Gainsboro;
            kb.KeyTextPressed = Color::Gainsboro;
            kb.ScrollBarBackground = Color::Silver;
            kb.ScrollBarThumb = Color::DimGrey;
        }
    }

    Result  FwkSettings::SetBottomScreenBackground(void *bmpData)
    {
        BMPImage *image = new BMPImage(bmpData);

        if (!image->IsLoaded())
        {
            delete image;
            return -1;
        }

        Preferences::bottomBackgroundImage = image;
        Window::UpdateBackgrounds();
        return 0;
    }
}
