#ifndef CTRPLUGINFRAMEWORKIMPL_KEYBOARDBG_HPP
#define CTRPLUGINFRAMEWORKIMPL_KEYBOARDBG_HPP

#include "CTRPluginFramework/System/Vector.hpp"
#include "CTRPluginFramework/System/Rect.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Button.hpp"
#include <string>

namespace CTRPluginFramework
{
    class BMPImage;
    class KeyboardBG
    {
    public:

        KeyboardBG(u32 posX, u32 posY, u32 width, u32 height, bool closeBtn);

        void    Draw(void) const;
        void    Draw(const std::string& title) const;

    private:
        IntRect     _rect2;
        IntRect     _border2;
        BMPImage* _image2;

    };
}

#endif