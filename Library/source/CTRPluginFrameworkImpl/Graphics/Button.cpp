#include "CTRPluginFrameworkImpl/Graphics/Button.hpp"

namespace CTRPluginFramework
{
    Button::Button(u32 type, const std::string &content, const IntRect &bounds, IconCallback icon) :
        _icon{(void *)icon}, _content{content}, _bounds{bounds}
    {
        float width = 0;

        _isEnabled = true;
        _isToggleBtn = type & Toggle;
        _isRounded = type & Rounded;
        _acceptSoundEvent = SoundEngine::Event::ACCEPT;

        if (type & Icon)
        {
            _isIconBtn = true;
            _posX = bounds.leftTop.x;
            _posY = bounds.leftTop.y;
        }

        width = Renderer::GetTextSize(content.c_str());
        _posY = _bounds.leftTop.y + ((_bounds.size.y - 16) >> 1);

        if (icon != nullptr)
            width += 18.f;

        _limit = _bounds.leftTop.x + _bounds.size.x;
    }

    // Shitty but avoid casts in code
    Button::Button(u32 type, const std::string &content, const IntRect &bounds, IconCallbackState icon):
        Button(type, content, bounds, (IconCallback)icon)
    {}

    Button::Button(u32 type, const IntRect &bounds, IconCallback icon) :
        _icon{(void *)icon},  _bounds{bounds}
    {
        _isEnabled = true;
        _isToggleBtn = type & Toggle;
        _isRounded = type & Rounded;
        _acceptSoundEvent = SoundEngine::Event::ACCEPT;

        if (type & Icon)
        {
            _isIconBtn = true;
            _posX = bounds.leftTop.x;
            _posY = bounds.leftTop.y;
            return;
        }
    }

    // Shitty but avoid casts in code
    Button::Button(u32 type, const IntRect &bounds, IconCallbackState icon) :
        Button(type, bounds, (IconCallback)icon)
    {}

    Button::~Button(void)
    {
    }

    bool    Button::operator()(void)
    {
        if (!_isEnabled || _isLocked)
            return false;

        if (!_isToggleBtn)
            _state = false;

        bool ret = _execute;
        _execute = false;
        if (ret && !_isToggleBtn)
        {
            SoundEngine::PlayMenuSound(_acceptSoundEvent);
            _isPressed = _wasPressed = false;
        }
        return ret;
    }

    void    Button::Draw(void)
    {
        int posY = _posY;

        if (!_isEnabled)
            return;

        if (_isIconBtn && _icon != nullptr)
        {
            reinterpret_cast<IconCallbackState>(_icon)(_posX, _posY, _isToggleBtn ? _state : _isPressed);
            return;
        }

        const Color &textColor = Color::Gainsboro;
        const Color &fillColor = _isLocked ? Color::Gray :
                                (_isPressed ? Color::Maroon :
                                (_isToggleBtn && _state ? Color::LightMagenta : Color::Magenta));

        // Content and BG
        if (_icon != nullptr)
            reinterpret_cast<IconCallbackState>(_icon)(_bounds.leftTop.x, _bounds.leftTop.y, _isToggleBtn ? _state : _isPressed);
        else
        {
            if (_isRounded)
                Renderer::DrawRoundedRectangle(_bounds, Color::Magenta, fillColor);
            else
            {
                Renderer::DrawRect(_bounds, fillColor);
                Renderer::DrawRect(_bounds, Color::Magenta, false);
            }
        }

        int centeredX = _bounds.leftTop.x + ((_bounds.size.x - (int)Renderer::GetTextSize(_content.c_str())) >> 1);
        Renderer::DrawGameFontString(_content.c_str(), centeredX, posY, _limit, textColor);
    }

    void    Button::Update(const bool isTouchDown, const IntVector &touchPos)
    {
        static Time halfSecond = Seconds(0.5f);

        if (!_isEnabled || _isLocked)
            return;

        if (_isPressed && !isTouchDown)
        {
            _isPressed = false;
            _state = !_state;
            if (_isToggleBtn)
            {
                if (_state)
                    SoundEngine::PlayMenuSound(SoundEngine::Event::SELECT);
                else
                    SoundEngine::PlayMenuSound(SoundEngine::Event::DESELECT);
            }
            _execute = true;
            _clock.Restart();
            return;
        }

        if (_isPressed != _wasPressed && !_isToggleBtn)
        {
            if (_isPressed)
                SoundEngine::PlayMenuSound(SoundEngine::Event::SELECT);
            else
                SoundEngine::PlayMenuSound(SoundEngine::Event::DESELECT);
        }
        _wasPressed = _isPressed;

        if (_isToggleBtn && !_clock.HasTimePassed(halfSecond))
            return;

        _isPressed = _bounds.Contains(touchPos) && isTouchDown;
    }

    void    Button::Enable(bool isEnabled)
    {
        _isEnabled = isEnabled;
    }

    void    Button::Enable(void)
    {
        _isEnabled = true;
    }

    void    Button::Disable(void)
    {
        _isEnabled = false;
    }

    void    Button::Lock(void)
    {
        _isLocked = true;
    }

    void    Button::Unlock(void)
    {
        _isLocked = false;
    }

    void    Button::Execute(void)
    {
        _execute = true;
    }

    void    Button::SetState(bool state)
    {
        _state = state;
    }

    bool    Button::GetState(void) const
    {
        return _state;
    }
    void Button::SetAcceptSoundEvent(SoundEngine::Event event)
    {
        _acceptSoundEvent = event;
    }
}
