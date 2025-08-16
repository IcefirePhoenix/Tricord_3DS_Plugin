#include "types.h"

#include "CTRPluginFramework/Graphics.hpp"
#include "CTRPluginFramework/System.hpp"
#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFrameworkImpl/Menu.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFrameworkImpl/System.hpp"
#include "CTRPluginFrameworkImpl/System/Screenshot.hpp"

#include <string>
#include <vector>
#include <cstdio>

namespace CTRPluginFramework
{
    bool openSearch, signalQuit;

    PluginMenuImpl  *PluginMenuImpl::_runningInstance = nullptr;
    Mutex           PluginMenuImpl::_trashBinMutex;

    PluginMenuImpl::PluginMenuImpl(std::string &name, std::string &about, u32 menuType) :

        OnFirstOpening(nullptr),
        OnOpening(nullptr),
        OnClosing(nullptr),
        _actionReplay{ new PluginMenuActionReplay() },
        _home(new PluginMenuHome(name)),
        _search(new PluginMenuSearch(_hexEditor)),
        _settings(new PluginMenuSettings()),
        _tools(new PluginMenuTools(_hexEditor)),
        _executeLoop(new PluginMenuExecuteLoop()),
        _guide(new GuideReader()),
        _hexEditor(0x00100000),
        _forceOpen(false)
    {
        SyncOnFrame = false;
        _isOpen = false;
        _aboutToOpen = false;
        _wasOpened = false;
        _pluginRun = true;
    }

    PluginMenuImpl::~PluginMenuImpl(void)
    {
        ProcessImpl::Play(false);
        delete _home;
        delete _search;
        delete _tools;
        delete _executeLoop;
        delete _guide;
        delete _settings;
    }

    void    PluginMenuImpl::SignalQuit(void)
    {
        signalQuit = true;
    }

    void    PluginMenuImpl::Append(MenuItem *item) const
    {
        _home->Append(item);
    }

    void    PluginMenuImpl::AddToHidden(MenuItem *item) const
    {
        _home->AddToHidden(item);
    }

    void    PluginMenuImpl::Callback(CallbackPointer callback)
    {
        // If the callback is going to be added, make sure it's not in the trash bin
        {
            Lock    lock(_trashBinMutex);
            if (_callbacksTrashBin.size())
            {
                auto it = std::find(_callbacksTrashBin.begin(), _callbacksTrashBin.end(), callback);
                if (it != _callbacksTrashBin.end()) _callbacksTrashBin.erase(it);
            }
        }

        if (std::find(_callbacks.begin(), _callbacks.end(), callback) == _callbacks.end())
            _callbacks.push_back(callback);
    }

    void    PluginMenuImpl::RemoveCallback(CallbackPointer callback)
    {
        Lock    lock(_trashBinMutex);

        if (std::find(_callbacksTrashBin.begin(), _callbacksTrashBin.end(), callback) == _callbacksTrashBin.end())
            _callbacksTrashBin.push_back(callback);
    }

    using KeyVector = std::vector<Key>;

    class   KeySequenceImpl
    {
    public:

        KeySequenceImpl(KeyVector sequence) :
            _sequence(sequence), _indexInSequence(0)
        {
        }

        ~KeySequenceImpl() {}

        /**
        * \brief Check the sequence
        * \return true if the sequence is completed, false otherwise
        */
        bool  operator()(void)
        {
            if (Controller::IsKeyDown(_sequence[_indexInSequence]))
            {
                _indexInSequence++;

                if (_indexInSequence >= _sequence.size())
                {
                    _indexInSequence = 0;
                    return (true);
                }

                _timer.Restart();
            }

            if (_timer.HasTimePassed(Seconds(1.f)))
            {
                _indexInSequence = 0;
                _timer.Restart();
            }

            return (false);
        }

    private:
        KeyVector   _sequence;
        Clock       _timer;
        u32         _indexInSequence;
    };

