#include "CTRPluginFrameworkImpl/Menu/KeyboardImpl.hpp"
#include "CTRPluginFrameworkImpl/System/ProcessImpl.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFramework/Menu/Keyboard.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuImpl.hpp"
#include <cmath>
#include "3ds.h"

namespace CTRPluginFramework
{
    #define USER_VALID  0
    #define USER_ABORT  -1
    #define SLEEP_ABORT -2
    #define KEY_ENTER 0xA
    #define KEY_BACKSPACE 0x8
    #define KEY_PREV_PAGE -3
    #define KEY_NEXT_PAGE -4
    #define KEY_SPACE -5
    #define KEY_CAPS -6
    #define KEY_PLUS_MINUS -7
    #define KEY_ASCII_TOGGLE -8
    #define KEY_NUM_TOGGLE -10
    #define KEY_SYMBOLS_TOGGLE -11
    #define KEY_JPN_TOGGLE -12

    int charKeyWidth = 25,
        charKeyHeight = 32,
        charKeyboardStartX = 35,
        digitKeyWidthLen = 38,
        activeKeySetStartIndex = 0,
        activeKeySetEndIndex = 37;

    IntRect backspaceKeyPos(252, 17, 25, 16);

    std::vector<TouchKey> KeyboardImpl::_DecimalKeys;
    std::vector<TouchKey> KeyboardImpl::_HexadecimalFullKeys;
    std::vector<TouchKey> KeyboardImpl::_HexadecimalLiteKeys;
    std::vector<TouchKey> KeyboardImpl::_QwertyKeys;

    KeyboardImpl::KeyboardImpl(const std::string &text) : submitBtn(Button(Button::GameFont, "Submit", IntRect(190, 200, 120, 32), Icon::DrawMenuButton))
    {
        _title = "";
        _text = text;
        _error = "";
        _userInput = "";
        _scrollCursorSize = 10;
        DisplayTopScreen = true;

        // submitBtn.SetAcceptSoundEvent(SoundEngine::Event::ACCEPT);
    }

    KeyboardImpl::KeyboardImpl(const std::string &title, const std::string &text) : submitBtn(Button(Button::GameFont, "Submit", IntRect(190, 200, 120, 32), Icon::DrawMenuButton))
    {
        _title = title;
        _text = text;
        _error = "";
        _userInput = "";
        _scrollCursorSize = 10;
        DisplayTopScreen = true;

        // submitBtn.SetAcceptSoundEvent(SoundEngine::Event::ACCEPT);
    }

    KeyboardImpl::KeyboardImpl(Keyboard *kb, const std::string &title, const std::string &text) : submitBtn(Button(Button::GameFont, "Submit", IntRect(190, 200, 120, 32), Icon::DrawMenuButton))
    {
        _owner = kb;
        _title = title;
        _text = text;
        _error = "";
        _userInput = "";
        _scrollCursorSize = 10;
        DisplayTopScreen = true;

        // submitBtn.SetAcceptSoundEvent(SoundEngine::Event::ACCEPT);
    }

    KeyboardImpl::~KeyboardImpl(void)
    {
        for (TouchKeyString *tks : _strKeys)
            delete tks;
    }

    void KeyboardImpl::InitKeyboards(void)
    {
        _InitQwertySequence();
        _InitDecimalKeyboard();
        _InitHexFullKeyboard();
        _InitHexLiteKeyboard();
    };

    void    KeyboardImpl::SetLayout(Layout layout)
    {
        _canChangeLayout = false;
        _layout = layout;
        _isHex = false;
        _userInput.clear();

        switch (_layout)
        {
            case QWERTY:
                _keys = &_QwertyKeys;
                break;
            case DECIMAL:
                _keys = &_DecimalKeys;
                break;
            case HEXADECIMAL_FULL:
                _isHex = true;
                _keys = &_HexadecimalFullKeys;
                break;
            case HEXADECIMAL_LITE:
                _isHex = true;
                _keys = &_HexadecimalLiteKeys;
                break;
            default:
                break;
        }
    }

    int KeyboardImpl::GetSelectedEntry(void)
    {
        return _manualKey;
    }

    void    KeyboardImpl::SetHexadecimal(bool isHex)
    {
        _isHex = isHex;
    }

    bool    KeyboardImpl::IsHexadecimal(void) const
    {
        return (_isHex);
    }

    void    KeyboardImpl::SetMaxInput(u32 max)
    {
        _max = max;
    }

    void    KeyboardImpl::CanAbort(bool canAbort)
    {
        _canAbort = canAbort;
    }

    std::string &KeyboardImpl::GetInput(void)
    {
        return (_userInput);
    }

    std::string &KeyboardImpl::GetMessage(void)
    {
        return (_text);
    }

    std::string& KeyboardImpl::GetTitle(void)
    {
        return _title;
    }

    u32&    KeyboardImpl::GetMax(void)
    {
        return _max;
    }

    void    KeyboardImpl::SetSignKeyState(bool usePlusMinus)
    {
        _usePlusMinus = usePlusMinus;
    }

    void    KeyboardImpl::SetError(std::string &error)
    {
        _errorMessage = true;
        _error = error;
    }

    void    KeyboardImpl::SetConvertCallback(ConvertCallback callback)
    {
        _convert = callback;
    }

    void    KeyboardImpl::SetCompareCallback(CompareCallback callback)
    {
        _compare = callback;
    }

    void    KeyboardImpl::OnKeyboardEvent(OnEventCallback callback)
    {
        _onKeyboardEvent = callback;
    }

    void    KeyboardImpl::ToggleDecimalKey(bool enable)
    {
        if (_layout == DECIMAL)
        {
            _keys->at(12).Enable(enable);
        }
    }

    void    KeyboardImpl::ToggleSignKey()
    {
        if (_layout == DECIMAL)
        {
            _keys->at(11).Enable(_usePlusMinus);
        }
    }

    void    KeyboardImpl::ChangeEntrySound(int entry, SoundEngine::Event soundEvent)
    {
        if (_customKeyboard && entry >= 0 && entry < static_cast<int>(_strKeys.size()) && _strKeys[entry]->CanUse())
        {
            _strKeys[entry]->SetAcceptSoundEvent(soundEvent);
        }
    }

    void    KeyboardImpl::Populate(const std::vector<std::string> &input, bool resetScroll)
    {
        bool mustReset = (_strKeys.size() != input.size()) || resetScroll;

        int count = input.size();

        if (mustReset)
            _ChangeManualKey(0, false);
        mustReset = (mustReset || count < 6);

        _customKeyboard = true;

        if (mustReset)
            _currentPosition = 0;

        std::vector<float> origPosY;

        for (TouchKeyString* tks : _strKeys)
        {
            if (!mustReset)
            {
                u16 posX; float posY;
                tks->GetPosition(posX, posY);
                origPosY.push_back(posY);
            }
            delete tks;
        }

        _strKeys.clear();


        int posY = (count < 6) ? (20 + (200 - ((30 * count) + 6 * (count - 1))) / 2) : 30;

        if (mustReset)
        {
            if (count < 6)
            {
                _displayScrollbar = false;
            }
            else
            {
                int height = 190;

                float lsize = 36.f * (float)count + 1;

                float padding = (float)height / lsize;
                int cursorSize = padding * height;
                float scrollTrackSpace = lsize - height;
                float scrollThumbSpace = height - cursorSize;

                _scrollJump = scrollTrackSpace / scrollThumbSpace;
                _scrollbarSize = height;

                if (cursorSize < 5)
                    cursorSize = 5;

                _scrollPadding = padding;
                _scrollCursorSize = cursorSize;
                _scrollPosition = 0.f;
                _scrollEnd = _scrollbarSize - _scrollCursorSize;
                _displayScrollbar = true;
            }
        }

        _scrollSize = 0;
        _inertialVelocity = 0;

        // note: selection menu are NOT combo box objects
        IntRect box(35, posY, 250, 30);
        int i = 0;
        for (const std::string &str : input)
        {
            TouchKeyString* tks = new TouchKeyString(str, box, true);

            if (!mustReset)
                tks->SetPosition(0xFFFF, origPosY[i++]);
            _strKeys.push_back(tks);
            if (mustReset) box.leftTop.y += 36;
        }

        origPosY.clear();
        _manualScrollUpdate = true;
        _UpdateScroll(0, true);
    }

    void    KeyboardImpl::Clear(void)
    {
        _customKeyboard = false;
        _currentPosition = 0;

        for (TouchKeyString *tks : _strKeys)
            delete tks;

        _strKeys.clear();
    }

    int     KeyboardImpl::Run(void)
    {
        _isOpen = true;
        _userAbort = false;
        _askForExit = false;

        // Check if Process is paused
        if (!ProcessImpl::IsPaused)
        {
            _mustRelease  = true;
            ProcessImpl::Pause(false);
        }
        else
            _mustRelease = false;

        int                 ret = -1;
        Event               event;
        EventManager        manager(EventManager::EventGroups::GROUP_KEYS | EventManager::EventGroups::GROUP_TOUCH);
        Clock               clock;

        // Construct keyboard
        if (!_customKeyboard)
        {
            if (_layout == QWERTY)
                _keys = &_QwertyKeys;
            else if (_layout == DECIMAL)
                _keys = &_DecimalKeys;
            else if (_layout == HEXADECIMAL_FULL)
                _keys = &_HexadecimalFullKeys;
            else if (_layout == HEXADECIMAL_LITE)
                _keys = &_HexadecimalLiteKeys;
        }

        // Check start input
        _errorMessage = !_CheckInput();

        // Set cursor
        if (_showCursor)
        {
            _cursorPositionInString = _userInput.size();
            _ScrollUp();
        }

        // Loop until exit
        while (_isOpen)
        {
            while (manager.PollEvent(event))
            {
                _ProcessEvent(event);
                if (_userAbort || Window::BottomWindow.MustClose())
                {
                    ret = USER_ABORT;
                    goto exit;
                }
            }

            _Update(clock.Restart().AsSeconds());

            if (DisplayTopScreen)
                _RenderTop();

            _RenderBottom();

            Renderer::EndFrame();

            if (submitBtn())
                _askForExit = true;

            // if it's a standard keyboard
            if (!_customKeyboard)
            {
                // Check keys
                bool inputChanged = _CheckKeys();

                if (_errorMessage && inputChanged)
                    _errorMessage = false;

                if (inputChanged)
                {
                    _errorMessage = !_CheckInput();
                    if (_onKeyboardEvent != nullptr && _owner != nullptr)
                        _onKeyboardEvent(*_owner, _KeyboardEvent);
                }

                // If user try to exit the keyboard
                if (_askForExit)
                {
                    // If input is invalid, user can't exit
                    if (_errorMessage)
                        _askForExit = false;
                    else
                    {
                        // Check input
                        _errorMessage = !_CheckInput();
                        if (!_errorMessage)
                        {
                            // input is valid, exit
                            _isOpen = false;
                            ret = 0;
                        }
                    }
                }
            }
            else
            {
                int  choice = -1;
                bool isSelected = _CheckButtons(choice);

                if (isSelected)
                {
                    ret = choice;
                    _isOpen = false;
                }
            }
            if (SystemImpl::IsSleeping()) {
                ret = SLEEP_ABORT;
                _isOpen = false;
            }
        }

    exit:
        PluginMenu *menu = PluginMenu::GetRunningInstance();
        if (menu && !menu->IsOpen() && ret != SLEEP_ABORT)
        {
            if (System::IsNew3DS())
            {
                ScreenImpl::Top->Clear(true);
                ScreenImpl::Bottom->Clear(true);
            }
        }
        if (_mustRelease)
            ProcessImpl::Play(false);
        return (ret);
    }

