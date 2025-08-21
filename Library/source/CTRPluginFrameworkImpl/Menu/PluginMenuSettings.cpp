#include "CTRPluginFrameworkImpl/Menu/HotkeysModifier.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuTools.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuSettings.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryTools.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuExecuteLoop.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuActionReplay.hpp"
#include "CTRPluginFrameworkImpl/System/Screenshot.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"

#include "CTRPluginFramework/Menu/MessageBox.hpp"
#include "CTRPluginFramework/Menu/PluginMenu.hpp"
#include "CTRPluginFramework/System/System.hpp"
#include "CTRPluginFramework/System/Process.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include "CTRPluginFramework/Utils/StringExtensions.hpp"

#include <3ds.h>

#include <ctime>
#include <cstring>
#include <cstdio>

namespace CTRPluginFramework
{
    bool earlyQuit;
    enum Mode
    {
        NORMAL = 0,
        SCREENSHOT,
        AR_BACKUPS,
        STARTUP
    };

    PluginMenuSettings::PluginMenuSettings(void) :
        _settingsMenu("Settings"),
        _screenshotMenu("Screenshot Options"),
        _AR_BackupsMenu("Action Replay Backups"),
        _menuEntryOpts("Startup Options"),
        _menu(&_settingsMenu, nullptr),
        _exit(false)
    {
        CreateMenu();
    }

    static void MenuHotkeyModifier(void)
    {
        u32 keys = Preferences::MenuHotkeys;

        (HotkeysModifier(keys, "Select the hotkeys you'd like to use to open the Tricord menu."))();

        if (keys != 0)
            Preferences::MenuHotkeys = keys;
    }

    void PluginMenuSettings::UpdateSettings(void)
    {
        auto checkboxStartIndex = _settingsMenu.begin();

        if (Preferences::IsEnabled(Preferences::QoL_Patch))
            (*checkboxStartIndex++)->AsMenuEntryTools().Enable();
        else
            (*checkboxStartIndex++)->AsMenuEntryTools().Disable();

        if (Preferences::IsEnabled(Preferences::HIDToggle))
            (*checkboxStartIndex++)->AsMenuEntryImpl().Enable();
        else
            (*checkboxStartIndex++)->AsMenuEntryImpl().Disable();

        if (Preferences::IsEnabled(Preferences::DisableOSDNotifs))
            (*checkboxStartIndex++)->AsMenuEntryImpl().Enable();
        else
            (*checkboxStartIndex++)->AsMenuEntryImpl().Disable();

        if (Preferences::IsEnabled(Preferences::AutoSaveCheats))
            (*checkboxStartIndex++)->AsMenuEntryImpl().Enable();
        else
            (*checkboxStartIndex++)->AsMenuEntryImpl().Disable();

        if (Preferences::IsEnabled(Preferences::AutoEnableFavorites))
            (*checkboxStartIndex++)->AsMenuEntryImpl().Enable();
        else
            (*checkboxStartIndex++)->AsMenuEntryImpl().Disable();

        if (Preferences::IsEnabled(Preferences::ScreenshotEnabled))
            (*(_screenshotMenu.begin()))->AsMenuEntryTools().Enable();
        else
            (*(_screenshotMenu.begin()))->AsMenuEntryTools().Disable();
    }

    void _ExitPlugin(void)
    {
        if (MessageBox("Warning", "Do you really want to exit Tricord? The game will remain running.", DialogType::DialogYesNo)())
        {
            PluginMenuImpl::SignalQuit();
            earlyQuit = true;
        }
    }

    const std::vector<std::string> screens = { "Top screen", "Bottom screen", "Both screens" };

    static MenuEntryTools *g_screenshotEntry;
    static MenuEntryTools *ss_Screen;
    static MenuEntryTools *ss_Hotkey;
    static MenuEntryTools *ss_Timer;
    static MenuEntryTools *ss_Name;
    static MenuEntryTools *ss_Dir;

    std::string KeysToString(u32 keys);
    bool stou32(std::string &input, u32 &res);

    static void UpdateScreenshotStatus(void)
    {
        if (Preferences::IsEnabled(Preferences::ScreenshotEnabled))
            g_screenshotEntry->Enable();
        else
            g_screenshotEntry->Disable();
    }

    static void setScreenShotMode(void)
    {
        Keyboard kb("Screenshot Settings", "Which screen(s) would you like to capture?");
        kb.Populate(screens);

        int mode = kb.Open();
        if (mode != -1)
        {
            Screenshot::Screens = mode + 1;
            ss_Screen->SetName(std::string("Change captured screens: ") + screens[(Screenshot::Screens & SCREENSHOT_BOTH) - 1]);
        }

        UpdateScreenshotStatus();
        Screenshot::UpdateFileCount();
    }

