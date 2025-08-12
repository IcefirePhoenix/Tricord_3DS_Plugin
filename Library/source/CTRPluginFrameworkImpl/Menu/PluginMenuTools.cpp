#include "CTRPluginFrameworkImpl/Menu/HotkeysModifier.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuTools.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryTools.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuHome.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuActionReplay.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuExecuteLoop.hpp"
#include "CTRPluginFrameworkImpl/System/Screenshot.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"

#include "CTRPluginFramework/Graphics/OSD.hpp"
#include "CTRPluginFramework/Menu/MessageBox.hpp"
#include "CTRPluginFramework/Menu/PluginMenu.hpp"
#include "CTRPluginFramework/System/Directory.hpp"
#include "CTRPluginFramework/System/System.hpp"
#include "CTRPluginFramework/System/Hook.hpp"
#include "CTRPluginFramework/System/Sleep.hpp"
#include "CTRPluginFramework/System/Process.hpp"
#include "CTRPluginFramework/Utils/StringExtensions.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"

#include <3ds.h>

#include <ctime>
#include <cstring>
#include <cstdio>

namespace CTRPluginFramework
{
    enum Mode
    {
        NORMAL = 0,
        HEXEDITOR,
        GWRAMDUMP,
        SEARCH
    };

    static int g_mode = NORMAL;

    // DO NOT REMOVE THIS COPYRIGHT NOTICE
    static const char g_ctrpfText[] = "Tricord is powered by CTRPluginFramework.";
    static const char g_copyrightText[] = "Copyright (c) The Pixellizer Group";
    static u32 g_textXpos[2] = { 0 };

    PluginMenuTools::PluginMenuTools(HexEditor &hexEditor) :
        _devTools("Developer Tools"),
        _hexEditor(hexEditor),
        _menu(&_devTools, nullptr),
        _exit(false)
    {
        CreateMenu();
    }

    void PluginMenuTools::UpdateSettings(void)
    {
        if (Preferences::IsEnabled(Preferences::DisplayLoadedFiles))
            (*(_devTools.begin() + 3))->AsMenuEntryTools().Enable();
        else
            (*(_devTools.begin() + 3))->AsMenuEntryTools().Disable();

        if (Preferences::IsEnabled(Preferences::WriteLoadedFiles))
            (*(_devTools.begin() + 4))->AsMenuEntryTools().Enable();
        else
            (*(_devTools.begin() + 4))->AsMenuEntryTools().Disable();
    }

    using FsTryOpenFileType = u32(*)(u32, u16*, u32);

    enum HookFilesMode
    {
        NONE = 0,
        OSD = 1,
        FILE = 2
    };
    static Hook g_FsTryOpenFileHook;
    static u32 g_HookMode = NONE;
    //static u32 g_returncode[4];
    static File g_hookExportFile;
    u32 g_FsTryOpenFileAddress = 0;
    static LightLock g_OpenFileLock;

    static u32 FindNearestSTMFD(u32 addr)
    {
        for (u32 i = 0; i < 1024; i++)
        {
            addr -= 4;
            if (*(u16 *)(addr + 2) == 0xE92D)
                return addr;
        }
        return (0);
    }

    static void FindFunction(u32 &FsTryOpenFile)
    {
        const u8 tryOpenFilePat1[] = { 0x0D, 0x10, 0xA0, 0xE1, 0x00, 0xC0, 0x90, 0xE5, 0x04, 0x00, 0xA0, 0xE1, 0x3C, 0xFF, 0x2F, 0xE1 };
        const u8 tryOpenFilePat2[] = { 0x10, 0x10, 0x8D, 0xE2, 0x00, 0xC0, 0x90, 0xE5, 0x05, 0x00, 0xA0, 0xE1, 0x3C, 0xFF, 0x2F, 0xE1 };

        u32    *addr = (u32 *)0x00100000;
        u32    *maxAddress = (u32 *)(Process::GetTextSize() + 0x00100000);

        while (addr < maxAddress)
        {
            if (!memcmp(addr, tryOpenFilePat1, sizeof(tryOpenFilePat1)) || !memcmp(addr, tryOpenFilePat2, sizeof(tryOpenFilePat2)))
            {
                FsTryOpenFile = FindNearestSTMFD((u32)addr);
                break;
            }
            addr++;
        }
    }