    void    KeyboardImpl::Close(void)
    {
        _isOpen = false;
    }

    void    KeyboardImpl::_RenderTop(void)
    {
        const Color     &red = Color::Red;
        static IntRect  background1(30, 20, 340, 200);
        static IntRect  background2(50, 30, 300, 180);

        IntRect &background = _mustRelease ? background2 : background1;
        int     maxX = background.leftTop.x + background.size.x;
        int     maxY = background.leftTop.y + background.size.y;

        int   posY =  background.leftTop.y + 5;
        int   posX =  background.leftTop.x + 5;

        Renderer::SetTarget(TOP);
        Window::TopWindow.Draw();

        TextBox(_title.c_str(), _text.c_str(), background).Draw();

        // IF error
        if (_errorMessage && !_error.empty())
        {
            if (posY < 120)
                posY += 48;
            Renderer::DrawGameFontStringReturn(_error.c_str(), posX, posY, maxX, red, maxY);
        }
        if (_onKeyboardEvent != nullptr && _owner != nullptr) {
            Render::Interface interface = Renderer::GetInterface();
            _ClearKeyboardEvent();
            _KeyboardEvent.type = KeyboardEvent::EventType::FrameTop;
            _KeyboardEvent.renderInterface = &interface;
            _onKeyboardEvent(*_owner, _KeyboardEvent);
        }
    }

    void    KeyboardImpl::_RenderBottom(void)
    {
        int xStart = 40;
        int yTextStart = 15;

        IntRect numericalBackground(20, 8, 280, 185);
        IntRect clampArea(22, 25, 270, 190); // note: used for selection menus

        Renderer::SetTarget(BOTTOM);
        Window::BottomWindow.Draw();

        // Draw "normal" keyboard
        if (!_customKeyboard)
        {
            static auto &theme = Preferences::Settings.Keyboard;

            if (_showCursor)
            {
                Renderer::DrawRoundedRectangle(numericalBackground, Color::Gainsboro, Color::Magenta);
                Renderer::DrawLine(40, 34, 240, Color::Gainsboro);
                Renderer::DrawGameFontString(_userInput.c_str(), xStart, yTextStart, 300, theme.Input, _offset);

                if (_blinkingClock.GetElapsedTime() < Seconds(0.5f))
                    Renderer::DrawLine(xStart + _cursorPositionOnScreen + 2, 16, 1, theme.Cursor, 15);
            }
            else
            {
                numericalBackground.leftTop.y = 20;

                Renderer::DrawRoundedRectangle(numericalBackground, Color::Gainsboro, Color::Magenta);
            }

            if (_layout != Layout::HEXADECIMAL_LITE)
            {
                submitBtn.Draw();
            }

            // Draw keys
            if (_layout != Layout::QWERTY)
            {
                for (TouchKey &key : *_keys) // decimal or hex layout
                {
                    key.Draw();
                }

                Renderer::DrawLine(182, 45, 1, Color::DeepGold, 140);
            }
            else
            {
                _UpdateActiveKeyIndexes();

                for (int i = activeKeySetStartIndex; i <= activeKeySetEndIndex; i++)
                {
                    (*_keys)[i].Draw();
                }
            }
        }
        else // Draw selection menu
        {
            size_t max = _strKeys.size();
            int offset = 6;
            max = std::min(static_cast<int>(max), _currentPosition + offset);

            PrivColor::UseClamp(true, clampArea);

            for (size_t i = _currentPosition; i < max && i < _strKeys.size(); i++)
            {
                _strKeys[i]->ForcePressed(static_cast<int>(i) == _manualKey);
                _strKeys[i]->Draw();
            }

            PrivColor::UseClamp(false);

            if (!_displayScrollbar)
                return;

            // Draw scroll bar
            const Color &sbBackground = Color::DimGrey;
            const Color &sbThumb = Color::Gainsboro;

            // Background
            int posX = 292;
            int posY = 25;

            Renderer::DrawLine(posX, posY + 1, 1, sbBackground, _scrollbarSize - 2);
            Renderer::DrawLine(posX + 1, posY, 1, sbBackground, _scrollbarSize);
            Renderer::DrawLine(posX + 2, posY + 1, 1,sbBackground, _scrollbarSize - 2);

            posY += (int)(_scrollPosition);

            // Draw thumb
            Renderer::DrawLine(posX, posY + 1, 1, sbThumb, _scrollCursorSize - 2);
            Renderer::DrawLine(posX + 1, posY, 1, sbThumb, _scrollCursorSize);
            Renderer::DrawLine(posX + 2, posY + 1, 1, sbThumb, _scrollCursorSize - 2);
        }
        if (_onKeyboardEvent != nullptr && _owner != nullptr) {
            Render::Interface interface = Renderer::GetInterface();
            _ClearKeyboardEvent();
            _KeyboardEvent.type = KeyboardEvent::EventType::FrameBottom;
            _KeyboardEvent.renderInterface = &interface;
            _onKeyboardEvent(*_owner, _KeyboardEvent);
        }
    }

    void    KeyboardImpl::_ProcessEvent(Event &event)
    {
        static Clock inputClock;
        bool inputPassedTime = inputClock.HasTimePassed(Milliseconds(200));
        bool keyPressIntended = false;

        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Key::B)
            {
                keyPressIntended = true;
                if (_canAbort)
                {
                    _userAbort = true;
                    SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);
                }