    static void setScreenshotHotkeys(void)
    {
        u32 keys = Screenshot::Hotkeys;
        (HotkeysModifier(keys, "Select the hotkeys you'd like to use to take a\nnew screenshot."))();

        if (keys != 0)
        {
            Screenshot::Hotkeys = keys;
            ss_Hotkey->SetName("Change hotkeys: " + (KeysToString(Screenshot::Hotkeys)));
        }

        UpdateScreenshotStatus();
        Screenshot::UpdateFileCount();
    }

    static void setScreenshotTimer(void)
    {
        std::string desc = "Enter the amount of seconds you would like to continuously take screenshots.\n\nTo disable the timer, enter 0.\n\nNote: May not work as expected on emulator. Timer may also be inconsistent on console.";

        u32 current = static_cast<u32>(Screenshot::Timer.AsSeconds());
        Keyboard keyboard("Screenshot Timer", desc);

        keyboard.IsHexadecimal(false);
        keyboard.OnKeyboardEvent([](Keyboard &kb, KeyboardEvent &event)
        {
            if (event.type == KeyboardEvent::CharacterAdded)
            {
                std::string &input = kb.GetInput();
                u32 value;
                stou32(input, value);

                if (value > 120)
                    input = "120";
            }
        });

        if (keyboard.Open(current, current) != -1)
        {
            Screenshot::Timer = Seconds(static_cast<float>(current));
            if (current)
            {
                ss_Timer->SetName("Set timer: " + std::to_string(current) + " second(s)");
                Screenshot::Screens |= 4; // TIMED flags
            }
            else
                ss_Timer->SetName("Set timer: Not enabled");
        }

        UpdateScreenshotStatus();
        Screenshot::UpdateFileCount();
    }

    static void setScreenshotName(void)
    {
        Keyboard nameKb("Screenshot Settings", "What would you like your screenshot filenames to begin with?");
        std::string out;

        if (nameKb.Open(out, Screenshot::Prefix) != -1)
        {
            Screenshot::Prefix = out;
            ss_Name->SetName("Edit filename: " + Screenshot::Prefix);
        }

        UpdateScreenshotStatus();
        Screenshot::UpdateFileCount();
    }

    static void setScreenshotDir(void)
    {
        std::string out;
        if (Utils::DirectoryPicker(out) == -1)
            return;

        Screenshot::Path = std::move(out);
        if (Screenshot::Path[Screenshot::Path.size() - 1] != '/')
        {
            Screenshot::Path += '/';
            ss_Dir->SetName("Edit directory: [root]" + Screenshot::Path);
        }

        UpdateScreenshotStatus();
        Screenshot::UpdateFileCount();
    }

