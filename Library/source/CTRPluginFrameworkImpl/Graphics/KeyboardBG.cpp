#include "CTRPluginFrameworkImpl/Graphics/KeyboardBG.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFramework/Graphics/Color.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Icon.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFramework/Graphics/OSD.hpp"

#include "CTRPluginFramework/System/Lock.hpp"

namespace CTRPluginFramework
{
    KeyboardBG::KeyboardBG(u32 posX, u32 posY, u32 width, u32 height, bool closeBtn) :
        _rect2(posX, posY, width, height),
        _border2(posX + 2, posY + 2, width - 4, height - 4),
        _image2(Preferences::bottomBoxBGImage)

    {
    }

    void    KeyboardBG::Draw(void) const
    {
        // Background
        if (_image2 != nullptr) {
            _image2->Draw(_rect2.leftTop);
        }
        else
        {
            Renderer::DrawRect2(_rect2, Preferences::Settings.BackgroundMainColor, Preferences::Settings.BackgroundSecondaryColor);
            Renderer::DrawRect(_border2, Preferences::Settings.BackgroundBorderColor, false);
        }
    }  
}