                return;
            }
            if (!_customKeyboard && event.key.code == Y && !_userInput.empty())
            {
                SoundEngine::PlayMenuSound(SoundEngine::Event::DESELECT);
                keyPressIntended  = true;
                _userInput.clear();
                _ClearKeyboardEvent();
                _KeyboardEvent.type = KeyboardEvent::EventType::InputWasCleared;
                _KeyboardEvent.codepoint = 0;
                _errorMessage = !_CheckInput();
                if (_onKeyboardEvent != nullptr && _owner != nullptr)
                    _onKeyboardEvent(*_owner, _KeyboardEvent);
            }
            if (event.key.code == X && !_customKeyboard && _layout != QWERTY && _canChangeLayout)
            {
                SoundEngine::PlayMenuSound(SoundEngine::Event::DESELECT);
                keyPressIntended = true;
                _userInput.clear();
                _ClearKeyboardEvent();
                _KeyboardEvent.type = KeyboardEvent::EventType::InputWasCleared;
                _KeyboardEvent.codepoint = 0;
                _errorMessage = !_CheckInput();
                if (_onKeyboardEvent != nullptr && _owner != nullptr)
                    _onKeyboardEvent(*_owner, _KeyboardEvent);
                SetLayout(_layout == DECIMAL ? HEXADECIMAL_FULL : DECIMAL);
                _canChangeLayout = true;
            }
            if (_customKeyboard && (event.key.code & (Key::Down | Key::Up | Key::Left | Key::Right | Key::A))) {
                keyPressIntended = true;
                _HandleManualKeyPress((Key)(event.key.code & Key::A));
                inputPassedTime = true;
            }
        }

        if (event.type == Event::KeyDown)
        {
            if (_showCursor && inputPassedTime)
            {
                if (event.key.code == Key::DPadLeft) {
                    keyPressIntended = true;
                    inputClock.Restart();
                    _ScrollDown();
                }
                else if (event.key.code == Key::DPadRight) {
                    keyPressIntended = true;
                    inputClock.Restart();
                    _ScrollUp();
                }
            }
            if (_customKeyboard && inputPassedTime) {
                if (event.key.code & (Key::Down | Key::Up | Key::Left | Key::Right | Key::A)) {
                    keyPressIntended = true;
                    _HandleManualKeyPress((Key)(event.key.code & ~(u32)Key::A));
                    inputClock.Restart();
                }
            }
        }
        if (!keyPressIntended && _onKeyboardEvent != nullptr && _owner != nullptr && (event.type == Event::KeyDown || event.type == Event::KeyPressed || event.type == Event::KeyReleased)) {
            _ClearKeyboardEvent();
            if (event.type == Event::KeyPressed) _KeyboardEvent.type = KeyboardEvent::EventType::KeyPressed;
            else if (event.type == Event::KeyDown) _KeyboardEvent.type = KeyboardEvent::EventType::KeyDown;
            else if (event.type == Event::KeyReleased) _KeyboardEvent.type = KeyboardEvent::EventType::KeyReleased;
            _KeyboardEvent.affectedKey = event.key.code;
            _onKeyboardEvent(*_owner, _KeyboardEvent);
        }

        if (event.type == Event::TouchMoved || event.type == Event::TouchEnded)
        {
            _scrollSize = 0; _ChangeManualKey(-1, false);
        }

        if (!_displayScrollbar)
            return;

        static IntRect  buttons(60, 26, 200, 200);
        // Touch / Scroll
        if (event.type == Event::TouchBegan)
        {
            if (!buttons.Contains(event.touch.x, event.touch.y))
            {
                _inertialVelocity = 0;
                _lastTouch = IntVector(event.touch.x, event.touch.y);
                _touchTimer.Restart();
            }
        }

        if (event.type == Event::TouchMoved)
        {
            if (!buttons.Contains(event.touch.x, event.touch.y))
            {
                Time delta = _touchTimer.Restart();

                float moveDistance = (float)(_lastTouch.y - event.touch.y);
                _inertialVelocity = moveDistance / delta.AsSeconds();
                _lastTouch = IntVector(event.touch.x, event.touch.y);
            }
        }

        if (event.type == Event::TouchEnded)
        {
            if (!buttons.Contains(event.touch.x, event.touch.y))
            {
                if (_touchTimer.GetElapsedTime().AsSeconds() > 0.3f)
                    _inertialVelocity = 0.f;
            }
        }
    }

    #define INERTIA_SCROLL_FACTOR 0.9f
    #define INERTIA_ACCELERATION 0.75f
    #define INERTIA_THRESHOLD 1.0f

    void    KeyboardImpl::_UpdateScroll(float delta, bool ignoreTouch) {

        bool            isTouchDown = Touch::IsDown() && !ignoreTouch;
        IntVector       touchPos(ignoreTouch ? IntVector(0, 0) : (IntVector)Touch::GetPosition());

        if (_displayScrollbar)
        {
            if (!_manualScrollUpdate)
                _scrollSize = (-_inertialVelocity * INERTIA_SCROLL_FACTOR * delta);

            _manualScrollUpdate = false;

            _scrollPosition += _scrollSize;

            if (_scrollPosition <= 0.f)
            {
                _scrollSize = _scrollSize - _scrollPosition;
                _scrollPosition = 0.f;
                _inertialVelocity = 0.f;
            }
            else if (_scrollPosition >= _scrollEnd)
            {
                _scrollSize -= (_scrollPosition - _scrollEnd);
                _scrollPosition = _scrollEnd;
                _inertialVelocity = 0.f;
            }

            _inertialVelocity += (0.98f) * delta;
            _inertialVelocity *= INERTIA_ACCELERATION;
            _currentPosition = (_scrollPosition * _scrollJump) / 36; //(_scrollPosition / 36);

            if (std::abs(_inertialVelocity) < INERTIA_THRESHOLD)
                _inertialVelocity = 0.f;

            float scr = -_scrollSize * _scrollJump;
            _scrollSize = 0;

            for (TouchKeyString* tks : _strKeys)
            {
                tks->Scroll(scr);
                tks->Update(isTouchDown, touchPos);
            }
        }
        else
        {
            for (TouchKeyString* tks : _strKeys)
                tks->Update(isTouchDown, touchPos);
        }
    }

    void KeyboardImpl::_UpdateActiveKeyIndexes(void)
    {
        // Numbers: [76 - 93]
        if (_useNumRow)
        {
            activeKeySetStartIndex = 76;
            activeKeySetEndIndex = 93;
        }
        // JPN: [209 - 248] [249 - 284] [285 - 321] [322 - 361] [362 - 386] [387 - 411] | [412 - 451] [452 - 489] [490 - 527]
        else if (_useJPN)
        {
            switch (_pageIndex)
            {
                case 0:
                    if (_useCaps)
                    {
                        activeKeySetStartIndex = 412;
                        activeKeySetEndIndex = 451;
                    }
                    else
                    {
                        activeKeySetStartIndex = 209;
                        activeKeySetEndIndex = 248;
                    }
                    break;
                case 1:
                    if (_useCaps)
                    {
                        activeKeySetStartIndex = 452;
                        activeKeySetEndIndex = 489;
                    }
                    else
                    {
                        activeKeySetStartIndex = 249;
                        activeKeySetEndIndex = 284;
                    }
                    break;
                case 2:
                    if (_useCaps)
                    {
                        activeKeySetStartIndex = 490;
                        activeKeySetEndIndex = 527;
                    }
                    else
                    {
                        activeKeySetStartIndex = 285;
                        activeKeySetEndIndex = 321;
                    }
                    break;
                case 3:
                    activeKeySetStartIndex = 322;
                    activeKeySetEndIndex = 361;
                    break;
                case 4:
                    activeKeySetStartIndex = 362;
                    activeKeySetEndIndex = 386;
                    break;
                case 5:
                    activeKeySetStartIndex = 387;
                    activeKeySetEndIndex = 411;
                    break;
                default:
                    _pageIndex = 0;
                    activeKeySetStartIndex = 209;
                    activeKeySetEndIndex = 248;
                    break;
            }
        }
        // Symbols: [94 - 133] [134 - 173] [174 - 208]
        else if (_useSymbols)
        {
            switch (_pageIndex)
            {
                case 0:
                    activeKeySetStartIndex = 94;
                    activeKeySetEndIndex = 133;
                    break;
                case 1:
                    activeKeySetStartIndex = 134;
                    activeKeySetEndIndex = 173;
                    break;
                case 2:
                    activeKeySetStartIndex = 174;
                    activeKeySetEndIndex = 208;
                    break;
                default:
                    _pageIndex = 0;
                    activeKeySetStartIndex = 94;
                    activeKeySetEndIndex = 133;
                    break;
            }
        }
        // Letters: [0 - 75]
        else
        {
            // Uppercase: [38 - 75]
            if (_useCaps)
            {
                activeKeySetStartIndex = 38;
                activeKeySetEndIndex = 75;
            }
            // Lowercase: [0 - 37]
            else
            {
                activeKeySetStartIndex = 0;
                activeKeySetEndIndex = 37;
            }
        }
    }

    void KeyboardImpl::_Update(float delta)
    {
		bool isTouchDown = Touch::IsDown();
		IntVector touchPos(Touch::GetPosition());

        submitBtn.Update(isTouchDown, touchPos);

        if (!_customKeyboard)
        {
            activeKeySetStartIndex = 0;
            activeKeySetEndIndex = _keys->size() - 1;

            if (_layout == Layout::QWERTY)
            {
                _UpdateActiveKeyIndexes();
            }

            for (int i = activeKeySetStartIndex; i <= activeKeySetEndIndex; i++)
            {
                (*_keys)[i].Update(isTouchDown, touchPos);
            }

            if (_showCursor && _blinkingClock.HasTimePassed(Seconds(1.f)))
                _blinkingClock.Restart();

            _UpdateScrollInfos();
        }
        else ///< Custom Keyboard
        {
            _UpdateScroll(delta, false);
        }

        Window::BottomWindow.Update(isTouchDown, touchPos);
    }

    void AddToKeySet(const std::string &value, IntRect &pos, std::vector<TouchKey> &keys)
    {
        keys.emplace_back(value, pos);
        pos.leftTop.x += charKeyWidth;
    }

    void BuildShortcutRow(IntRect &pos, std::vector<TouchKey> &keys, bool addPageBtns)
    {
        int shortcutKeyWidth = 68;
        int spaceWidth = addPageBtns ? 125 : 200;

        pos.leftTop.x = charKeyboardStartX;

        keys.emplace_back(KEY_CAPS, Icon::DrawCapsLockOn, pos);

        pos.leftTop.x += charKeyWidth;
        pos.size.x = spaceWidth;

        keys.emplace_back("____", pos, KEY_SPACE);

        pos.leftTop.x += spaceWidth;

        if (addPageBtns)
        {
            pos.size.x = 55;

            keys.emplace_back("Prev", pos, KEY_PREV_PAGE);
            pos.leftTop.x += pos.size.x;
            keys.emplace_back("Next", pos, KEY_NEXT_PAGE);
            pos.leftTop.x += pos.size.x;
        }

        pos.leftTop.x = 23;
        pos.leftTop.y += charKeyHeight;

        pos.size.x = shortcutKeyWidth;
        pos.size.y = charKeyHeight - 5;

        keys.emplace_back("Aa", pos, KEY_ASCII_TOGGLE);
        pos.leftTop.x += shortcutKeyWidth;
        keys.emplace_back("0-9", pos, KEY_NUM_TOGGLE);
        pos.leftTop.x += shortcutKeyWidth;
        keys.emplace_back("\uE073", pos, KEY_SYMBOLS_TOGGLE);
        pos.leftTop.x += shortcutKeyWidth;
        keys.emplace_back("JPN", pos, KEY_JPN_TOGGLE);
        pos.leftTop.x += shortcutKeyWidth;

        pos.size.x = 0;
        pos.size.y = 0;

        keys.emplace_back("", pos, KEY_JPN_TOGGLE); // Placeholder in case KEY_OTHER_TOGGLE is impl'd
    }

    void UpdateCharRowPos(IntRect &pos)
    {
        pos.leftTop.x = charKeyboardStartX;
        pos.leftTop.y += charKeyHeight;
    }

    void KeyboardImpl::_InitQwertyLowercase(void)
    {
        IntRect pos(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);

        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [0]

        AddToKeySet("q", pos, _QwertyKeys); // [1]
        AddToKeySet("w", pos, _QwertyKeys); // [2]
        AddToKeySet("e", pos, _QwertyKeys); // [3]
        AddToKeySet("r", pos, _QwertyKeys); // [4]
        AddToKeySet("t", pos, _QwertyKeys); // [5]
        AddToKeySet("y", pos, _QwertyKeys); // [6]
        AddToKeySet("u", pos, _QwertyKeys); // [7]
        AddToKeySet("i", pos, _QwertyKeys); // [8]
        AddToKeySet("o", pos, _QwertyKeys); // [9]
        AddToKeySet("p", pos, _QwertyKeys); // [10]
        UpdateCharRowPos(pos);

        AddToKeySet("a", pos, _QwertyKeys);  // [11]
        AddToKeySet("s", pos, _QwertyKeys);  // [12]
        AddToKeySet("d", pos, _QwertyKeys);  // [13]
        AddToKeySet("f", pos, _QwertyKeys);  // [14]
        AddToKeySet("g", pos, _QwertyKeys);  // [15]
        AddToKeySet("h", pos, _QwertyKeys);  // [16]
        AddToKeySet("j", pos, _QwertyKeys);  // [17]
        AddToKeySet("k", pos, _QwertyKeys);  // [18]
        AddToKeySet("l", pos, _QwertyKeys);  // [19]
        AddToKeySet("\"", pos, _QwertyKeys); // [20]
        UpdateCharRowPos(pos);

        AddToKeySet("z", pos, _QwertyKeys); // [21]
        AddToKeySet("x", pos, _QwertyKeys); // [22]
        AddToKeySet("c", pos, _QwertyKeys); // [23]
        AddToKeySet("v", pos, _QwertyKeys); // [24]
        AddToKeySet("b", pos, _QwertyKeys); // [25]
        AddToKeySet("n", pos, _QwertyKeys); // [26]
        AddToKeySet("m", pos, _QwertyKeys); // [27]
        AddToKeySet(",", pos, _QwertyKeys); // [28]
        AddToKeySet(".", pos, _QwertyKeys); // [29]
        AddToKeySet("?", pos, _QwertyKeys); // [30]
        UpdateCharRowPos(pos);

        BuildShortcutRow(pos, _QwertyKeys, false); // [31 - 37]
    }

    void KeyboardImpl::_InitQwertyUppercase(void)
    {
        IntRect pos(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);

        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [38]

        AddToKeySet("Q", pos, _QwertyKeys); // [39]
        AddToKeySet("W", pos, _QwertyKeys); // [40]
        AddToKeySet("E", pos, _QwertyKeys); // [41]
        AddToKeySet("R", pos, _QwertyKeys); // [42]
        AddToKeySet("T", pos, _QwertyKeys); // [43]
        AddToKeySet("Y", pos, _QwertyKeys); // [44]
        AddToKeySet("U", pos, _QwertyKeys); // [45]
        AddToKeySet("I", pos, _QwertyKeys); // [46]
        AddToKeySet("O", pos, _QwertyKeys); // [47]
        AddToKeySet("P", pos, _QwertyKeys); // [48]

        UpdateCharRowPos(pos);

        AddToKeySet("A", pos, _QwertyKeys);  // [49]
        AddToKeySet("S", pos, _QwertyKeys);  // [50]
        AddToKeySet("D", pos, _QwertyKeys);  // [51]
        AddToKeySet("F", pos, _QwertyKeys);  // [52]
        AddToKeySet("G", pos, _QwertyKeys);  // [53]
        AddToKeySet("H", pos, _QwertyKeys);  // [54]
        AddToKeySet("J", pos, _QwertyKeys);  // [55]
        AddToKeySet("K", pos, _QwertyKeys);  // [56]
        AddToKeySet("L", pos, _QwertyKeys);  // [57]
        AddToKeySet("\"", pos, _QwertyKeys); // [58]
        UpdateCharRowPos(pos);

        AddToKeySet("Z", pos, _QwertyKeys); // [59]
        AddToKeySet("X", pos, _QwertyKeys); // [60]
        AddToKeySet("C", pos, _QwertyKeys); // [61]
        AddToKeySet("V", pos, _QwertyKeys); // [62]
        AddToKeySet("B", pos, _QwertyKeys); // [63]
        AddToKeySet("N", pos, _QwertyKeys); // [64]
        AddToKeySet("M", pos, _QwertyKeys); // [65]
        AddToKeySet(";", pos, _QwertyKeys); // [66]
        AddToKeySet(":", pos, _QwertyKeys); // [67]
        AddToKeySet("!", pos, _QwertyKeys); // [68]
        UpdateCharRowPos(pos);

        BuildShortcutRow(pos, _QwertyKeys, false); // [69 - 75]
    }

    void KeyboardImpl::_InitQwertyNumRow(void)
    {
        IntRect pos(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);

        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [76]

        AddToKeySet("0", pos, _QwertyKeys); // [77]
        AddToKeySet("1", pos, _QwertyKeys); // [78]
        AddToKeySet("2", pos, _QwertyKeys); // [79]
        AddToKeySet("3", pos, _QwertyKeys); // [80]
        AddToKeySet("4", pos, _QwertyKeys); // [81]
        AddToKeySet("5", pos, _QwertyKeys); // [82]
        AddToKeySet("6", pos, _QwertyKeys); // [83]
        AddToKeySet("7", pos, _QwertyKeys); // [84]
        AddToKeySet("8", pos, _QwertyKeys); // [85]
        AddToKeySet("9", pos, _QwertyKeys); // [86]

        pos.leftTop.x = charKeyboardStartX;
        pos.leftTop.y += charKeyHeight * 3;

        BuildShortcutRow(pos, _QwertyKeys, false); // [87 - 93]
    }

    void KeyboardImpl::_InitQwertySymbols()
    {
        IntRect pos(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);

        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [94]

        // page 1: 94 - 133
        AddToKeySet("", pos, _QwertyKeys); // [95] Nintendo A
        AddToKeySet("", pos, _QwertyKeys); // [96] Nintendo B
        AddToKeySet("", pos, _QwertyKeys); // [97] Nintendo X
        AddToKeySet("", pos, _QwertyKeys); // [98] Nintendo Y
        AddToKeySet("", pos, _QwertyKeys); // [99] Nintendo L
        AddToKeySet("", pos, _QwertyKeys); // [100] Nintendo R
        AddToKeySet("", pos, _QwertyKeys); // [101] Diamond
        AddToKeySet("", pos, _QwertyKeys); // [102] Heart
        AddToKeySet("", pos, _QwertyKeys); // [103] Club
        AddToKeySet("", pos, _QwertyKeys); // [104] Down Arrow
        UpdateCharRowPos(pos);

        AddToKeySet("", pos, _QwertyKeys); // [105] Upper Left Corner
        AddToKeySet("", pos, _QwertyKeys); // [106] Upper Platform
        AddToKeySet("", pos, _QwertyKeys); // [107] Upper Right Corner
        AddToKeySet("", pos, _QwertyKeys); // [108] Left Platform
        AddToKeySet("", pos, _QwertyKeys); // [109] Lower Right Corner
        AddToKeySet("", pos, _QwertyKeys); // [110] Bottom Platform
        AddToKeySet("", pos, _QwertyKeys); // [111] Lower Left Corner
        AddToKeySet("", pos, _QwertyKeys); // [112] Center Pillar
        AddToKeySet("", pos, _QwertyKeys); // [113] Nintendo C
        AddToKeySet("", pos, _QwertyKeys); // [114] Nintendo Home
        UpdateCharRowPos(pos);

        AddToKeySet("■", pos, _QwertyKeys); // [115]
        AddToKeySet("□", pos, _QwertyKeys); // [116]
        AddToKeySet("▲", pos, _QwertyKeys); // [117]
        AddToKeySet("△", pos, _QwertyKeys); // [118]
        AddToKeySet("▼", pos, _QwertyKeys); // [119]
        AddToKeySet("▽", pos, _QwertyKeys); // [120]
        AddToKeySet("◆", pos, _QwertyKeys); // [121]
        AddToKeySet("◇", pos, _QwertyKeys); // [122]
        AddToKeySet("○", pos, _QwertyKeys); // [123]
        AddToKeySet("◎", pos, _QwertyKeys); // [124]
        UpdateCharRowPos(pos);

        BuildShortcutRow(pos, _QwertyKeys, true); // [125 - 133]

        pos = IntRect(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [134]

        // page 2: 134 - 173
        AddToKeySet("←", pos, _QwertyKeys); // [135]
        AddToKeySet("↑", pos, _QwertyKeys); // [136]
        AddToKeySet("→", pos, _QwertyKeys); // [137]
        AddToKeySet("↓", pos, _QwertyKeys); // [138]
        AddToKeySet("⇔", pos, _QwertyKeys); // [139]
        AddToKeySet("●", pos, _QwertyKeys); // [140]
        AddToKeySet("★", pos, _QwertyKeys); // [141]
        AddToKeySet("☆", pos, _QwertyKeys); // [142]
        AddToKeySet("♀", pos, _QwertyKeys); // [143]
        AddToKeySet("♂", pos, _QwertyKeys); // [144]
        UpdateCharRowPos(pos);

        AddToKeySet("∴", pos, _QwertyKeys); // [145]
        AddToKeySet("∵", pos, _QwertyKeys); // [146]
        AddToKeySet("⊂", pos, _QwertyKeys); // [147]
        AddToKeySet("⊃", pos, _QwertyKeys); // [148]
        AddToKeySet("⌒", pos, _QwertyKeys); // [149]
        AddToKeySet("♪", pos, _QwertyKeys); // [150]
        AddToKeySet("♭", pos, _QwertyKeys); // [151]
        AddToKeySet("¼", pos, _QwertyKeys); // [152]
        AddToKeySet("½", pos, _QwertyKeys); // [153]
        AddToKeySet("¾", pos, _QwertyKeys); // [154]
        UpdateCharRowPos(pos);

        AddToKeySet("«", pos, _QwertyKeys); // [155]
        AddToKeySet("»", pos, _QwertyKeys); // [156]
        AddToKeySet("¦", pos, _QwertyKeys); // [157]
        AddToKeySet("∞", pos, _QwertyKeys); // [158]
        AddToKeySet("※", pos, _QwertyKeys); // [159]
        AddToKeySet("№", pos, _QwertyKeys); // [160]
        AddToKeySet("(", pos, _QwertyKeys); // [161]
        AddToKeySet(")", pos, _QwertyKeys); // [162]
        AddToKeySet("[", pos, _QwertyKeys); // [163]
        AddToKeySet("]", pos, _QwertyKeys); // [164]
        UpdateCharRowPos(pos);

        BuildShortcutRow(pos, _QwertyKeys, true); // [165 - 173]

        pos = IntRect(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [174]

        // page 3: 174 - 208
        AddToKeySet("™", pos, _QwertyKeys); // [175]
        AddToKeySet("#", pos, _QwertyKeys); // [176]
        AddToKeySet("|", pos, _QwertyKeys); // [177]
        AddToKeySet("_", pos, _QwertyKeys); // [178]
        AddToKeySet("{", pos, _QwertyKeys); // [179]
        AddToKeySet("}", pos, _QwertyKeys); // [180]
        AddToKeySet("&", pos, _QwertyKeys); // [181]
        AddToKeySet("~", pos, _QwertyKeys); // [182]
        AddToKeySet("†", pos, _QwertyKeys); // [183]
        AddToKeySet("‡", pos, _QwertyKeys); // [184]
        UpdateCharRowPos(pos);

        AddToKeySet("+", pos, _QwertyKeys); // [185]
        AddToKeySet("-", pos, _QwertyKeys); // [186]
        AddToKeySet("*", pos, _QwertyKeys); // [187]
        AddToKeySet("/", pos, _QwertyKeys); // [188]
        AddToKeySet("<", pos, _QwertyKeys); // [189]
        AddToKeySet(">", pos, _QwertyKeys); // [190]
        AddToKeySet("=", pos, _QwertyKeys); // [191]
        AddToKeySet("^", pos, _QwertyKeys); // [192]
        AddToKeySet("±", pos, _QwertyKeys); // [193]
        AddToKeySet("%", pos, _QwertyKeys); // [194]
        UpdateCharRowPos(pos);

        AddToKeySet("£", pos, _QwertyKeys); // [195]
        AddToKeySet("¤", pos, _QwertyKeys); // [196]
        AddToKeySet("¥", pos, _QwertyKeys); // [197]
        AddToKeySet("′", pos, _QwertyKeys); // [198]
        AddToKeySet("€", pos, _QwertyKeys); // [199]
        UpdateCharRowPos(pos);

        BuildShortcutRow(pos, _QwertyKeys, true); // [200 - 208]
    }

    void KeyboardImpl::_InitQwertyJPN(void)
    {
        IntRect pos(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);

        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [209]

        // page 1: 209 - 248
        AddToKeySet("あ", pos, _QwertyKeys); // [210]
        AddToKeySet("い", pos, _QwertyKeys); // [211]
        AddToKeySet("う", pos, _QwertyKeys); // [212]
        AddToKeySet("え", pos, _QwertyKeys); // [213]
        AddToKeySet("お", pos, _QwertyKeys); // [214]
        AddToKeySet("か", pos, _QwertyKeys); // [215]
        AddToKeySet("き", pos, _QwertyKeys); // [216]
        AddToKeySet("く", pos, _QwertyKeys); // [217]
        AddToKeySet("け", pos, _QwertyKeys); // [218]
        AddToKeySet("こ", pos, _QwertyKeys); // [219]
        UpdateCharRowPos(pos);

        AddToKeySet("さ", pos, _QwertyKeys); // [220]
        AddToKeySet("し", pos, _QwertyKeys); // [221]
        AddToKeySet("す", pos, _QwertyKeys); // [222]
        AddToKeySet("せ", pos, _QwertyKeys); // [223]
        AddToKeySet("そ", pos, _QwertyKeys); // [224]
        AddToKeySet("た", pos, _QwertyKeys); // [225]
        AddToKeySet("ち", pos, _QwertyKeys); // [226]
        AddToKeySet("つ", pos, _QwertyKeys); // [227]
        AddToKeySet("て", pos, _QwertyKeys); // [228]
        AddToKeySet("と", pos, _QwertyKeys); // [229]
        UpdateCharRowPos(pos);

        AddToKeySet("な", pos, _QwertyKeys); // [230]
        AddToKeySet("に", pos, _QwertyKeys); // [231]
        AddToKeySet("ぬ", pos, _QwertyKeys); // [232]
        AddToKeySet("ね", pos, _QwertyKeys); // [233]
        AddToKeySet("の", pos, _QwertyKeys); // [234]
        AddToKeySet("ま", pos, _QwertyKeys); // [235]
        AddToKeySet("み", pos, _QwertyKeys); // [236]
        AddToKeySet("む", pos, _QwertyKeys); // [237]
        AddToKeySet("め", pos, _QwertyKeys); // [238]
        AddToKeySet("も", pos, _QwertyKeys); // [239]
        UpdateCharRowPos(pos);

        BuildShortcutRow(pos, _QwertyKeys, true); // [240 - 248]

        pos = IntRect(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [249]

        // page 2: 249 - 284
        AddToKeySet("ぁ", pos, _QwertyKeys); // [250]
        AddToKeySet("ぃ", pos, _QwertyKeys); // [251]
        AddToKeySet("ぅ", pos, _QwertyKeys); // [252]
        AddToKeySet("ぇ", pos, _QwertyKeys); // [253]
        AddToKeySet("ぉ", pos, _QwertyKeys); // [254]
        AddToKeySet("が", pos, _QwertyKeys); // [255]
        AddToKeySet("ぎ", pos, _QwertyKeys); // [256]
        AddToKeySet("ぐ", pos, _QwertyKeys); // [257]
        AddToKeySet("げ", pos, _QwertyKeys); // [258]
        AddToKeySet("ご", pos, _QwertyKeys); // [259]
        UpdateCharRowPos(pos);

        AddToKeySet("ざ", pos, _QwertyKeys); // [260]
        AddToKeySet("じ", pos, _QwertyKeys); // [261]
        AddToKeySet("ず", pos, _QwertyKeys); // [262]
        AddToKeySet("ぜ", pos, _QwertyKeys); // [263]
        AddToKeySet("ぞ", pos, _QwertyKeys); // [264]
        AddToKeySet("だ", pos, _QwertyKeys); // [265]
        AddToKeySet("ぢ", pos, _QwertyKeys); // [266]
        AddToKeySet("づ", pos, _QwertyKeys); // [267]
        AddToKeySet("で", pos, _QwertyKeys); // [268]
        AddToKeySet("ど", pos, _QwertyKeys); // [269]
        UpdateCharRowPos(pos);

        AddToKeySet("ら", pos, _QwertyKeys); // [270]
        AddToKeySet("り", pos, _QwertyKeys); // [271]
        AddToKeySet("る", pos, _QwertyKeys); // [272]
        AddToKeySet("れ", pos, _QwertyKeys); // [273]
        AddToKeySet("ろ", pos, _QwertyKeys); // [274]
        pos.leftTop.x += charKeyWidth * 2;
        AddToKeySet("っ", pos, _QwertyKeys); // [275]
        UpdateCharRowPos(pos);

        BuildShortcutRow(pos, _QwertyKeys, true); // [276 - 284]

        pos = IntRect(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [285]

        // page 3: 285 - 321
        AddToKeySet("や", pos, _QwertyKeys); // [286]
        AddToKeySet("ゆ", pos, _QwertyKeys); // [287]
        AddToKeySet("よ", pos, _QwertyKeys); // [288]
        AddToKeySet("わ", pos, _QwertyKeys); // [289]
        AddToKeySet("を", pos, _QwertyKeys); // [290]
        AddToKeySet("は", pos, _QwertyKeys); // [291]
        AddToKeySet("ひ", pos, _QwertyKeys); // [292]
        AddToKeySet("ふ", pos, _QwertyKeys); // [293]
        AddToKeySet("へ", pos, _QwertyKeys); // [294]
        AddToKeySet("ほ", pos, _QwertyKeys); // [295]
        UpdateCharRowPos(pos);

        AddToKeySet("ゃ", pos, _QwertyKeys); // [296]
        AddToKeySet("ゅ", pos, _QwertyKeys); // [297]
        AddToKeySet("ょ", pos, _QwertyKeys); // [298]
        AddToKeySet("ゎ", pos, _QwertyKeys); // [299]
        AddToKeySet("ん", pos, _QwertyKeys); // [300]
        AddToKeySet("ば", pos, _QwertyKeys); // [301]
        AddToKeySet("び", pos, _QwertyKeys); // [302]
        AddToKeySet("ぶ", pos, _QwertyKeys); // [303]
        AddToKeySet("べ", pos, _QwertyKeys); // [304]
        AddToKeySet("ぼ", pos, _QwertyKeys); // [305]
        UpdateCharRowPos(pos);

        AddToKeySet("ゝ", pos, _QwertyKeys); // [306]
        AddToKeySet("ゞ", pos, _QwertyKeys); // [307]
        pos.leftTop.x += charKeyWidth * 3;
        AddToKeySet("ぱ", pos, _QwertyKeys); // [308]
        AddToKeySet("ぴ", pos, _QwertyKeys); // [309]
        AddToKeySet("ぷ", pos, _QwertyKeys); // [310]
        AddToKeySet("ぺ", pos, _QwertyKeys); // [311]
        AddToKeySet("ぽ", pos, _QwertyKeys); // [312]
        UpdateCharRowPos(pos);

        BuildShortcutRow(pos, _QwertyKeys, true); // [313 - 321]

        pos = IntRect(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [322]

        // page 4: 322 - 361
        AddToKeySet("乗", pos, _QwertyKeys); // [323]
        AddToKeySet("事", pos, _QwertyKeys); // [324]
        AddToKeySet("仝", pos, _QwertyKeys); // [325]
        AddToKeySet("何", pos, _QwertyKeys); // [326]
        AddToKeySet("魔", pos, _QwertyKeys); // [327]
        AddToKeySet("倒", pos, _QwertyKeys); // [328]
        AddToKeySet("利", pos, _QwertyKeys); // [329]
        AddToKeySet("前", pos, _QwertyKeys); // [330]
        AddToKeySet("動", pos, _QwertyKeys); // [331]
        AddToKeySet("北", pos, _QwertyKeys); // [332]
        UpdateCharRowPos(pos);

        AddToKeySet("南", pos, _QwertyKeys); // [333]
        AddToKeySet("名", pos, _QwertyKeys); // [334]
        AddToKeySet("場", pos, _QwertyKeys); // [335]
        AddToKeySet("塞", pos, _QwertyKeys); // [336]
        AddToKeySet("変", pos, _QwertyKeys); // [337]
        AddToKeySet("天", pos, _QwertyKeys); // [338]
        AddToKeySet("女", pos, _QwertyKeys); // [339]
        AddToKeySet("字", pos, _QwertyKeys); // [340]
        AddToKeySet("川", pos, _QwertyKeys); // [341]
        AddToKeySet("戦", pos, _QwertyKeys); // [342]
        UpdateCharRowPos(pos);

        AddToKeySet("文", pos, _QwertyKeys); // [343]
        AddToKeySet("更", pos, _QwertyKeys); // [344]
        AddToKeySet("服", pos, _QwertyKeys); // [345]
        AddToKeySet("本", pos, _QwertyKeys); // [346]
        AddToKeySet("東", pos, _QwertyKeys); // [347]
        AddToKeySet("止", pos, _QwertyKeys); // [348]
        AddToKeySet("毛", pos, _QwertyKeys); // [349]
        AddToKeySet("水", pos, _QwertyKeys); // [350]
        AddToKeySet("漢", pos, _QwertyKeys); // [351]
        AddToKeySet("火", pos, _QwertyKeys); // [352]
        UpdateCharRowPos(pos);

        BuildShortcutRow(pos, _QwertyKeys, true); // [353 - 361]

        pos = IntRect(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [362]

        // page 5: 362 - 386
        AddToKeySet("牧", pos, _QwertyKeys); // [363]
        AddToKeySet("王", pos, _QwertyKeys); // [364]
        AddToKeySet("砂", pos, _QwertyKeys); // [365]
        AddToKeySet("示", pos, _QwertyKeys); // [366]
        AddToKeySet("移", pos, _QwertyKeys); // [367]
        AddToKeySet("空", pos, _QwertyKeys); // [368]
        AddToKeySet("緑", pos, _QwertyKeys); // [369]
        AddToKeySet("表", pos, _QwertyKeys); // [370]
        AddToKeySet("西", pos, _QwertyKeys); // [371]
        AddToKeySet("要", pos, _QwertyKeys); // [372]
        UpdateCharRowPos(pos);

        AddToKeySet("走", pos, _QwertyKeys); // [373]
        AddToKeySet("返", pos, _QwertyKeys); // [374]
        AddToKeySet("間", pos, _QwertyKeys); // [375]
        AddToKeySet("闇", pos, _QwertyKeys); // [376]
        AddToKeySet("雪", pos, _QwertyKeys); // [377]

        pos.leftTop.x = charKeyboardStartX;
        pos.leftTop.y += charKeyHeight * 2;

        BuildShortcutRow(pos, _QwertyKeys, true); // [378 - 386]

        pos = IntRect(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [387]

        // page 6: 387 - 411
        AddToKeySet("、", pos, _QwertyKeys); // [388]
        AddToKeySet("〃", pos, _QwertyKeys); // [389]
        AddToKeySet("々", pos, _QwertyKeys); // [390]
        AddToKeySet("〆", pos, _QwertyKeys); // [391]
        AddToKeySet("「", pos, _QwertyKeys); // [392]
        AddToKeySet("」", pos, _QwertyKeys); // [393]
        AddToKeySet("『", pos, _QwertyKeys); // [394]
        AddToKeySet("』", pos, _QwertyKeys); // [395]
        AddToKeySet("【", pos, _QwertyKeys); // [396]
        AddToKeySet("】", pos, _QwertyKeys); // [397]
        UpdateCharRowPos(pos);

        AddToKeySet("〒", pos, _QwertyKeys); // [398]
        AddToKeySet("！", pos, _QwertyKeys); // [399]
        AddToKeySet("？", pos, _QwertyKeys); // [400]
        AddToKeySet("～", pos, _QwertyKeys); // [401]
        AddToKeySet("・", pos, _QwertyKeys); // [402]
        UpdateCharRowPos(pos);

        pos.leftTop.x = charKeyboardStartX;
        pos.leftTop.y += charKeyHeight;

        BuildShortcutRow(pos, _QwertyKeys, true); // [403 - 411]
    }

    void KeyboardImpl::_InitQwertyJPNAlt(void)
    {
        IntRect pos(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);

        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [412]

        // page 1: 412 - 451
        AddToKeySet("ア", pos, _QwertyKeys); // [413]
        AddToKeySet("イ", pos, _QwertyKeys); // [414]
        AddToKeySet("ウ", pos, _QwertyKeys); // [415]
        AddToKeySet("エ", pos, _QwertyKeys); // [416]
        AddToKeySet("オ", pos, _QwertyKeys); // [417]
        AddToKeySet("カ", pos, _QwertyKeys); // [418]
        AddToKeySet("キ", pos, _QwertyKeys); // [419]
        AddToKeySet("ク", pos, _QwertyKeys); // [420]
        AddToKeySet("ケ", pos, _QwertyKeys); // [421]
        AddToKeySet("コ", pos, _QwertyKeys); // [422]
        UpdateCharRowPos(pos);

        AddToKeySet("サ", pos, _QwertyKeys); // [423]
        AddToKeySet("シ", pos, _QwertyKeys); // [424]
        AddToKeySet("ス", pos, _QwertyKeys); // [425]
        AddToKeySet("セ", pos, _QwertyKeys); // [426]
        AddToKeySet("ソ", pos, _QwertyKeys); // [427]
        AddToKeySet("タ", pos, _QwertyKeys); // [428]
        AddToKeySet("チ", pos, _QwertyKeys); // [429]
        AddToKeySet("ツ", pos, _QwertyKeys); // [430]
        AddToKeySet("テ", pos, _QwertyKeys); // [431]
        AddToKeySet("ト", pos, _QwertyKeys); // [432]
        UpdateCharRowPos(pos);

        AddToKeySet("ナ", pos, _QwertyKeys); // [433]
        AddToKeySet("ニ", pos, _QwertyKeys); // [434]
        AddToKeySet("ヌ", pos, _QwertyKeys); // [435]
        AddToKeySet("ネ", pos, _QwertyKeys); // [436]
        AddToKeySet("ノ", pos, _QwertyKeys); // [437]
        AddToKeySet("マ", pos, _QwertyKeys); // [438]
        AddToKeySet("ミ", pos, _QwertyKeys); // [439]
        AddToKeySet("ム", pos, _QwertyKeys); // [440]
        AddToKeySet("メ", pos, _QwertyKeys); // [441]
        AddToKeySet("モ", pos, _QwertyKeys); // [442]
        UpdateCharRowPos(pos);

        BuildShortcutRow(pos, _QwertyKeys, true); // [443 - 451]

        pos = IntRect(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [452]

        // page 2: 452 - 489
        AddToKeySet("ァ", pos, _QwertyKeys); // [453]
        AddToKeySet("ィ", pos, _QwertyKeys); // [454]
        AddToKeySet("ゥ", pos, _QwertyKeys); // [455]
        AddToKeySet("ェ", pos, _QwertyKeys); // [456]
        AddToKeySet("ォ", pos, _QwertyKeys); // [457]
        AddToKeySet("ガ", pos, _QwertyKeys); // [458]
        AddToKeySet("ギ", pos, _QwertyKeys); // [459]
        AddToKeySet("グ", pos, _QwertyKeys); // [460]
        AddToKeySet("ゲ", pos, _QwertyKeys); // [461]
        AddToKeySet("ゴ", pos, _QwertyKeys); // [462]
        UpdateCharRowPos(pos);

        AddToKeySet("ザ", pos, _QwertyKeys); // [463]
        AddToKeySet("ジ", pos, _QwertyKeys); // [464]
        AddToKeySet("ズ", pos, _QwertyKeys); // [465]
        AddToKeySet("ゼ", pos, _QwertyKeys); // [466]
        AddToKeySet("ゾ", pos, _QwertyKeys); // [467]
        AddToKeySet("ダ", pos, _QwertyKeys); // [468]
        AddToKeySet("ヂ", pos, _QwertyKeys); // [469]
        AddToKeySet("ヅ", pos, _QwertyKeys); // [470]
        AddToKeySet("デ", pos, _QwertyKeys); // [471]
        AddToKeySet("ド", pos, _QwertyKeys); // [472]
        UpdateCharRowPos(pos);

        AddToKeySet("ラ", pos, _QwertyKeys); // [473]
        AddToKeySet("リ", pos, _QwertyKeys); // [474]
        AddToKeySet("ル", pos, _QwertyKeys); // [475]
        AddToKeySet("レ", pos, _QwertyKeys); // [476]
        AddToKeySet("ロ", pos, _QwertyKeys); // [477]
        pos.leftTop.x += charKeyWidth;
        AddToKeySet("ッ", pos, _QwertyKeys); // [478]
        pos.leftTop.x += charKeyWidth;
        AddToKeySet("・", pos, _QwertyKeys); // [479]
        AddToKeySet("ー", pos, _QwertyKeys); // [480]
        UpdateCharRowPos(pos);

        BuildShortcutRow(pos, _QwertyKeys, true); // [481 - 489]

        pos = IntRect(charKeyboardStartX, 36, charKeyWidth, charKeyHeight);
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos); // [490]

        // page 3: 490 - 527
        AddToKeySet("ヤ", pos, _QwertyKeys); // [491]
        AddToKeySet("ユ", pos, _QwertyKeys); // [492]
        AddToKeySet("ヨ", pos, _QwertyKeys); // [493]
        AddToKeySet("ワ", pos, _QwertyKeys); // [494]
        AddToKeySet("ヲ", pos, _QwertyKeys); // [495]
        AddToKeySet("ハ", pos, _QwertyKeys); // [496]
        AddToKeySet("ヒ", pos, _QwertyKeys); // [497]
        AddToKeySet("フ", pos, _QwertyKeys); // [498]
        AddToKeySet("ヘ", pos, _QwertyKeys); // [499]
        AddToKeySet("ホ", pos, _QwertyKeys); // [500]
        UpdateCharRowPos(pos);

        AddToKeySet("ャ", pos, _QwertyKeys); // [501]
        AddToKeySet("ュ", pos, _QwertyKeys); // [502]
        AddToKeySet("ョ", pos, _QwertyKeys); // [503]
        AddToKeySet("ヮ", pos, _QwertyKeys); // [504]
        AddToKeySet("ン", pos, _QwertyKeys); // [505]
        AddToKeySet("バ", pos, _QwertyKeys); // [506]
        AddToKeySet("ビ", pos, _QwertyKeys); // [507]
        AddToKeySet("ブ", pos, _QwertyKeys); // [508]
        AddToKeySet("ベ", pos, _QwertyKeys); // [509]
        AddToKeySet("ボ", pos, _QwertyKeys); // [510]
        UpdateCharRowPos(pos);

        pos.leftTop.x += charKeyWidth;
        AddToKeySet("ヵ", pos, _QwertyKeys); // [511]
        AddToKeySet("ヶ", pos, _QwertyKeys); // [512]
        AddToKeySet("ヴ", pos, _QwertyKeys); // [513]
        pos.leftTop.x += charKeyWidth;
        AddToKeySet("パ", pos, _QwertyKeys); // [514]
        AddToKeySet("ピ", pos, _QwertyKeys); // [515]
        AddToKeySet("プ", pos, _QwertyKeys); // [516]
        AddToKeySet("ペ", pos, _QwertyKeys); // [517]
        AddToKeySet("ポ", pos, _QwertyKeys); // [518]
        UpdateCharRowPos(pos);

        BuildShortcutRow(pos, _QwertyKeys, true); // [519 - 527]
    }

    void KeyboardImpl::_InitQwertyOther(void)
    {
        // TODO: If needed in the future: Greek, Coptic, Latin Extensions, and Cyrillic
    }

    void KeyboardImpl::_InitQwertySequence(void)
    {
        if (!_QwertyKeys.empty())
            return;

        _QwertyKeys.reserve(530);

        _InitQwertyLowercase(); // [0 - 37]
        _InitQwertyUppercase(); // [38 - 75]
        _InitQwertyNumRow(); // [76 - 93]
        _InitQwertySymbols(); // [94 - 133] [134 - 173] [174 - 208]
    }

    void KeyboardImpl::_InitDigitKeys(IntRect &keyPosition, std::vector<TouchKey> &keys, int xStartCoord)
    {
        // 0 -> 9
        char digit = '1';
        for (int i = 0; i < 10; i++, digit++)
        {
            if (i == 9)
            {
                digit = '0';
                keyPosition.size.x = digitKeyWidthLen * 3;
            }

            keys.emplace_back(digit, keyPosition);

            if (i % 3 == 2) // start new row
            {
                keyPosition.leftTop.x = xStartCoord;
                keyPosition.leftTop.y += digitKeyWidthLen;
            }
            else if (i == 9) // ensure keyPosition is using standard key sizing for future keys after function return
            {
                keyPosition.leftTop.x += (digitKeyWidthLen * 3) + digitKeyWidthLen;
                keyPosition.size.x = digitKeyWidthLen;
            }
            else
            {
                keyPosition.leftTop.x += digitKeyWidthLen;
            }
        }
    }

    void KeyboardImpl::_InitHexKeys(IntRect &keyPosition, std::vector<TouchKey> &keys, int yStart)
    {
        keyPosition.leftTop.y = yStart;

        // A -> F
        char c = 'A';
        for (int i = 0; i < 6; i++, c++)
        {
            keys.emplace_back(c, keyPosition);

            if (i % 2 == 1)
            {
                keyPosition.leftTop.x -= digitKeyWidthLen;
                keyPosition.leftTop.y += digitKeyWidthLen;
            }
            else
            {
                keyPosition.leftTop.x += digitKeyWidthLen;
            }
        }
    }

    void KeyboardImpl::_InitDecimalKeyboard(void)
    {
        int xStart = 45,
            yStart = 40,
            ySpecStart = 58,
            xStartOther = 225;

        IntRect keyPosition(xStart, yStart, digitKeyWidthLen, digitKeyWidthLen);
        IntRect backspaceKeyPos(252, 17, 25, 16);

        if (!_DecimalKeys.empty()) // no need to repopulate this if it was done previously
            return;

        _InitDigitKeys(keyPosition, _DecimalKeys, xStart);

        // Special keys: backspace, plus-minus, decimal
        _DecimalKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos);

        _DecimalKeys.emplace_back("\u00B1", IntRect(xStartOther, ySpecStart, digitKeyWidthLen, digitKeyWidthLen));      // 00B1 = ± //
        _DecimalKeys.emplace_back("\u002E", IntRect(xStartOther, ySpecStart + 57, digitKeyWidthLen, digitKeyWidthLen)); // 002E = . //
    }

    void KeyboardImpl::_CreateHexLayout(int yStart, std::vector<TouchKey> &keys)
    {
        int xStart = 45,
            yHexStart = 50;

        IntRect keyPosition(xStart, yStart, digitKeyWidthLen, digitKeyWidthLen);

        if (!keys.empty())
            return;

        _InitDigitKeys(keyPosition, keys, xStart);
        _InitHexKeys(keyPosition, keys, yHexStart);
    }

    void KeyboardImpl::_InitHexFullKeyboard(void)
    {
        _CreateHexLayout(40, _HexadecimalFullKeys);

        IntRect backspaceKeyPos(252, 17, 25, 16);
        _HexadecimalFullKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, backspaceKeyPos);
    }

    void KeyboardImpl::_InitHexLiteKeyboard(void)
    {
        _CreateHexLayout(35, _HexadecimalLiteKeys);
    }

    static int UnitsToAdvance(const char *str, u32 target)
    {
        const u8 *s = reinterpret_cast<const u8 *>(str);
        int units = 0;

        // Skip UTF8 sig
        if (s[0] == 0xEF && s[1] == 0xBB && s[2] == 0xBF)
            s += 3;

        while (*s && target > 0)
        {
            if (*s == 0x18)
            {
                ++s;
                ++units;
                --target;
                continue;
            }

            if (*s == 0x1B)
            {
                s += 4;
                units += 4;
                target -= 4;
                continue;
            }

            u32 code;
            int unit = decode_utf8(&code, s);

            if (code == 0)
                break;
            if (unit == -1)
                return -1;

            s += unit;
            units += unit;
            target -= unit;
        }
        return units;
    }

    int     UnitsToNextChar(const char *cstr, int left)
    {
        int units = 0;

        while (*cstr && left > 0)
        {
            if (*cstr == 0x1B)
            {
                units += 4;
                left -= 4;
                cstr += 4;
                continue;
            }

            if (*cstr == 0x18)
            {
                ++units;
                --left;
                ++cstr;
                continue;
            }

            u32 code;
            int u = decode_utf8(&code, (u8 *)cstr);

           /* if (code == 0)
                break;*/
            if (u == -1)
                return u;

            units += u;
            break;
        }
        return units;
    }

    int     UnitsToPreviousChar(const char *cstr, int cursor)
    {
        int units = 0;

        --cstr;
        while (*cstr && cursor > 0)
        {
            --cursor;
            if (cursor > 4 && *(cstr - 3) == 0x1B)
            {
                units += 4;
                cursor -= 4;
                cstr -= 3;
                continue;
            }

            if (*cstr == 0x18)
            {
                ++units;
                --cursor;
                --cstr;
                continue;
            }

            u32 code;
            int u = decode_utf8(&code, (u8 *)cstr);

            if (u == -1 && !cursor)
                return u;

            if (u != -1)
            {
                units += u;
                break;
            }
            --cstr;
        }
        return units;
    }

    void    KeyboardImpl::_ScrollUp(void)
    {
        const u32   strLength = _userInput.size();
        const char  *cstr = _userInput.c_str();

        // If input is empty
        if (strLength == 0)
            goto empty;

        {
            // Get units to advance to go to the next char
            const int units = UnitsToNextChar(cstr + _cursorPositionInString, strLength - _cursorPositionInString);

            if (units < 0)
            {
                ///< Weird char being found, better clear the input than abort/crash later
                goto error;
            }

            // Increase cursor position
            _cursorPositionInString += units;
        }
        // cursor must be at most, after the last char of input
        if (_cursorPositionInString > static_cast<int>(strLength))
            _cursorPositionInString = strLength;

        _blinkingClock.Restart();
        return;

    error:
        _userInput.clear();
    empty:
        _offset = 0.f;
        _cursorPositionInString = _cursorPositionOnScreen = 0;
        return;

    }

    void    KeyboardImpl::_ScrollDown(void)
    {
        const u32   strLength = _userInput.size();
        const char  *cstr = _userInput.c_str();

        // If input is empty
        if (strLength == 0)
            goto empty;

        // If cursor is beyond the string, fix it (can happen when deleting a unicode)
        if (_cursorPositionInString > static_cast<int>(strLength))
            _cursorPositionInString = strLength;

        {
            // Get units to advance to go to the previous char
            const int units = UnitsToPreviousChar(cstr + _cursorPositionInString, _cursorPositionInString);

            if (units < 0)
            {
                ///< Weird char being found, better clear the input than abort/crash later
                goto error;
            }

            // Increase cursor position
            _cursorPositionInString -= units;
        }

        // cursor must be at most, at the begining of the string
        if (_cursorPositionInString < 0)
            _cursorPositionInString = 0;

        _blinkingClock.Restart();
        return;

    error:
        _userInput.clear();
    empty:
        _offset = 0.f;
        _cursorPositionInString = _cursorPositionOnScreen = 0;
        return;
    }

    void    KeyboardImpl::_UpdateScrollInfos(void)
    {
        const char *cstr = _userInput.c_str();
        const float strWidth = Renderer::GetTextSize(cstr);
        // Get units untils current cursor position
        const int unitsToCursor = UnitsToAdvance(cstr, _cursorPositionInString);

        // Weird character, might as well purge the string
        if (unitsToCursor < 0)
        {
            _offset = 0.f;
            _cursorPositionInString = _cursorPositionOnScreen = 0;
            _userInput.clear();
            return;
        }

        // Get width of both before and after cursor
        const float after = Renderer::GetTextSize(cstr + unitsToCursor);
        const float before = strWidth - after;

        // Compute offsets
        if (strWidth > 260.f)
        {
            if (before > 140.f)
                _offset = before + std::min(after, 140.f) - 260.f;
            else
                _offset = 0.f;
            _cursorPositionOnScreen = before - _offset;
        }
        else
        {
            _offset = 0;
            _cursorPositionOnScreen = before;
        }
    }

    bool KeyboardImpl::_HasDecimal()
    {
        return _userInput.find('.') != std::string::npos;
    }

    enum
    {
        CLEAR_NOT_PRESSED = 0,
        CLEAR_JUST_PRESSED,
        CLEAR_QUICK_MODE
    };
    bool    KeyboardImpl::_CheckKeys(void)
    {
        static Clock    backspacetimer;
        static u32      backspaceFastMode = CLEAR_NOT_PRESSED;
        static Time     FastModeWaitTime = Seconds(0.5f);
        static Time     FastClearingFrame = Seconds(0.1f);

        activeKeySetStartIndex = 0;
        activeKeySetEndIndex = _keys->size() - 1;

        if (_layout == Layout::QWERTY)
        {
            _UpdateActiveKeyIndexes();
        }

        // Check cursor position, just in case
        if (_cursorPositionInString > static_cast<int>(_userInput.size()))
            _ScrollUp();

        for (int i = activeKeySetStartIndex; i <= activeKeySetEndIndex; i++)
        {
            std::string  temp;

            int ret = (*_keys)[i](temp);

            if (ret != USER_ABORT)
            {
                if (ret == ~KEY_BACKSPACE)
                {
                    backspaceFastMode = CLEAR_NOT_PRESSED;
                    goto _backspacePressed;
                }
                if (ret == KEY_BACKSPACE)
                {
                    if (backspaceFastMode == CLEAR_NOT_PRESSED)
                    {
                        backspaceFastMode = CLEAR_JUST_PRESSED;
                        backspacetimer.Restart();
                        return (false);
                    }
                    if (backspaceFastMode == CLEAR_JUST_PRESSED)
                    {
                        if (!backspacetimer.HasTimePassed(FastModeWaitTime))
                            return (false);
                        backspaceFastMode = CLEAR_QUICK_MODE;
                    }
                    if (backspaceFastMode == CLEAR_QUICK_MODE)
                    {
                        if (!backspacetimer.HasTimePassed(FastClearingFrame))
                            return (false);
                    }
                    backspacetimer.Restart();
                    goto _backspacePressed;
                }

                // Reset backspace state if any other key is pressed
                backspaceFastMode = CLEAR_NOT_PRESSED;

                if (ret == 0x12345678)
                {
                    if ((_layout == DECIMAL && _userInput.size() >= 18)
                        || (_layout == QWERTY && _max && Utils::GetSize(_userInput) >= _max))
                        return (false);

                    _ClearKeyboardEvent();
                    _KeyboardEvent.type = KeyboardEvent::CharacterAdded;
                    decode_utf8(&_KeyboardEvent.codepoint, (const u8 *)temp.c_str());

                    if (_KeyboardEvent.codepoint == 0x00B1) // ± key
                    {
                        if (_userInput[0] == '-')
                        {
                            _ScrollDown();
                            _userInput.erase(0, 1);
                        }
                        else
                        {
                            _userInput.insert(_userInput.begin(), '-');
                            _ScrollUp();
                        }
                    }
                    else
                    {
                        if (_KeyboardEvent.codepoint == 0x002E && _layout != Layout::QWERTY)
                        {
                            if (_HasDecimal())
                                return false;

                            if (_cursorPositionInString == 0)
                            {
                                _userInput.insert(0, "0");
                                _ScrollUp();
                            }
                            else if (_cursorPositionInString == 1 && _userInput == "-")
                            {
                                _userInput.insert(1, "0");
                                _ScrollUp();
                            }
                        }

                        _userInput.insert(_cursorPositionInString, temp);
                        _ScrollUp();
                    }
                    return true;
                }
                if (ret == KEY_SPACE && (!_max || Utils::GetSize(_userInput) < _max))
                {
                    _userInput.insert(_cursorPositionInString, " ");
                    _ClearKeyboardEvent();
                    _KeyboardEvent.type = KeyboardEvent::CharacterAdded;
                    _KeyboardEvent.codepoint = ' ';
                    _ScrollUp();
                    return (true);
                }
                else if (ret == KEY_CAPS)
                {
                    _useCaps = !_useCaps;
                }
                else if (ret == KEY_ASCII_TOGGLE)
                {
                    _useNumRow = false;
                    _useJPN = false;
                    _useSymbols = false;
                    _useCaps = false;
                }
                else if (ret == KEY_NUM_TOGGLE)
                {
                    _useNumRow = !_useNumRow;
                    _useJPN = false;
                    _useSymbols = false;
                    _useCaps = false;
                }
                else if (ret == KEY_JPN_TOGGLE)
                {
                    if (_QwertyKeys.size() < 298)
                    {
                        _InitQwertyJPN(); // [209 - 248] [249 - 284] [285 - 321] [322 - 361] [362 - 386] [387 - 411]
                        _InitQwertyJPNAlt(); // [412 - 451] [452 - 489] [490 - 527]
                     }

                    _useJPN = !_useJPN;
                    _useNumRow = false;
                    _useSymbols = false;
                    _useCaps = false;

                    _pageIndex = 0;
                }
                else if (ret == KEY_SYMBOLS_TOGGLE)
                {
                    _useSymbols = !_useSymbols;
                    _useNumRow = false;
                    _useJPN = false;
                    _useCaps = false;

                    _pageIndex = 0;
                }
                else if (ret == KEY_PREV_PAGE)
                {
                    _pageIndex--;

                    if (_pageIndex < 0)
                        _pageIndex = 0;
                }
                else if (ret == KEY_NEXT_PAGE)
                {
                    _pageIndex++;

                    if (_useJPN && _pageIndex > 5)
                        _pageIndex = 5;

                    if (_useSymbols && _pageIndex > 2)
                        _pageIndex = 2;
                }
                else
                {
                    if (_layout == DECIMAL && _userInput.length() >= 18)
                        return false;

                    if (_max == 0 || Utils::GetSize(_userInput) < _max)
                    {
                        temp.clear();
                        temp += ret;

                        if (ret == '.')
                        {
                            if (_HasDecimal() && _layout != QWERTY)
                                return false;
                        }

                        _userInput.insert(_cursorPositionInString, temp);
                        _ClearKeyboardEvent();
                        _KeyboardEvent.type = KeyboardEvent::CharacterAdded;
                        _KeyboardEvent.codepoint = ret;
                        _ScrollUp();
                    }
                    return true;
                }
            }
        }

        backspaceFastMode = CLEAR_NOT_PRESSED;
        return false;

    _backspacePressed:
        std::string &&right = _userInput.substr(_cursorPositionInString);
        _userInput.erase(_cursorPositionInString);
        _ScrollDown(); ///< Scroll down before removing the char
        _ClearKeyboardEvent();
        _KeyboardEvent.codepoint = Utils::RemoveLastChar(_userInput);
        _userInput += right;

        if (_KeyboardEvent.codepoint != 0)
        {
            _KeyboardEvent.type = KeyboardEvent::CharacterRemoved;
            return true;
        }
        return false;
    }

    bool    KeyboardImpl::_CheckInput(void)
    {
        if (_layout == QWERTY)
        {
            if (_compare != nullptr)
                return (_compare((void *)&_userInput, _error));
            return (true);
        }

        // In case there's no convert function, always consider input as valid
        if (_convert == nullptr)
            return (true);

        // Always call convert function, can avoid overflow
        void *convertedInput = _convert(_userInput, _isHex);

        // In case there's no callback, always consider input as valid
        if (_compare == nullptr)
            return (true);

        return (_compare(convertedInput, _error));
    }

    bool    KeyboardImpl::_CheckButtons(int &ret)
    {
        bool res = false;
        for (int i = 0; i < static_cast<int>(_strKeys.size()); i++)
        {
            ret = (*_strKeys[i])();
            if (ret != -1)
            {
                ret = i;
                res = true;
                break;
            }
        }
        if (_userSelectedKey && _manualKey != -1)
        {
            ret = _manualKey; res = true; _userSelectedKey = false;
        }
        return res;
    }

    void    KeyboardImpl::_HandleManualKeyPress(Key key)
    {
        _inertialVelocity = 0;

        if (_manualKey == -1)
        {
            if (key & Key::Down)
            {
                int tempKey = _displayScrollbar ? _currentPosition : 0;
                if (!_strKeys[tempKey]->CanUse())
                    tempKey = (int)_strKeys.size() - 1;

                _ChangeManualKey(tempKey);
            } else if (key & Key::Up)
            {
                int tempKey = _displayScrollbar ? std::min((int)_strKeys.size() - 1, _currentPosition + 5) : _strKeys.size() - 1;
                if (!_strKeys[tempKey]->CanUse())
                    tempKey = 1;

                _ChangeManualKey(tempKey);
            }
            else return;
        }
        if (key & (Key::Down | Key::Up))
        {
            if (key & Key::Down)
            {
                int orig = _manualKey;
                int tempKey = _manualKey;

                do {
                    tempKey++;
                } while (tempKey < static_cast<int>(_strKeys.size()) && !_strKeys[tempKey]->CanUse() && tempKey - orig < 4);

                if (tempKey >= static_cast<int>(_strKeys.size()) || tempKey - orig >= 4)
                    tempKey = 0;

                _ChangeManualKey(tempKey);
            }
            else if (key & Key::Up)
            {
                int orig = _manualKey;
                int tempKey = _manualKey;

                do {
                    tempKey--;
                } while (tempKey > 0 && !_strKeys[tempKey]->CanUse() && orig - tempKey < 4);

                if (tempKey < 0 || orig - tempKey >= 4)
                    tempKey = static_cast<int>(_strKeys.size()) - 1;

                _ChangeManualKey(tempKey);
            }

            if (_displayScrollbar)
            {
                int keyRow = _manualKey;
                int positionRow = _currentPosition;

                if (keyRow > positionRow + 6 - 2)
                {
                    positionRow = keyRow - 4;
                    _scrollSize = ((positionRow * 36.01f) + 15) / _scrollJump - _scrollPosition;
                    _manualScrollUpdate = true;
                    _UpdateScroll(0.f, true);
                }
                else if (keyRow < positionRow + 1)
                {
                    positionRow = std::max(keyRow, 0);
                    _scrollSize = ((positionRow * 36.01f) - 15) / _scrollJump - _scrollPosition;
                    _manualScrollUpdate = true;
                    _UpdateScroll(0.f, true);
                }
            }
        }


        if (key == A) {
            if (_manualKey != -1)
                SoundEngine::PlayMenuSound(_strKeys[_manualKey]->GetAcceptSoundEvent());
            _userSelectedKey = true;
        }
    }

    void KeyboardImpl::_ClearKeyboardEvent()
    {
        _KeyboardEvent.codepoint = 0;
        _KeyboardEvent.selectedIndex = -1;
        _KeyboardEvent.affectedKey = (Key)0;
        _KeyboardEvent.renderInterface = nullptr;
    }

    void    KeyboardImpl::_ChangeManualKey(int newVal, bool playSound)
    {
        _manualKey = newVal;
        static bool preventRecursion = false;
        if (_manualKey != _prevManualKey && !preventRecursion && playSound)
            SoundEngine::PlayMenuSound(SoundEngine::Event::CURSOR);
        if (_onKeyboardEvent != nullptr && _owner != nullptr && _manualKey != _prevManualKey && !preventRecursion) {
            preventRecursion = true;
            _ClearKeyboardEvent();
            _KeyboardEvent.type = KeyboardEvent::EventType::SelectionChanged;
            _KeyboardEvent.selectedIndex = _manualKey;
            _onKeyboardEvent(*_owner, _KeyboardEvent);
            preventRecursion = false;
        }
        _prevManualKey = _manualKey;
    }

    // WIll only be used in the hex editor, so no need to do a full implementation
    bool    KeyboardImpl::operator()(int &out)
    {
        _Update(0.f);

        if (!_CheckKeys())
            return (false);

        if (_userInput.size())
        {
            out = (int)_userInput[0];
            if (out >= '0' && out <= '9')
                out -= '0';
            if (out >= 'A' && out <= 'F')
                out = 10 + out - 'A';
            _userInput.pop_back();
            return (true);
        }
        return (false);
    }

    void    KeyboardImpl::CanChangeLayout(bool canChange)
    {
        _canChangeLayout = canChange;
    }
}