    /*
    ** Run
    **************/
    int    PluginMenuImpl::Run(void)
    {
        Event                   event;
        EventManager            closedManager(EventManager::EventGroups::GROUP_KEYS);
        EventManager            openManager(EventManager::EventGroups::GROUP_KEYS | EventManager::EventGroups::GROUP_TOUCH_AND_SWIPE);
        Clock                   clock;
        Clock                   inputClock;
        int                     mode = 0;
        bool                    shouldClose = false;

        // Component
        PluginMenuActionReplay  &ar = *_actionReplay;
        PluginMenuHome          &home = *_home;
        PluginMenuTools         &tools = *_tools;
        PluginMenuSettings      &settings = *_settings;
        PluginMenuSearch        &search = *_search;
        GuideReader             &guide = *_guide;

        Time                    delta;
        std::vector<Event>      eventList;

        // Set _runningInstance to this menu
        _runningInstance = this;

        // Load backgrounds
        Preferences::Initialize();

        // Load settings
        Preferences::LoadSettings();

        // Refresh hid
        Controller::Update();

        //TODO: remove If Start is pressed, don't auto enable the cheats
        if (Controller::IsKeyPressed(Key::Start) || Controller::IsKeyDown(Key::Start))
            Preferences::Clear(Preferences::AutoEnableSavedCheats);

        _tools->UpdateSettings();

        // Load saved MenuEntry preferences
        Preferences::LoadEntryPreferences(Preferences::IsEnabled(Preferences::AutoEnableSavedCheats), Preferences::IsEnabled(Preferences::AutoEnableFavorites));

        // Update PluginMenuHome variables
        home.Init();

        // Restore Search state
        search.RestoreSearchState();

        if (FwkSettings::Header->isDefaultPlugin)
        {
            MessageBox("Error", "Tricord must be loaded as a title-specific plugin and not the default plugin. Please place Tricord.3gx into \"/luma/plugins/[title ID]\" and try again.\n\nThe plugin will now exit.")();

            ForceExit();
            svcExitThread();
        }
        else
        {
            if (Process::GetRegionCode().empty())
            {
                MessageBox("Error", "The current title is not a supported retail version of The Legend of Zelda: Tri Force Heroes! Supported regions of this game include NA, EU/AUS, and JPN.\n\nTricord will now exit.")();

                ForceExit();
                svcExitThread();
            }
        }

        if (!Directory::IsExists("/Tricord/AR_Backups/"))
            Directory::Create("/Tricord/AR_Backups/");

        ar.Initialize();
        PluginMenuActionReplay::BackupCodes(false);

        OSD::Notify("Plugin ready!", Color::White, Color());
        OSD::Notify("Tricord can now be started.", Color::White, Color());

        // Main loop
        while (_pluginRun)
        {
            // Check Event
            eventList.clear();
            while ((_isOpen && openManager.PollEvent(event)) || (!_isOpen && closedManager.PollEvent(event)) || _forceOpen)
            {
                bool isHotkeysDown = false;

                // If it's a KeyPressed event
                if (event.type == Event::KeyPressed && inputClock.HasTimePassed(Milliseconds(500))
                    && Controller::GetKeysDown() != SystemImpl::RosalinaHotkey)
                {
                    if (Controller::IsKeysPressed(Preferences::MenuHotkeys))
                        isHotkeysDown = true;
                }

                // If MenuHotkeys are pressed
                if (_forceOpen || isHotkeysDown)
                {
                    if (_isOpen) ///< Close menu
                    {
                        SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);

                        if (OnClosing != nullptr)
                            OnClosing();

                        ProcessImpl::Play(true);
                        _isOpen = false;
                        openManager.Clear();

                        // Save settings
                        Preferences::WriteSettings();
                    }
                    else ///< Open menu
                    {
                        bool continueOpening = true;

                        if (OnOpening != nullptr)
                            continueOpening = OnOpening();

                        if (continueOpening)
                        {
                            SoundEngine::PlayMenuSound(SoundEngine::Event::ACCEPT);
                            ProcessImpl::Pause(true);

                            _aboutToOpen = _isOpen = true;
                            closedManager.Clear();
                            _wasOpened = true;

                            // Refresh HexEditor data
                            _hexEditor.Refresh();
                        }
                        // Clean the event list
                        eventList.clear();
                        _forceOpen = false;
                     }
                    inputClock.Restart();
                    continue;
                }

                if (_isOpen)
                {
                    eventList.push_back(event);
                }
            }

            if (_isOpen)
            {
                if (mode == 0)
                {   /* Home */
                    if (OnFrame != nullptr)
                        OnFrame(delta);
                    if (_aboutToOpen)
                        home.UpdateNote();
                    shouldClose = home(eventList, mode, delta);
                }
                /*
                else if (mode == 1)
                { // Mapper

                }
                */
                else if (mode == 2)
                { /* Guide */
                    if (guide(eventList, delta))
                        mode = 0;
                }
                else if (mode == 3)
                { /* Settings */
                    if (settings(eventList, delta))
                        mode = 0;
                }
                else if (openSearch)
                { /* Search */
                    if (search(eventList, delta))
                        openSearch = false;
                    goto __skip;
                }
                else if (mode == 4)
                { /* ActionReplay  */
                    if (ar(eventList, delta))
                        mode = 0;
                }
                else if (mode == 5)
                { /* Tools  */
                    if (tools(eventList, delta))
                        mode = 0;
                }
                _aboutToOpen = false;
                // End frame
                Renderer::EndFrame(shouldClose);

            __skip:
                if (OnFirstOpening != nullptr)
                {
                    static u32 count = 0;

                    if (count > 0)
                    {
                        OnFirstOpening();
                        OnFirstOpening = nullptr;
                        count = 0;
                    }
                    count++;
                }

                delta = clock.Restart();

                // Close menu
                if (shouldClose || SystemImpl::WantsToSleep() || signalQuit)
                {
                    if (shouldClose)
                        SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);
                    ProcessImpl::Play(true);
                    _isOpen = false;
                    openManager.Clear();
                    shouldClose = false;

                    Preferences::WriteSettings();
                    SystemImpl::ReadyToSleep();
                }
            }
            else // menu is closed
            {
                if (SyncOnFrame && !ProcessImpl::IsPaused)
                    LightEvent_Wait(&OSDImpl::OnNewFrameEvent);

                if (SystemImpl::Status())
                {
                    _runningInstance = nullptr;
                    return 0;
                }

                if (signalQuit)
                {
                    ForceExit();
                    svcExitThread();
                    return 0;
                }

                // Lock the AR & execute codes before releasing it
                PluginMenuExecuteLoop::ExecuteAR();

                // Remove callbacks in the trash bin
                if (_callbacksTrashBin.size())
                {
                    Lock    lock(_trashBinMutex);
                    if (_callbacksTrashBin.size())
                    {
                        _callbacks.erase(std::remove_if(_callbacks.begin(), _callbacks.end(),
                            [](CallbackPointer cb)
                        {
                            auto&   trashbin = _runningInstance->_callbacksTrashBin;
                            auto    foundIter = std::find(trashbin.begin(), trashbin.end(), cb);

                            return foundIter != trashbin.end();
                        }),
                            _callbacks.end());

                        _callbacksTrashBin.clear();
                    }
                }

                // Execute callbacks before cheats
                for (size_t i = 0; i < _callbacks.size(); i++)
                    if (_callbacks[i]) _callbacks[i]();

                // Execute activated cheats
                PluginMenuExecuteLoop::ExecuteBuiltin();

                //static KeySequenceImpl konamicode({ DPadUp, DPadUp, DPadDown, DPadDown, DPadLeft, DPadRight, DPadLeft, DPadRight, B, A, B, A });

                //if (konamicode())
                //    OSDImpl::MessColors = !OSDImpl::MessColors;

                if (_wasOpened)
                    _wasOpened = false;
            }
        }

        // Remove Running Instance
        _runningInstance = nullptr;
        return (0);
    }

    void PluginMenuImpl::Close(MenuFolderImpl *folder)
    {
        if (_runningInstance == nullptr)
            return;

        PluginMenuHome &home = *_runningInstance->_home;

        home.Close(folder);
    }

    void PluginMenuImpl::ActivateEnabledCheatsFromFile(const Preferences::Header &header, File &settings)
    {
        if (_runningInstance == nullptr)
            return;

        std::vector<u32> uids;
        MenuFolderImpl *folder = _runningInstance->_home->_folder;

        uids.resize(header.enabledCheatsCount);

        settings.Seek(header.enabledCheatsOffset, File::SET);

        if (settings.Read(uids.data(), sizeof(u32) * header.enabledCheatsCount) == 0)
        {
            for (u32 &uid : uids)
            {
                MenuItem *item = folder->GetItem(uid);

                if (item != nullptr && item->IsEntry())
                    reinterpret_cast<MenuEntryImpl *>(item)->Enable();
            }
        }
    }

    void PluginMenuImpl::ActivateFavoritesFromFile(const Preferences::Header &header, File &settings)
    {
        if (_runningInstance == nullptr)
            return;

        std::vector<u32> uids;
        MenuFolderImpl *folder = _runningInstance->_home->_folder;

        uids.resize(header.favoritesCount);

        settings.Seek(header.favoritesOffset, File::SET);

        if (settings.Read(uids.data(), sizeof(u32) * header.favoritesCount) == 0)
        {
            for (u32 &uid : uids)
            {
                MenuItem *item = folder->GetItem(uid);

                if (item != nullptr && item->IsEntry())
                    reinterpret_cast<MenuEntryImpl *>(item)->Enable();
            }
        }
    }

    void PluginMenuImpl::LoadFavoritesFromFile(const Preferences::Header &header, File &settings)
    {
        if (_runningInstance == nullptr)
            return;

        std::vector<u32>    uids;
        MenuFolderImpl      *folder = _runningInstance->_home->_folder;
        MenuFolderImpl      *starred = _runningInstance->_home->_starredConst;

        uids.resize(header.favoritesCount);

        settings.Seek(header.favoritesOffset, File::SET);

        if (settings.Read(uids.data(), sizeof(u32) * header.favoritesCount) == 0)
        {
            for (u32 &uid : uids)
            {
                MenuItem *item = folder->GetItem(uid);

                if (item != nullptr && !item->_IsStarred())
                {
                    item->_TriggerStar();
                    starred->Append(item, true);
                }
            }
        }
    }

    void PluginMenuImpl::LoadHotkeysFromFile(const Preferences::Header &header, File &settings)
    {
        if (_runningInstance == nullptr || header.hotkeysCount == 0)
            return;

        settings.Seek(header.hotkeysOffset, File::SET);

        u32     buffer[50];
        MenuFolderImpl      *folder = _runningInstance->_home->_root;

        for (size_t count = 0; count < header.hotkeysCount; count++)
        {
            if (settings.Read(buffer, sizeof(u32) * 2) == 0)
            {
                if (settings.Read(&buffer[2], sizeof(u32) * buffer[1]) == 0)
                {
                    MenuItem  *item = folder->GetItem(buffer[0]);

                    if (item == nullptr || !item->IsEntry()) return; ///< An error occurred, abort operation

                    MenuEntry *entry = item->AsMenuEntryImpl().AsMenuEntry();
                    HotkeyManager::OnHotkeyChangeClbk callback = entry->Hotkeys._callback;

                    if (entry->Hotkeys.Count() == buffer[1])
                    {
                        for (u32 i = 0; i < buffer[1]; i++)
                        {
                            entry->Hotkeys[i] = buffer[2 + i];
                            if (callback != nullptr)
                                callback(entry, i);
                        }

                        entry->RefreshNote();
                    }
                    else
                        return; ///< An error occurred so abort operation
                }
            }
        }
    }

    void PluginMenuImpl::LoadNameColorsFromFile(const Preferences::Header &header, File &settings)
    {
        if (_runningInstance == nullptr)
            return;

        int players = 3;
        settings.Seek(header.nameColorOffset, File::SET);

        if (!settings.Read(Preferences::CustomNameColors, sizeof(u32) * players) == 0)
            return;
    }

    void PluginMenuImpl::LoadBookmarkWarpsFromFile(const Preferences::Header &header, File &settings)
    {
        if (_runningInstance == nullptr)
            return;

        Preferences::WarpDestination warps[3];

        settings.Seek(header.warpDestOffset, File::SET);
        if (settings.Read(warps, sizeof(Preferences::WarpDestination) * 3) == 0)
        {
            for (int i = 0; i < 3; i++)
            {
                Preferences::SavedWarps[i] = warps[i];
            }
        }
    }

    void PluginMenuImpl::WriteScreenshotConfigToFile(Preferences::Header &header, File &settings)
    {
        if (_runningInstance == nullptr)
            return;

        u64 offset = settings.Tell();
        header.screenshotScreenCapture = Screenshot::Screens;
        header.screenshotHotkeys = Screenshot::Hotkeys;
        header.screenshotTimer = static_cast<u32>(Screenshot::Timer.AsSeconds());

        std::strncpy(header.screenshotCustomName, Screenshot::Prefix.c_str(), sizeof(header.screenshotCustomName) - 1);
        header.screenshotCustomName[sizeof(header.screenshotCustomName) - 1] = '\0';

        std::strncpy(header.screenshotCustomDir, Screenshot::Path.c_str(), sizeof(header.screenshotCustomDir) - 1);
        header.screenshotCustomDir[sizeof(header.screenshotCustomDir) - 1] = '\0';

        header.screenshotOffset = offset;
    }

    void PluginMenuImpl::WriteEnabledCheatsToFile(Preferences::Header &header, File &settings)
    {
        if (_runningInstance == nullptr)
            return;

        std::vector<u32> uids;
        MenuFolderImpl *folder = _runningInstance->_home->_folder;

        for (MenuItem *item : folder->_items)
        {
            if (item->IsEntry())
            {
                MenuEntryImpl* enabledEntry = reinterpret_cast<MenuEntryImpl *>(item);
                if (enabledEntry->IsActivated() && !enabledEntry->IsRestricted())
                    uids.push_back(item->Uid);
            }
        }

        if (uids.size())
        {
            u64 offset = settings.Tell();

            if (settings.Write(uids.data(), sizeof(u32) * uids.size()) == 0)
            {
                header.enabledCheatsCount = uids.size();
                header.enabledCheatsOffset = offset;
            }
        }
    }

    void PluginMenuImpl::WriteFavoritesToFile(Preferences::Header &header, File &settings)
    {
        if (_runningInstance == nullptr)
            return;

        std::vector<u32> uids;
        MenuFolderImpl *folder = _runningInstance->_home->_starred;

        // restricted MenuFolders/MenuEntries cannot be starred in the first place -> no specific check needed here
        for (MenuItem *item : folder->_items)
        {
            uids.push_back(item->Uid);
        }

        if (uids.size())
        {
            u64 offset = settings.Tell();

            if (settings.Write(uids.data(), sizeof(u32) * uids.size()) == 0)
            {
                header.favoritesCount = uids.size();
                header.favoritesOffset = offset;
            }
        }
    }

    void PluginMenuImpl::WriteCustomNameColorToFile(Preferences::Header &header, File &settings)
    {
        if (_runningInstance == nullptr)
            return;

        u64 offset = settings.Tell();
        for (int index = 0; index < 3; index++)
        {
            if (settings.Write(&Preferences::CustomNameColors[index], sizeof(u32)) != 0)
                return;
        }
        header.nameColorOffset = offset;
    }

    void PluginMenuImpl::WriteBookmarkWarpsToFile(Preferences::Header &header, File &settings)
    {
        if (_runningInstance == nullptr)
            return;

        u64 listOffset = settings.Tell();
        if (!settings.Write(Preferences::SavedWarps, sizeof(Preferences::WarpDestination) * 3) == 0)
            return;

        header.warpDestOffset = listOffset;
    }

    void    PluginMenuImpl::ExtractHotkeys(HotkeysVector &hotkeys, MenuFolderImpl *folder, u32 &size)
    {
        if (folder == nullptr)
            return;

        for (MenuItem *item : folder->_items)
        {
            if (item == nullptr)
                continue;

            if (item->IsFolder())
            {
                ExtractHotkeys(hotkeys, reinterpret_cast<MenuFolderImpl*>(item), size);
                continue;
            }

            MenuEntry *entry = item->AsMenuEntryImpl().AsMenuEntry();

            if (entry && entry->Hotkeys.Count())
            {
                Preferences::HotkeysInfos hInfos = { 0 };

                hInfos.uid = item->Uid;
                hInfos.count = entry->Hotkeys.Count();

                for (u32 j = 0; j < hInfos.count; j++)
                {
                    Hotkey &hk = entry->Hotkeys[j];

                    hInfos.hotkeys.push_back(hk._keys);
                }

                hotkeys.push_back(hInfos);
                size += 2 + hInfos.count;
            }
        }
    }

    void    PluginMenuImpl::WriteHotkeysToFile(Preferences::Header &header, File &settings)
    {
        if (_runningInstance != nullptr)
        {
            MenuFolderImpl  *root = _runningInstance->_home->_root;
            HotkeysVector   hotkeys;
            u32             size = 0;
            u32             *buffer;

            ExtractHotkeys(hotkeys, root, size);

            if (size)
            {
                buffer = new u32[size];
                u64     offset = settings.Tell();
                int     i = 0;

                for (Preferences::HotkeysInfos &hkInfos : hotkeys)
                {
                    buffer[i++] = hkInfos.uid;
                    buffer[i++] = hkInfos.count;
                    for (u32 keys : hkInfos.hotkeys)
                        buffer[i++] = keys;
                }

                if (settings.Write(buffer, sizeof(u32) * size) == 0)
                {
                    header.hotkeysCount = hotkeys.size();
                    header.hotkeysOffset = offset;
                }

                delete [] buffer;
            }
        }
    }

    void    PluginMenuImpl::GetRegionsList(std::vector<Region>& list)
    {
        if (_runningInstance != nullptr)
            _runningInstance->_search->GetRegionsList(list);
    }

    PluginMenuSearch*    PluginMenuImpl::GetSearchInstance(void)
    {
        if (_runningInstance != nullptr)
            return _runningInstance->_search;
        else
            return nullptr;
    }

    void    PluginMenuImpl::ForceExit(void)
    {
        if (_runningInstance != nullptr)
        {
            _runningInstance->_pluginRun = false;
            if (_runningInstance->SyncOnFrame && !ProcessImpl::IsPaused)
                LightEvent_Signal(&OSDImpl::OnNewFrameEvent);
        }
    }

    void    PluginMenuImpl::ForceOpen(void)
    {
        if (_runningInstance != nullptr)
            _runningInstance->_forceOpen = true;
    }

    void    PluginMenuImpl::UnStar(MenuItem* item)
    {
        if (_runningInstance != nullptr)
        {
            _runningInstance->_home->UnStar(item);
        }
    }

    void    PluginMenuImpl::Refresh(void)
    {
        if (_runningInstance != nullptr)
        {
            _runningInstance->_home->Refresh();
        }
    }

    void    PluginMenuImpl::OpenSearch(void)
    {
        openSearch = true;
    }

    PluginMenuImpl* PluginMenuImpl::GetRunningInstance()
    {
        return _runningInstance;
    }

    MenuFolderImpl* PluginMenuImpl::GetRoot() const
    {
        return (_home->_root);
    }

    MenuFolderImpl* PluginMenuImpl::GetHidden() const
    {
        return (_home->_hidden);
    }

    bool    PluginMenuImpl::IsOpen(void) const
    {
        return (_isOpen);
    }

    bool    PluginMenuImpl::WasOpened(void) const
    {
        return (_wasOpened);
    }

    void PluginMenuImpl::AddPluginVersion(void) const
    {
        _home->AddPluginVersion();
    }
}