    void PluginMenuSettings::CreateMenu(void)
    {
        // Main menu
        _settingsMenu.Append(new MenuEntryTools("Change Tricord menu hotkeys", MenuHotkeyModifier, Icon::DrawGameController));
        _settingsMenu.Append(new MenuEntryTools("Startup Options", nullptr, Icon::DrawTools, new u32(STARTUP)));
        _settingsMenu.Append(new MenuEntryTools("Screenshot Options", nullptr, Icon::DrawCamera, new u32(SCREENSHOT)));
        _settingsMenu.Append(new MenuEntryTools("Action Replay Backups", nullptr, Icon::DrawRestart, new u32(AR_BACKUPS)));
        _settingsMenu.Append(new MenuEntryTools("Enable QoL patches", [] { Preferences::Toggle(Preferences::QoL_Patch); }, true, Preferences::IsEnabled(Preferences::QoL_Patch)));
        _settingsMenu.Append(new MenuEntryTools("Disable HID memory allocation", [] { Preferences::Toggle(Preferences::HIDToggle); }, true, Preferences::IsEnabled(Preferences::HIDToggle)));
        _settingsMenu.Append(new MenuEntryTools("Disable on-screen notification messages", [] { Preferences::Toggle(Preferences::DisableOSDNotifs); }, true, Preferences::IsEnabled(Preferences::DisableOSDNotifs)));
        _settingsMenu.Append(new MenuEntryTools("Shutdown Tricord", _ExitPlugin, Icon::DrawShutdown));

        // Submenus
        u32 time = static_cast<u32>(Screenshot::Timer.AsSeconds());
        std::string timerName = time ? "Set timer: " + std::to_string(time) + " second(s)" : "Set timer: Not enabled";

        _screenshotMenu.Append((g_screenshotEntry = new MenuEntryTools("Enable screenshot tool", [] { Preferences::Toggle(Preferences::ScreenshotEnabled); }, true, Preferences::IsEnabled(Preferences::ScreenshotEnabled))));
        _screenshotMenu.Append((ss_Screen = new MenuEntryTools(std::string("Change captured screens: ") + screens[(Screenshot::Screens & SCREENSHOT_BOTH) - 1], setScreenShotMode, Icon::DrawSettings)));
        _screenshotMenu.Append((ss_Hotkey = new MenuEntryTools("Change hotkeys: " + (KeysToString(Screenshot::Hotkeys)), setScreenshotHotkeys, Icon::DrawSettings)));
        _screenshotMenu.Append((ss_Timer = new MenuEntryTools(timerName, setScreenshotTimer, Icon::DrawSettings)));
        _screenshotMenu.Append((ss_Name = new MenuEntryTools("Edit filename: " + Screenshot::Prefix, setScreenshotName, Icon::DrawSettings)));
        _screenshotMenu.Append((ss_Dir = new MenuEntryTools("Edit directory: [root]" + Screenshot::Path, setScreenshotDir, Icon::DrawSettings)));

        _menuEntryOpts.Append(new MenuEntryTools("Automatically re-enable currently active cheats", [] { Preferences::Toggle(Preferences::AutoSaveCheats); Preferences::Toggle(Preferences::AutoEnableSavedCheats); }, true, Preferences::IsEnabled(Preferences::AutoSaveCheats)));
        _menuEntryOpts.Append(new MenuEntryTools("Automatically enable Favorites", [] { Preferences::Toggle(Preferences::AutoEnableFavorites); }, true, Preferences::IsEnabled(Preferences::AutoEnableFavorites)));

        _AR_BackupsMenu.Append(new MenuEntryTools("Backup Action Replay codes now", [] { PluginMenuActionReplay::BackupCodes(true); }, Icon::DrawSettings));
        _AR_BackupsMenu.Append(new MenuEntryTools("Restore Action Replay codes from backup", [] { PluginMenuActionReplay::RestoreFromBackup(false); }, Icon::DrawSettings));
    }

    bool PluginMenuSettings::operator()(EventList &eventList, Time &delta)
    {
        for (size_t i = 0; i < eventList.size(); i++)
            _ProcessEvent(eventList[i]);

        _Update(delta);

        static Task task([](void *arg) -> s32
        {
            static_cast<PluginMenuSettings *>(arg)->_RenderTop();
            return 0;
        }, static_cast<void *>(this), Task::AppCores);


        task.Start();
        _RenderBottom();
        task.Wait();

        bool exit = _exit || Window::BottomWindow.MustClose() || earlyQuit;
        _exit = false;

        return exit;
    }

    void PluginMenuSettings::_ProcessEvent(Event &event)
    {
        MenuItem *item = nullptr;
        static int selector = -1;

        int ret = _menu.ProcessEvent(event, &item);
        if (ret == EntrySelected && item != nullptr)
        {
            void *arg = ((MenuEntryTools *)item)->GetArg();

            if (arg == nullptr); // skip
            else if (*(u32 *)arg == SCREENSHOT)
            {
                selector = _menu._selector;
                UpdateScreenshotStatus();
                _menu.Open(&_screenshotMenu);
            }
            else if (*(u32 *)arg == AR_BACKUPS)
            {
                selector = _menu._selector;
                _menu.Open(&_AR_BackupsMenu);
            }
            else if (*(u32 *)arg == STARTUP)
            {
                selector = _menu._selector;
                _menu.Open(&_menuEntryOpts);
            }
        }

        if (ret == MenuClose)
        {
            if (_menu.GetFolder() == &_settingsMenu)
                _exit = true;
            else
                _menu.Open(&_settingsMenu, selector);
        }
    }

    void PluginMenuSettings::_RenderTop(void)
    {
        Renderer::SetTarget(TOP);
        _menu.Draw();
    }

    void    PluginMenuSettings::_RenderBottom(void)
    {
        Render::DisplayPluginInfo();
    }

    void    PluginMenuSettings::_Update(Time delta)
    {
        bool isTouched = Touch::IsDown();
        IntVector touchPos(Touch::GetPosition());

        Window::BottomWindow.Update(isTouched, touchPos);
    }
}