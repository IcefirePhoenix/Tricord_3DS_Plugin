#ifndef CTRPLUGINFRAMEWORKIMPL_PLUGINMENUIMPL_HPP
#define CTRPLUGINFRAMEWORKIMPL_PLUGINMENUIMPL_HPP

#include "CTRPluginFramework.hpp"
#include "CTRPluginFrameworkImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/GuideReader.hpp"
#include "CTRPluginFrameworkImpl/Menu/DiscordInfo.hpp"

#include "CTRPluginFrameworkImpl/Menu/PluginMenuHome.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuSearch.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuTools.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuSettings.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuExecuteLoop.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuActionReplay.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"

#include <vector>

namespace CTRPluginFramework
{
    class PluginMenuImpl
    {
        using CallbackPointer = void (*)(void);
        using FuncPointer = void (*)(void);
		using OnOpeningCallback = bool (*)(void);
        using OnClosingCallback = void (*)(void);
        using FramePointer = void (*)(Time);
        using HotkeysVector = std::vector<Preferences::HotkeysInfos>;
    public:

        PluginMenuImpl(std::string &name, std::string &note, u32 menuType);
        ~PluginMenuImpl(void);

        void    Append(MenuItem *item) const;
        void    AddToHidden(MenuItem *item) const;
        void    Callback(CallbackPointer callback);
        void    RemoveCallback(CallbackPointer callback);
        int     Run(void);

        static void    Close(MenuFolderImpl *menuFolderImpl);

        static void ActivateEnabledCheatsFromFile(const Preferences::Header &header, File &settings);
        static void ActivateFavoritesFromFile(const Preferences::Header &header, File &settings);
        static void LoadFavoritesFromFile(const Preferences::Header &header, File &settings);
        static void LoadHotkeysFromFile(const Preferences::Header &header, File &settings);
        static void LoadBookmarkWarpsFromFile(const Preferences::Header &header, File &settings);
        static void LoadFaceExprFromFile(const Preferences::Header &header, File &settings);
        static void LoadNameColorsFromFile(const Preferences::Header &header, File &settings);

        static void WriteEnabledCheatsToFile(Preferences::Header &header, File &settings);
        static void WriteFavoritesToFile(Preferences::Header &header, File &settings);
        static void WriteBookmarkWarpsToFile(Preferences::Header &header, File &settings);
        static void WriteFaceExprsToFile(Preferences::Header &header, File &settings);
        static void WriteCustomNameColorToFile(Preferences::Header &header, File &settings);
        static void WriteScreenshotConfigToFile(Preferences::Header &header, File &settings);
        static void ExtractHotkeys(HotkeysVector &hotkeys, MenuFolderImpl *folder, u32 &size);
        static void WriteHotkeysToFile(Preferences::Header &header, File &file);
        static void GetRegionsList(std::vector<Region> &list);
        static PluginMenuSearch* GetSearchInstance(void);

        // Used to forcefully exit a menu
        static void ForceExit(void);
        static void ForceOpen(void);
        static void SignalQuit(void);

        static void UnStar(MenuItem *item);
        static void Refresh(void);
        static void OpenSearch(void);

        static PluginMenuImpl* GetRunningInstance();

        MenuFolderImpl *GetRoot(void) const;
        MenuFolderImpl *GetHidden(void) const;
        bool    IsOpen(void) const;
        bool    WasOpened(void) const;
        void    AddPluginVersion(bool isDev = false) const;

        FuncPointer			OnFirstOpening;
        FuncPointer			OnReady;
		OnOpeningCallback	OnOpening;
        OnClosingCallback	OnClosing;
        FramePointer		OnFrame;
        bool				SyncOnFrame;
    private:
        friend std::string &PluginMenu::Title(void);
        static PluginMenuImpl       *_runningInstance;
        static Mutex                _trashBinMutex;

        bool                        _isOpen;
        bool                        _aboutToOpen;
        bool                        _wasOpened;
        bool                        _pluginRun;

        PluginMenuActionReplay      *_actionReplay;
        PluginMenuHome              *_home;
        PluginMenuSearch            *_search;
        PluginMenuSettings          *_settings;
        PluginMenuTools             *_tools;
        PluginMenuExecuteLoop       *_executeLoop;
        GuideReader                 *_guide;
        DiscordInfo                 *_discordMenu;
        HexEditor                   _hexEditor;
        std::vector<CallbackPointer>     _callbacks;
        std::vector<CallbackPointer>     _callbacksTrashBin;
        bool                        _forceOpen;
    };
}

#endif