    // TODO: clean this whole code
    static u32 FsTryOpenFileCallback(u32 a1, u16 *fileName, u32 mode);
    static bool InitFsTryOpenFileHook(void)
    {
        static bool isInitialized = false;

        if (isInitialized)
            return isInitialized;

        /*auto  createReturncode = [](u32 address, u32 *buf)
        {
            Process::CopyMemory(buf, (void *)address, 8);
            buf[2] = 0xE51FF004;
            buf[3] = address + 8;
        };*/

        // Hook on OpenFile
        u32     FsTryOpenFileAddress = 0;

        FindFunction(FsTryOpenFileAddress);

        // Check that we found the function
        if (FsTryOpenFileAddress != 0)
        {
            // Create lock
            LightLock_Init(&g_OpenFileLock);

            // Initialize the return code
            //createReturncode(FsTryOpenFileAddress, g_returncode);

            // Initialize the hook
            g_FsTryOpenFileHook.InitializeForMitm(FsTryOpenFileAddress, (u32)FsTryOpenFileCallback);
            g_FsTryOpenFileAddress = FsTryOpenFileAddress;
            isInitialized = true;
        }
        else
        {
            OSD::Notify("Error: couldn't find OpenFile function");
            // Disable the option
            Preferences::Clear(Preferences::DisplayLoadedFiles);
        }

        return isInitialized;
    }

    static u32 FsTryOpenFileCallback(u32 a1, u16 *fileName, u32 mode)
    {
        std::string str;

        LightLock_Lock(&g_OpenFileLock);

        if (g_HookMode & OSD)
        {
            // Convert utf16 to utf8
            Utils::ConvertUTF16ToUTF8(str, fileName);
            OSD::Notify(str);
        }

        if (g_HookMode & FILE)
        {
            // Convert utf16 to utf8 if necessary
            if (str.empty())
            {
                Utils::ConvertUTF16ToUTF8(str, fileName);
            }

            // If string isn't empty, write to file
            if (!str.empty())
            {
                g_hookExportFile.WriteLine(str);
            }
        }

        LightLock_Unlock(&g_OpenFileLock);

        return HookContext::GetCurrent().OriginalFunction<u32>(a1, fileName, mode);
    }

    static void _DisplayLoadedFiles(MenuEntryTools *entry)
    {
        // If we must enable the hook
        if (entry->WasJustActivated())
        {
            // Initialize hook
            if (!InitFsTryOpenFileHook())
            {
                MessageBox("Note", "Loaded filenames cannot be displayed on screen. If mods are enabled, please disable them before trying again.")();
                entry->Disable(); ///< Hook failed
            }

            // Enable the hook
            Preferences::Set(Preferences::DisplayLoadedFiles);
            g_HookMode |= OSD;
            g_FsTryOpenFileHook.Enable();
            return;
        }
        if (!entry->IsActivated())
        {
            // Disable OSD
            g_HookMode &= ~OSD;

            Preferences::Clear(Preferences::DisplayLoadedFiles);

            // If there's no task to do on the hook, disable it
            if (g_HookMode == 0)
                g_FsTryOpenFileHook.Disable();
        }
    }

    static void _WriteLoadedFiles(MenuEntryTools *entry)
    {
        // If we must enable the hook
        if (entry->WasJustActivated())
        {
            // Initialize hook
            if (!InitFsTryOpenFileHook())
            {
                MessageBox("Note", "Loaded filenames cannot be logged to the SD card. If mods are enabled, please disable them before trying again.")();
                entry->Disable(); ///< Hook failed
            }

            std::string currDate = Time::GetDate() + Time::GetTime();
            std::string logPath = "/Tricord/Loaded File Logs/";

            if (!Directory::IsExists(logPath))
                Directory::Create(logPath);

            logPath.append(Process::GetRegionCode() + "/");

            if (!Directory::IsExists(logPath))
                Directory::Create(logPath);

            logPath.append(Utils::Format("Loaded File Log - %s.txt", currDate.c_str()));

            if (!File::Exists(logPath))
                File::Create(logPath);

            int mode = File::READ | File::WRITE | File::CREATE | File::APPEND;
            if (File::Open(g_hookExportFile, logPath, mode) != 0)
            {
                OSD::Notify(std::string("Error: Failed to open Loaded File Log"), Color::Red, Color::White);
                entry->Disable();
                return;
            }

            OSD::Notify("File logging started.");
            g_hookExportFile.WriteLine(Utils::Format("### New log: %s ###\n", currDate.c_str()));

            // Enable the hook
            Preferences::Set(Preferences::WriteLoadedFiles);
            g_HookMode |= FILE;
            g_FsTryOpenFileHook.Enable();

            return;
        }

        if (!entry->IsActivated())
        {
            // Disable File exporting
            g_HookMode &= ~FILE;
            g_hookExportFile.Flush();
            g_hookExportFile.Close();

            // If there's no task to do on the hook, disable it
            if (g_HookMode == 0)
                g_FsTryOpenFileHook.Disable();

            Preferences::Clear(Preferences::WriteLoadedFiles);
        }
    }

