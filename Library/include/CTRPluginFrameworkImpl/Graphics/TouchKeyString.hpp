#ifndef CTRPLUGINFRAMEWORKIMPL_TOUCHKEYSTRING_KEYBOARD_HPP
#define CTRPLUGINFRAMEWORKIMPL_TOUCHKEYSTRING_KEYBOARD_HPP

#include "CTRPluginFrameworkImpl/Graphics/Drawable.hpp"
#include "CTRPluginFramework/Graphics/Color.hpp"
#include "CTRPluginFrameworkImpl/Graphics.hpp"

#include "CTRPluginFramework/System/Touch.hpp"
#include "CTRPluginFramework/Sound.hpp"

#include <string>

namespace CTRPluginFramework
{
    class TouchKeyString
    {
    public:
        TouchKeyString(const std::string &content, IntRect ui, bool enabled = true);
        ~TouchKeyString(){}

        void    Enable(bool enable = true);

        // Force key being pressed (graphically only)
        void ForcePressed(bool force);

        bool    CanUse(void);
        void    Draw(void);
        void    Update(const bool touchIsDown, const IntVector &touchPos);
        void    Scroll(float amount);

        void    GetPosition(u16& posX, float& posY);
		void	SetPosition(u16 posX, float posY);

        // Return -1 if not pressed, 1 otherwise
        int    operator()(void);

        void    SetAcceptSoundEvent(SoundEngine::Event event);
        SoundEngine::Event GetAcceptSoundEvent();

    private:
        std::string     _content;

        IntRect         _uiProperties;
        SoundEngine::Event      _acceptSoundEvent;

        bool            _isPressed;
        bool            _wasPressed;
        bool            _isForcePressed;
        bool            _execute;
        bool            _enabled;
        u16             _posX;
        float           _contentLength;
        float           _posY;

        TouchKeyString(IntRect ui, bool isEnabled);
    };
}

#endif
