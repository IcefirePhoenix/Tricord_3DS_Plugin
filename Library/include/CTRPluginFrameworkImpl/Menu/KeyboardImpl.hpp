#ifndef CTRPLUGINFRAMEWORKIMPL_KEYBOARD_HPP
#define CTRPLUGINFRAMEWORKIMPL_KEYBOARD_HPP

#include "CTRPluginFramework/Menu/Keyboard.hpp"
#include "CTRPluginFramework/Sound.hpp"
#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFrameworkImpl/Graphics/TouchKey.hpp"
#include "CTRPluginFrameworkImpl/Graphics/TouchKeyString.hpp"
#include "CTRPluginFrameworkImpl/System.hpp"

#include <string>
#include <vector>

namespace CTRPluginFramework
{
    enum Layout
    {
        QWERTY,
        DECIMAL,
        HEXADECIMAL_FULL,
        HEXADECIMAL_LITE // this has no backspace/submit buttons
    };

    class Keyboard;
    class KeyboardImpl
    {
        using CompareCallback = bool (*)(const void *, std::string &);
        using ConvertCallback = void *(*)(std::string &, bool);
        using OnEventCallback = void (*)(Keyboard &, KeyboardEvent &);
        using KeyIter = std::vector<TouchKey>::iterator;
        using KeyStringIter = std::vector<TouchKeyString>::iterator;

    public:
        KeyboardImpl(const std::string &text = "");
        KeyboardImpl(const std::string &title, const std::string &text);
        explicit KeyboardImpl(Keyboard *kb, const std::string &title, const std::string &text);
        ~KeyboardImpl(void);

        static void InitKeyboards(void);

        void SetLayout(Layout layout);
        void SetHexadecimal(bool isHex);
        bool IsHexadecimal(void) const;
        void SetMaxInput(u32 max);
        void CanAbort(bool canAbort);
        void CanChangeLayout(bool canChange);
        std::string &GetInput(void);
        std::string &GetMessage(void);
        std::string &GetTitle(void);
        u32 &GetMax(void);
        void SetError(std::string &error);

        void SetConvertCallback(ConvertCallback callback);
        void SetCompareCallback(CompareCallback callback);
        void OnKeyboardEvent(OnEventCallback callback);
        int GetSelectedEntry(void);
        void ChangeEntrySound(int entry, SoundEngine::Event soundEvent);
        void Populate(const std::vector<std::string> &input, bool resetScroll);
        void Clear(void);

        Button submitBtn;
        int Run(void);
        void Close(void);
        bool operator()(int &out);

        bool DisplayTopScreen;

    private:
        friend class HexEditor;
        friend class ARCodeEditor;

        Task _DisplayLoadStatus{_ShowLoadProgress, nullptr, Task::AppCores};

        void _RenderTop(void);
        void _RenderBottom(void);
        void _ProcessEvent(Event &event);
        void _UpdateScroll(float delta, bool ignoreTouch);
        void _UpdateActiveKeyIndexes(void);
        void _Update(float delta);
        static s32 _ShowLoadProgress(void *arg);

        static void _InitQwertySequence(void);
        static void _InitQwertyLowercase(void);
        static void _InitQwertyUppercase(void);
        static void _InitQwertyNumRow(void);
        static void _InitQwertyJPN(void);
        static void _InitQwertyJPNAlt(void);
        static void _InitQwertySymbols(void);
        static void _InitQwertyOther(void);
        static void _InitDigitKeys(IntRect &keyPosition, std::vector<TouchKey> &keys, int xStartCoord);
        static void _InitHexKeys(IntRect &keyPosition, std::vector<TouchKey> &keys, int yStart);
        static void _CreateHexLayout(int yStart, std::vector<TouchKey> &keys);
        static void _InitDecimalKeyboard(void);
        static void _InitHexFullKeyboard(void);
        static void _InitHexLiteKeyboard(void);

        void _ScrollUp(void);
        void _ScrollDown(void);
        void _UpdateScrollInfos(void);
        bool _CheckKeys(void);  //<- Return if input have changed
        bool _CheckInput(void); //<- Call compare callback, return true if the input is valid

        bool _CheckButtons(int &ret); //<- for string button
        void _HandleManualKeyPress(Key key);
        void _ClearKeyboardEvent();
        void _ChangeManualKey(int newVal, bool playSound = true);

        Keyboard *_owner{nullptr};

        std::string _title;
        std::string _text;
        std::string _error;
        std::string _userInput;

        bool _canChangeLayout{false};
        bool _canAbort{true};
        bool _isOpen{false};
        bool _askForExit{false};
        bool _errorMessage{false};
        bool _userAbort{false};
        bool _isHex{true};
        bool _mustRelease{false};
        bool _useCaps{false};
        bool _useJPN{false};
        bool _useSymbols{false};
        bool _useNumRow{false};
        int _pageIndex{0};
        float _offset{0.f};
        u32 _max{0};
        Layout _layout{HEXADECIMAL_FULL};
        Clock _blinkingClock;
        int _cursorPositionInString{0};
        int _cursorPositionOnScreen{0};
        bool _showCursor{true};

        CompareCallback _compare{nullptr};
        ConvertCallback _convert{nullptr};
        OnEventCallback _onKeyboardEvent{nullptr};
        KeyboardEvent _KeyboardEvent{};
        std::vector<TouchKey> *_keys{nullptr};

        static std::vector<TouchKey> _DecimalKeys;
        static std::vector<TouchKey> _HexadecimalFullKeys;
        static std::vector<TouchKey> _HexadecimalLiteKeys;
        static std::vector<TouchKey> _QwertyKeys;

        // Custom keyboard stuff
        int _manualKey{0};
        int _prevManualKey{-2};
        bool _manualScrollUpdate{false};
        bool _userSelectedKey{false};
        bool _customKeyboard{false};
        bool _displayScrollbar{false};
        int _currentPosition{0};
        u32 _scrollbarSize{0};
        u32 _scrollCursorSize{0};
        float _scrollSize{0.f};
        float _scrollPosition{0.f};
        float _scrollPadding{0.f};
        float _scrollJump{0.f};
        float _inertialVelocity{0.f};
        float _scrollStart{0.f};
        float _scrollEnd{0.f};
        IntVector _lastTouch;
        Clock _touchTimer;

        std::vector<TouchKeyString *> _strKeys;
    };
}

#endif