    void PluginMenuTools::CreateMenu(void)
    {
        _devTools.Append(new MenuEntryTools("Hex Editor", [] { g_mode = HEXEDITOR; }, Icon::DrawGrid));
        _devTools.Append(new MenuEntryTools("Memory Search", [] { g_mode = SEARCH; }, Icon::DrawSearch));
        _devTools.Append(new MenuEntryTools("Gateway RAM Dumper", [] { g_mode = GWRAMDUMP; }, Icon::DrawRAM));
        _devTools.Append(new MenuEntryTools("Display loaded game files on-screen", _DisplayLoadedFiles, true));
        _devTools.Append(new MenuEntryTools("Log loaded filenames to .txt file (breaks w/mods)", _WriteLoadedFiles, true));
        _devTools.Append(new MenuEntryTools("Display touchcreen cursor coordinates", [] { Preferences::Toggle(Preferences::DrawTouchPosition); }, true, Preferences::IsEnabled(Preferences::DrawTouchPosition)));
        _devTools.Append(new MenuEntryTools("Display top screen FPS", [] { Preferences::Toggle(Preferences::ShowTopFps); }, true, Preferences::IsEnabled(Preferences::ShowTopFps)));
        _devTools.Append(new MenuEntryTools("Display bottom screen FPS", [] { Preferences::Toggle(Preferences::ShowBottomFps); }, true, Preferences::IsEnabled(Preferences::ShowBottomFps)));

        // Get strings x position
        g_textXpos[0] = (320 - (int)Renderer::GetTextSize(g_ctrpfText)) / 2;
        g_textXpos[1] = (320 - (int)Renderer::GetTextSize(g_copyrightText)) / 2;
    }

    bool PluginMenuTools::operator()(EventList &eventList, Time &delta)
    {
        if (g_mode == HEXEDITOR)
        {
            if (_hexEditor(eventList))
                g_mode = NORMAL;
            return (false);
        }

        if (g_mode == GWRAMDUMP)
        {
            _gatewayRamDumper();
            g_mode = NORMAL;
            return (false);
        }

        if (g_mode == SEARCH)
        {
            PluginMenuImpl::OpenSearch();
            g_mode = NORMAL;
            return (false);
        }

        // Process Event
        for (size_t i = 0; i < eventList.size(); i++)
            _ProcessEvent(eventList[i]);

        // Update
        _Update(delta);

        static Task task([](void *arg) -> s32
        {
            static_cast<PluginMenuTools *>(arg)->_RenderTop();
            return 0;
        }, static_cast<void *>(this), Task::AppCores);

        task.Start();

        // Render Bottom
        _RenderBottom();

        task.Wait();

        // Check buttons
        bool exit = _exit || Window::BottomWindow.MustClose();
        _exit = false;

        return (exit);
    }

    void PluginMenuTools::_ProcessEvent(Event &event)
    {
        MenuItem *item = nullptr;
        static int selector = -1;

        if (_menu.ProcessEvent(event, &item) == MenuClose)
        {
            if (_menu.GetFolder() == &_devTools)
            {
                _exit = true;
                _menu.Open(&_devTools, selector);
            }
            else
                _menu.Open(&_devTools, selector);
        }
    }

    void PluginMenuTools::_RenderTop(void)
    {
        Renderer::SetTarget(TOP);
        _menu.Draw();
    }

    // TODO: move to PluginMenu, make public
    void    PluginMenuTools::_RenderBottom(void)
    {
        Renderer::SetTarget(BOTTOM);
        Window::BottomWindow.Draw();

        // Draw Framework version
        {
            int posY = 85;
            Renderer::DrawGameFontString("Tricord Build Information:", 40, posY, 300, Color::Gainsboro);
            Renderer::DrawLine(40, posY, 25 * 6, Color::Gainsboro);

            posY += 10;
            Renderer::DrawGameFontString(Utils::Format("Tricord Version: %s", TRICORD_BUILD_METADATA).c_str(), 40, posY, 300, Color::Gainsboro);
            Renderer::DrawGameFontString(Utils::Format("CTRPF Version: %s", CTRPF_BUILD_METADATA).c_str(), 40, posY, 300, Color::Gainsboro);
            Renderer::DrawGameFontString(Utils::Format("Compiled: %s", COMPILE_DATE).c_str(), 40, posY, 300, Color::Gainsboro);

            posY = 165;
            Renderer::DrawGameFontString(g_ctrpfText, g_textXpos[0], posY, 300, Color::Gainsboro);
            Renderer::DrawGameFontString(g_copyrightText, g_textXpos[1], posY, 300, Color::Gainsboro);
        }
    }

    void    PluginMenuTools::_Update(Time delta)
    {
        bool isTouched = Touch::IsDown();

        IntVector touchPos(Touch::GetPosition());
        Window::BottomWindow.Update(isTouched, touchPos);
    }
}
