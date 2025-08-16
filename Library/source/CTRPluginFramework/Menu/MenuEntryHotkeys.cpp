#include "Unicode.h"

#include "CTRPluginFramework/Menu/MenuEntryHotkeys.hpp"
#include "CTRPluginFramework/System/Controller.hpp"
#include "CTRPluginFrameworkImpl/Menu/HotkeysModifier.hpp"
#include "CTRPluginFramework/Menu/MenuEntry.hpp"
#include "CTRPluginFramework/Menu/Keyboard.hpp"
#include "CTRPluginFramework/System/System.hpp"
#include "CTRPluginFrameworkImpl/System/SystemImpl.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"

namespace CTRPluginFramework
{
    Hotkey::Hotkey() : _keys(0)
    {
    }

    Hotkey::Hotkey(u32 keys, const std::string& name) :
    _keys(keys), _name(name)
    {
    }

    Hotkey::~Hotkey()
    {
    }

    void    Hotkey::operator=(u32 keys)
    {
        _keys = keys;
    }

    void    Hotkey::operator=(const std::string& name)
    {
        _name = name;
    }

    bool    Hotkey::IsDown(void) const
    {
        return (Controller::IsKeysDown(_keys));
    }

    bool    Hotkey::IsPressed(void) const
    {
        return (Controller::IsKeysPressed(_keys));
    }

    u32     Hotkey::GetKeys(void)
    {
        return (_keys);
    }

    const std::string& Hotkey::GetName() const
    {
        return _name;
    }

    // note: refer to Key enum in Controller class
    static const std::string g_keyName[32] =
    {
        FONT_A,
        FONT_B,
        "Select",
        "Start",
        "D-Pad Right",
        "D-Pad Left",
        "D-Pad Up",
        "D-Pad Down",
        FONT_R,
        FONT_L,
        FONT_X,
        FONT_Y,
        "",
        "",
        "ZL",
        "ZR",
        "",
        "",
        "",
        "",
        "Touchscreen", // unused for now
        "",
        "",
        "",
        "C-Pad Right",
        "C-Pad Left",
        "C-Pad Up",
        "C-Pad Down",
        "C-Stick Right",
        "C-Stick Left",
        "C-Stick Up",
        "C-Stick Down"
    };

    std::string     KeysToString(u32 keys)
    {
        std::string ret;
        bool    add = false;

        for (int i = 0; i < 32; i++)
        {
            if (keys & (1u << i))
            {
                if (add)
                {
                    ret += " + ";
                }
                ret += g_keyName[i];
                add = true;
            }
        }
        return (ret);
    }

    void    Hotkey::AskForKeys(void)
    {
        FwkSettings& settings = FwkSettings::Get();
        std::string lockKeys = (!System::IsNew3DS() || !settings.AreN3DSButtonsAvailable) ? "\n\n\nNote: o2/3DS detected; checkboxes for\nn2/3DS buttons have been disabled and\ncannot be selected in this menu." : "";

        HotkeysModifier(_keys, "Select new hotkeys for the following entry:\n\n" + _name + lockKeys)();
    }


    std::string     Hotkey::ToString(bool withName) const
    {
        if (withName)
            return (KeysToString(_keys) + ": " + _name);

        return (KeysToString(_keys));
    }

    HotkeyManager::HotkeyManager(MenuEntry *owner) :
    _owner(owner),
    _callback(nullptr)
    {
    }

    HotkeyManager::~HotkeyManager()
    {
    }

    void    HotkeyManager::operator+=(const Hotkey& hotkey)
    {
        _hotkeys.push_back(hotkey);
    }

    static Hotkey g_dummy = Hotkey(0, "Dummy");

    Hotkey& HotkeyManager::operator[](u32 index)
    {
        if (index >= _hotkeys.size())
            return (g_dummy);

        return (_hotkeys[index]);
    }

    Hotkey& HotkeyManager::operator[](const std::string& name)
    {
        for (auto& hk : _hotkeys)
            if (hk._name == name)
                return hk;

        return (g_dummy);
    }

    std::string     HotkeyManager::ToString(void)
    {
        if (_hotkeys.size() == 0)
            return ("");

        std::string ret("Hotkeys:\n");

        for (Hotkey &hotkey : _hotkeys)
        {
            ret += hotkey.ToString(true) + "\n";
        }

        return (ret);
    }

    void    HotkeyManager::AskForKeys(void)
    {
        if (_hotkeys.size() == 0)
            return;

        Keyboard keyboard("Hotkey Modifier", "Select an option to configure its current hotkeys.\n\nNote: ZL/ZR and C-stick controls are available only on n2/3DS models.");
        std::vector<std::string> hkNames;
        int ret;

        for (Hotkey &hotkey : _hotkeys)
            hkNames.push_back(hotkey._name);

        keyboard.Populate(hkNames);

        do
        {
            ret = keyboard.Open();

            if (ret != -1)
            {
                _hotkeys[ret].AskForKeys();

                if (_callback != nullptr)
                    _callback(_owner, ret);

                _owner->RefreshNote();
            }

        }
        while (ret != -1);
    }

    void    HotkeyManager::OnHotkeyChangeCallback(OnHotkeyChangeClbk callback)
    {
        _callback = callback;
    }

    u32     HotkeyManager::Count(void)
    {
        return (_hotkeys.size());
    }
}
