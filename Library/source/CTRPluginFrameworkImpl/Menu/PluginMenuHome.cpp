#include "CTRPluginFrameworkImpl/Menu/PluginMenuHome.hpp"
#include "CTRPluginFramework/Menu/MenuFolder.hpp"
#include "CTRPluginFramework/Sound.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Icon.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuExecuteLoop.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "qrcodegen.hpp"
#include <CTRPluginFramework.hpp>
#include <cstdlib>
#include <cstring>

namespace CTRPluginFramework
{

    PluginMenuHome::PluginMenuHome(std::string& name) :
        _noteTB("", "", IntRect(40, 30, 320, 180)),

        _showStarredBtn(Button::Toggle | Button::GameFont, "Favorites", IntRect(35, 80, 120, 32), Icon::DrawMenuButton),
        _freecamBtn(Button::Toggle | Button::GameFont, "Freecam", IntRect(35, 120, 120, 32), Icon::DrawMenuButton),
        _arBtn(Button::GameFont, "Action Replay", IntRect(35, 160, 120, 32), Icon::DrawMenuButton),
        _gameModeBtn(Button::Toggle | Button::GameFont, "Game Modes", IntRect(166, 80, 120, 32), Icon::DrawMenuButton),
        _settingsBtn(Button::GameFont, "Settings", IntRect(166, 120, 120, 32), Icon::DrawMenuButton),
        _toolsBtn(Button::GameFont, "Dev Tools", IntRect(166, 160, 120, 32), Icon::DrawMenuButton),

        _faqBtn(Button::Icon, IntRect(287, 205, 25, 25), Icon::DrawTricord),
        _discordBtn(Button::Icon, IntRect(254, 207, 25, 25), Icon::DrawDiscord),
        _controllerBtn(Button::Icon | Button::Toggle, IntRect(182, 41, 25, 25), Icon::Draw3DS),

        _AddFavoriteBtn(Button::Icon | Button::Toggle, IntRect(149, 41, 25, 25), Icon::DrawAddFavorite),
        _InfoBtn(Button::Icon | Button::Toggle, IntRect(116, 41, 25, 25), Icon::DrawInfoNew)
    {
        _root = _folder = new MenuFolderImpl("Main Menu - Code Collection");
        _hidden = new MenuFolderImpl("Hidden");
        _starredConst = _starred = new MenuFolderImpl("Favorites");
        _freecamFolder = new MenuFolderImpl("Freecam");
        _gameModesFolder = new MenuFolderImpl("Game Modes");
        _returnFolder = nullptr;

        _starMode = false;
        _altGameplayMode = false;
        _freecamMode = false;
        _selector = 0;
        _selectedTextSize = 0;
        _scrollOffset = 0.f;
        _maxScrollOffset = 0.f;
        _reverseFlow = false;
        _closedRootFolder = false;

        _mode = 0;
    }

    bool PluginMenuHome::operator()(EventList& eventList, int& mode, Time& delta)
    {
        bool close;

        static Task top([](void *arg)
        {
            PluginMenuHome *home = reinterpret_cast<PluginMenuHome *>(arg);

            Renderer::SetTarget(TOP);
            if (home->_noteTB.IsOpen())
            {
                home->_showStarredBtn.Lock();
                home->_freecamBtn.Lock();
                home->_gameModeBtn.Lock();
                home->_faqBtn.Lock();
                home->_settingsBtn.Lock();
                home->_arBtn.Lock();
                home->_toolsBtn.Lock();
                home->_AddFavoriteBtn.Lock();
                home->_controllerBtn.Lock();
                home->_discordBtn.Lock();
                home->_noteTB.Draw();
            }
            else
            {
                if (home->_freecamMode) // gamemode toggle remains locked
                    home->_freecamBtn.Unlock();
                else if (home->_altGameplayMode)
                {
                    home->_gameModeBtn.Unlock();
                    home->_freecamBtn.Unlock();
                }
                else if (home->_starMode) // freecam and gamemode toggle remain locked
                {
                    home->_showStarredBtn.Unlock();
                    home->_faqBtn.Unlock();
                    home->_settingsBtn.Unlock();
                    home->_arBtn.Unlock();
                    home->_toolsBtn.Unlock();
                }
                else // no special mode is active
                {
                    home->_gameModeBtn.Unlock();
                    home->_freecamBtn.Unlock();
                    home->_showStarredBtn.Unlock();
                    home->_faqBtn.Unlock();
                    home->_settingsBtn.Unlock();
                    home->_arBtn.Unlock();
                    home->_toolsBtn.Unlock();
                }

                // always unlock regardless of active mode
                home->_AddFavoriteBtn.Unlock();
                home->_controllerBtn.Unlock();
                home->_discordBtn.Unlock();
                home->_InfoBtn.Enable();
                home->_InfoBtn.Unlock();
                home->_RenderTop();
            }

            return (s32)0;

        }, this, Task::AppCores);

        _mode = mode;
        bool closeRequested = Window::BottomWindow.MustClose();

        // Process events
        if (_noteTB.IsOpen())
        {
            if (closeRequested)
            {
                _noteTB.Close();
                _InfoBtn.SetState(false);
                closeRequested = false;
            }

            for (size_t i = 0; i < eventList.size(); i++)
            {
                if (_noteTB.ProcessEvent(eventList[i]) == false)
                {
                    _InfoBtn.SetState(false);
                    break;
                }
            }
        }
        else
        {
            for (size_t i = 0; i < eventList.size(); i++)
                _ProcessEvent(eventList[i]);
        }

        if (_toolsBtn()) _toolsBtn_OnClick();
        if (_showStarredBtn()) _showStarredBtn_OnClick();
        if (_freecamBtn()) _freecamBtn_OnClick();
        if (_gameModeBtn()) _gameModeBtn_OnClick();
        if (_faqBtn()) _faqBtn_OnClick();
        if (_settingsBtn()) _settingsBtn_OnClick();
        if (_arBtn()) _actionReplayBtn_OnClick();
        if (_AddFavoriteBtn()) _StarItem();
        if (_InfoBtn()) _InfoBtn_OnClick();
        if (_discordBtn()) _discordBtn_OnClick();
        if (_controllerBtn()) _controllerBtn_OnClick();

        // allow submenus to return to main menu via return button
        // note: closeRequest is consumed, preventing plugin from being closed entirely
        if (_starMode && closeRequested)
        {
            _showStarredBtn_OnClick();
            _showStarredBtn.SetState(false);
            closeRequested = false;
        }
        else if (_freecamMode && closeRequested)
        {
            _freecamBtn_OnClick();
            _freecamBtn.SetState(false);
            closeRequested = false;
        }
        else if (_altGameplayMode && closeRequested)
        {
            _gameModeBtn_OnClick();
            closeRequested = false;
        }

        top.Start();
        top.Wait();

        _Update(delta);
        _RenderBottom();

        mode = _mode;

        close = closeRequested;
        close |= _closedRootFolder && !Preferences::IsEnabled(Preferences::Prevent_Close_Menu_B);

        _closedRootFolder = false;

        return close;
    }

    void PluginMenuHome::Append(MenuItem* item) const
    {
        _folder->Append(item);
    }

    void PluginMenuHome::AddToHidden(MenuItem* item) const
    {
        _hidden->Append(item);
    }

    void PluginMenuHome::ValidateFolderVisibility(MenuFolderImpl *&folder, int altModeIndex)
    {
        do
        {
            // Root folder
            if (folder->_container == nullptr)
                break;

            while (!folder->Flags.isVisible)
            {
                MenuFolderImpl *p = folder->_Close(_selector, altModeIndex);

                if (p)
                {
                    folder = p;

                    if (_selector >= 1)
                        _selector--;
                }
                else
                    break;
            }

        } while (true);
    }

    void PluginMenuHome::Refresh(void)
    {
        // If the currently selected folder is root
        // Nothing to do
        if (_folder->_container != nullptr)
        {
            // If current folder is hidden, close it
            while (!_folder->Flags.isVisible)
            {
                MenuFolderImpl *p = _folder->_Close(_selector, 0);

                if (p)
                {
                    _folder = p;
                    if (_selector >= 1)
                        _selector--;
                }
                else
                    break;
            }
        }

        ValidateFolderVisibility(_starred, 1);
        ValidateFolderVisibility(_freecamFolder, 2);
        ValidateFolderVisibility(_gameModesFolder, 3);

        // Check for the validity of _selector range
        MenuFolderImpl *folder = _folder;

        if (_freecamMode)
            folder = _freecamFolder;
        else if (_starMode)
            folder = _starred;
        else if (_altGameplayMode)
            folder = _gameModesFolder;

        if (_selector >= static_cast<int>(folder->ItemsCount()))
            _selector = 0;
    }

#define IsUnselectableEntry(item) (item->IsEntry() && item->AsMenuEntryImpl()._flags.isUnselectable)
    static u32  SelectableEntryCount(MenuFolderImpl &folder)
    {
        u32 count = 0;

        for (u32 i = 0; i < folder.ItemsCount(); i++)
        {
            MenuItem *item = folder[i];

            if (item->IsEntry() && item->AsMenuEntryImpl()._flags.isUnselectable)
                continue;
            else
                count++;
        }
        return (count);
    }

    static void ScrollUp(int &selector, MenuFolderImpl &folder, int step)
    {
        // If there's no selectable entry in the folder, return
        if (!SelectableEntryCount(folder))
            return;

        // We're already at the begining
        if (selector == 0)
        {
            // Else select last item
            selector = folder.ItemsCount() - 1;
            // If entry is unselectable scroll again
            if (IsUnselectableEntry(folder[selector]))
                ScrollUp(selector, folder, step);
            else
                SoundEngine::PlayMenuSound(SoundEngine::Event::CURSOR);
            return;
        }
        // Else go up
        selector -= step;
        if (selector < 0)
            selector = 0;
        // If entry is unselectable, scroll again
        if (IsUnselectableEntry(folder[selector]))
        {
            step = step > 1 ? step - 1 : 1;
            ScrollUp(selector, folder, step);
        }
        else
            SoundEngine::PlayMenuSound(SoundEngine::Event::CURSOR);
    }

    static void ScrollDown(int &selector, MenuFolderImpl &folder, int step)
    {
        // If there's no selectable entry in the folder, return
        if (!SelectableEntryCount(folder))
            return;

        // We're already at the end
        if (selector == static_cast<int>(folder.ItemsCount()) - 1)
        {
            // Else select first item
            selector = 0;
            // If entry is unselectable scroll again
            if (IsUnselectableEntry(folder[selector]))
                ScrollDown(selector, folder, step);
            else
                SoundEngine::PlayMenuSound(SoundEngine::Event::CURSOR);
            return;
        }
        // Else go down
        selector += step;
        if (selector >= static_cast<int>(folder.ItemsCount()))
            selector = folder.ItemsCount() - 1;
        // If entry is unselectable, scroll again
        if (IsUnselectableEntry(folder[selector]))
        {
            step = step > 1 ? step - 1 : 1;
            ScrollDown(selector, folder, step);
        }
        else
            SoundEngine::PlayMenuSound(SoundEngine::Event::CURSOR);
    }

    //###########################################
    // Process Event
    //###########################################
    void PluginMenuHome::_ProcessEvent(Event& event)
    {
        static Clock fastScroll;
        static Clock inputClock;
        static MenuItem* last = nullptr;

        int altModeIndex = 0;
        MenuItem *item;
        MenuFolderImpl *folder = _folder;

        if (_freecamMode)
        {
            folder = _freecamFolder;
            altModeIndex = 2;
        }
        else if (_starMode)
        {
            folder = _starred;
            altModeIndex = 1;
        }
        else if (_altGameplayMode)
        {
            folder = _gameModesFolder;
            altModeIndex = 3;
        }

        switch (event.type)
        {
            case Event::KeyDown:
            {
                if (fastScroll.HasTimePassed(Milliseconds(800)) && inputClock.HasTimePassed(Milliseconds(100)))
                {
                    switch (event.key.code)
                    {
                            /*
                            ** Selector
                            **************/
                        case Key::CPadUp:
                        case Key::DPadUp:
                        {
                            ScrollUp(_selector, *folder, 1);
                            break;
                        }
                        case Key::CPadDown:
                        case Key::DPadDown:
                        {
                            ScrollDown(_selector, *folder, 1);
                            break;
                        }
                        case Key::CPadLeft:
                        case Key::DPadLeft:
                        {
                            ScrollUp(_selector, *folder, 4);
                            break;
                        }
                        case Key::CPadRight:
                        case Key::DPadRight:
                        {
                            ScrollDown(_selector, *folder, 4);
                            break;
                        }
                        default: break;
                    }
                    inputClock.Restart();
                }
                break;
            } // Event::KeyDown
            case Event::KeyPressed:
            {
                switch (event.key.code)
                {
                        /*
                        ** Selector
                        **************/
                    case Key::CPadUp:
                    case Key::DPadUp:
                    {
                        ScrollUp(_selector, *folder, 1);
                        fastScroll.Restart();
                        break;
                    }
                    case Key::CPadDown:
                    case Key::DPadDown:
                    {
                        ScrollDown(_selector, *folder, 1);
                        fastScroll.Restart();
                        break;
                    }
                    case Key::CPadLeft:
                    case Key::DPadLeft:
                    {
                        ScrollUp(_selector, *folder, 4);
                        fastScroll.Restart();
                        break;
                    }
                    case Key::CPadRight:
                    case Key::DPadRight:
                    {
                        ScrollDown(_selector, *folder, 4);
                        fastScroll.Restart();
                        break;
                    }
                        /*
                        ** Trigger entry
                        ** Top Screen
                        ******************/
                    case Key::A:
                    {
                        _TriggerEntry();
                        break;
                    }
                        /*
                        ** Closing Folder
                        ********************/
                    case Key::B:
                    {
                        MenuFolderImpl *newFolder = folder->_Close(_selector, altModeIndex);

                        // Call the MenuEntry::OnAction callback if there's one
                        if (folder->_owner != nullptr && folder->_owner->OnAction != nullptr)
                            folder->_owner->OnAction(*_folder->_owner, ActionType::Closing);

                        // Switch current folder
                        if (newFolder != nullptr)
                        {
                            SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);

                            if (_freecamMode)
                                _freecamFolder = newFolder;
                            else if (_starMode)
                                _starred = newFolder;
                            else if (_altGameplayMode)
                                _gameModesFolder = newFolder;
                            else
                                _folder = newFolder;
                        }
                        else
                        {
                            // handle behavior once root folder has been reached
                            if (_noteTB.IsOpen())
                            {
                                _noteTB.Close();
                                _InfoBtn.SetState(false);
                            }
                            else if (_starMode)
                            {
                                _showStarredBtn_OnClick();
                                _showStarredBtn.SetState(false);
                            }
                            else if (_freecamMode)
                            {
                                _freecamBtn_OnClick();
                                _freecamBtn.SetState(false);
                            }
                            else if (_altGameplayMode)
                            {
                                _gameModeBtn_OnClick();
                            }
                            else
                            {
                                // signal plugin close, as user has already reached the root folder AND is not attempting to exit a submenu
                                _closedRootFolder = true;
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            // End Key::Pressed event
            default:
                break;
        }

        if (_freecamMode)
            folder = _freecamFolder;
        else if (_starMode)
            folder = _starred;
        else if (_altGameplayMode)
            folder = _gameModesFolder;

        if (_selector >= static_cast<int>(folder->ItemsCount()))
            _selector = 0;

        /*
        ** Scrolling text variables
        *********************************/
        if (folder->ItemsCount() > 0 && event.key.code != Key::Touchpad && (event.type < Event::TouchBegan || event.type > Event::TouchSwipped))
        {
            item = folder->_items[_selector];
            _selectedTextSize = Renderer::GetTextSize(item->name.c_str());
            _maxScrollOffset = static_cast<float>(_selectedTextSize) - 200.f;
            _scrollClock.Restart();
            _scrollOffset = 0.f;
            _reverseFlow = false;
        }
        else if (folder->ItemsCount() == 0)
        {
            _selectedTextSize = 0;
            _AddFavoriteBtn.SetState(false);
            _AddFavoriteBtn.Lock();
            _InfoBtn.SetState(false);
            _InfoBtn.Disable();
            _controllerBtn.SetState(false);
            _controllerBtn.Lock();
        }

        /*
        ** Update icon buttons state
        **************************/
        if (folder->ItemsCount() > 0 && _selector < static_cast<int>(folder->ItemsCount()))
        {
            item = folder->_items[_selector];

            // Update favorite icon
            _AddFavoriteBtn.SetState(item->_IsStarred());

            if (last != item)
            {
                last = item;

                // Toggle the info button
                if (item->GetNote().size() > 0)
                {
                    _noteTB.Update(item->name, item->GetNote());
                    _InfoBtn.Enable();
                    _InfoBtn.Unlock();
                }
                else
                    _InfoBtn.Disable();
            }
        }
    }

    //###########################################
    // Render Menu
    //###########################################

    void PluginMenuHome::_RenderTop(void)
    {
        const Color &maintext = Preferences::Settings.MainTextColor;

        int posY = 25;
        int posX = 40;

        // Draw background
        Window::TopWindow.Draw();

        MenuFolderImpl *folder = _folder;

        if (_freecamMode)
            folder = _freecamFolder;
        else if (_starMode)
            folder = _starred;
        else if (_altGameplayMode)
            folder = _gameModesFolder;

        // Draw Title
        int maxWidth = 360;
        int width = Renderer::DrawGameFontString(folder->name.c_str(), posX, posY, maxWidth, maintext);
        Renderer::DrawLine(posX, posY, width, maintext);
        posY += 7;

        // Draw Entry
        u32  drawSelector = SelectableEntryCount(*folder);
        int max = folder->ItemsCount();
        if (max == 0)
            return;
        int i = std::max(0, _selector - 6);
        max = std::min(max, (i + 8));

        for (; i < max; i++)
        {
            MenuItem    *item = folder->_items[i];
            ItemFlags   flags = item->Flags;
            const char  *name = item->name.c_str();
            float       offset = i == _selector ? _scrollOffset : 0.f;

            // Draw separator if needed
            if (flags.useSeparatorBefore)
            {
                if (flags.useStippledLineForBefore)
                    Renderer::DrawStippledLine(posX, posY - 1, 320, Color::Gainsboro, 1);
                else
                    Renderer::DrawLine(posX, posY - 1, 320, Color::Gainsboro, 1);
            }

            if (flags.useSeparatorSpacer)
            {
                Renderer::DrawLine(posX, posY + 10, 320, Color::Gainsboro, 1);
            }

            // Draw cursor
            if (drawSelector && i == _selector)
                Renderer::MenuSelector(posX - 5, posY - 3, 330, 20);

            // Draw entry
            if (item->_type == MenuType::Entry)
            {
                MenuEntryImpl   *entry = reinterpret_cast<MenuEntryImpl *>(item);

                // lazy... might rewrite later
                if (entry->_flags.disableIcon && !entry->_flags.isUnselectable) {
                    Renderer::DrawGameFontString(name, posX + 20, posY, 350, Color::Gainsboro, offset);
                }
                else if (entry->_flags.useControllerIcon && !entry->_flags.isUnselectable) {
                    Icon::DrawGameController(posX, posY);
                    Renderer::DrawGameFontString(name, posX + 20, posY, 350, Color::Gainsboro, offset);
                    posY += 1;
                }
                else {
                    if (entry->GameFunc != nullptr)
                        Renderer::DrawSysCheckBox(name, posX, posY, 350, Color::Gainsboro, entry->IsActivated(), offset);
                    else
                    {
                        if (entry->MenuFunc != nullptr && !entry->_flags.isUnselectable)
                            Icon::DrawSettings(posX, posY);

                        Renderer::DrawGameFontString(name, posX + 20, posY, 350, Color::Gainsboro, offset);
                        posY += 1;
                    }
                }
            }
            // Draw folder
            else
            {
                Renderer::DrawSysFolder(name, posX, posY, 350, Color::Gainsboro, offset);
            }

            // Draw separator if needed
            if (flags.useSeparatorAfter)
            {
                if (flags.useStippledLineForAfter)
                    Renderer::DrawStippledLine(posX, posY - 1, 320, Color::Gainsboro, 1);
                else
                    Renderer::DrawLine(posX, posY - 1, 320, Color::Gainsboro, 1);
            }
            posY += 4;

        }
    }

    //###########################################
    // Render Bottom Screen
    //###########################################

    void PluginMenuHome::_RenderBottom(void)
    {
        Renderer::SetTarget(BOTTOM);
        Window::BottomWindow.Draw();

        int posY = 10;
        Renderer::DrawGameFontString(("Tricord Plugin v" + _versionStr).c_str(), 95, posY, 360, Color::Gainsboro);

        Icon::DrawBG_Underline(8, 10);
        Icon::DrawInfoNewDark(116, 41);

        _showStarredBtn.Draw();
        _freecamBtn.Draw();
        _gameModeBtn.Draw();
        _faqBtn.Draw();
        _settingsBtn.Draw();
        _arBtn.Draw();
        _toolsBtn.Draw();
        _AddFavoriteBtn.Draw();
        _InfoBtn.Draw();
        _discordBtn.Draw();
        _controllerBtn.Draw();
    }

    //###########################################
    // Update menu
    //###########################################
    void PluginMenuHome::_Update(Time delta)
    {
        /*
        ** Scrolling
        *************/

        if (_selectedTextSize >= 280 && _scrollClock.HasTimePassed(Seconds(2)))
        {
            if (!_reverseFlow && _scrollOffset < _maxScrollOffset)
            {
                _scrollOffset += 29.f * delta.AsSeconds();
            }
            else
            {
                _scrollOffset -= 55.f * delta.AsSeconds();
                if (_scrollOffset <= 0.0f)
                {
                    _reverseFlow = false;
                    _scrollOffset = 0.f;
                    _scrollClock.Restart();
                }
                else
                    _reverseFlow = true;
            }
        }

        /*
        ** Buttons
        *************/

        // Buttons visibility

        MenuFolderImpl *folder = _folder;

        if (_freecamMode)
            folder = _freecamFolder;
        else if (_starMode)
            folder = _starred;
        else if (_altGameplayMode)
            folder = _gameModesFolder;

        if (folder && (*folder)[_selector])
        {
            // If current folder is empty
            if (folder->ItemsCount() == 0)
            {
                _AddFavoriteBtn.Lock();
                _InfoBtn.Disable();
                _controllerBtn.Lock();
            }
            // If selected object is a folder
            else if ((*folder)[_selector]->IsFolder())
            {
                MenuFolderImpl *e = reinterpret_cast<MenuFolderImpl *>((*folder)[_selector]);

                if (e->note.size())
                {
                    _InfoBtn.Enable();
                    _InfoBtn.Unlock();
                }
                else
                    _InfoBtn.Disable();

                _AddFavoriteBtn.SetState(e->_IsStarred());
                _controllerBtn.SetState(false);
                _controllerBtn.Lock();

                if (e->HasNoteChanged())
                {
                    _noteTB.Update(e->firstName, e->GetNote());
                    e->HandledNoteChanges();
                }
            }

            // If selected object is an entry
            else if ((*folder)[_selector]->IsEntry())
            {
                MenuEntryImpl   *e = reinterpret_cast<MenuEntryImpl *>((*folder)[_selector]);
                std::string &note = e->GetNote();

                if (e->note.size())
                {
                    _InfoBtn.Enable();
                    _InfoBtn.Unlock();
                }
                else
                    _InfoBtn.Disable();

                if (e->_owner != nullptr && e->_owner->Hotkeys.Count() > 0)
                {
                    _controllerBtn.SetState(true);
                    _controllerBtn.Unlock();
                }
                else
                {
                    _controllerBtn.SetState(false);
                    _controllerBtn.Lock();
                }

                _AddFavoriteBtn.Unlock();
                _AddFavoriteBtn.SetState(e->_IsStarred());

                if (e->HasNoteChanged())
                {
                    _noteTB.Update(e->firstName, note);
                    e->HandledNoteChanges();
                }
            }
            // An error is happening
            else
            {
                _AddFavoriteBtn.Enable(false);
                _InfoBtn.Enable(false);
                _discordBtn.Enable(false);
                _controllerBtn.Enable(false);
            }
        }
        // Buttons status
        bool isTouched = Touch::IsDown();
        IntVector touchPos(Touch::GetPosition());

        // Update buttons
        _showStarredBtn.Update(isTouched, touchPos);
        _freecamBtn.Update(isTouched, touchPos);
        _gameModeBtn.Update(isTouched, touchPos);
        _faqBtn.Update(isTouched, touchPos);
        _settingsBtn.Update(isTouched, touchPos);
        _arBtn.Update(isTouched, touchPos);
        _AddFavoriteBtn.Update(isTouched, touchPos);
        _InfoBtn.Update(isTouched, touchPos);
        _discordBtn.Update(isTouched, touchPos);
        _controllerBtn.Update(isTouched, touchPos);
        _toolsBtn.Update(isTouched, touchPos);

        Window::BottomWindow.Update(isTouched, touchPos);
    }

    void PluginMenuHome::_TriggerEntry(void)
    {
        MenuFolderImpl *folder = _folder;
        int altModeIndex = 0;

        if (_freecamMode)
        {
            folder = _freecamFolder;
            altModeIndex = 2;
        }
        else if (_starMode)
        {
            folder = _starred;
            altModeIndex = 1;
        }
        else if (_altGameplayMode)
        {
            folder = _gameModesFolder;
            altModeIndex = 3;
        }

        if (_selector >= static_cast<int>(folder->ItemsCount()))
            return;

        MenuItem* item = folder->_items[_selector];

        /*
        ** MenuEntryImpl
        **************/
        if (item->_type == MenuType::Entry)
        {
            MenuEntryImpl* entry = reinterpret_cast<MenuEntryImpl *>(item);

            if (entry->_flags.isUnselectable)
            {
                SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);
                return;
            }

            // If the entry has a valid funcpointer
            if (entry->GameFunc != nullptr)
            {
                // Change the state
                bool just = entry->_flags.justChanged;
                bool state = entry->_TriggerState();

                // If is activated add to executeLoop
                if (state)
                {
                    SoundEngine::PlayMenuSound(SoundEngine::Event::SELECT);
                    PluginMenuExecuteLoop::Add(entry);
                }
                else if (just)
                {
                    SoundEngine::PlayMenuSound(SoundEngine::Event::DESELECT);
                    PluginMenuExecuteLoop::Remove(entry);
                }
            }
            else if (entry->MenuFunc != nullptr)
            {
                SoundEngine::PlayMenuSound(SoundEngine::Event::ACCEPT);
                entry->MenuFunc(entry->_owner);
            }
        }
        /*
        ** MenuFolderImpl
        ****************/
        else
        {
            MenuFolderImpl* p = reinterpret_cast<MenuFolderImpl *>(item);

            // If a MenuFolder exists and has a callback
            if (p->_owner != nullptr && p->_owner->OnAction != nullptr)
            {
                // If the callabck tells us to not open the folder
                if (!(p->_owner->OnAction(*p->_owner, ActionType::Opening)))
                {
                    SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);
                    return;
                }
            }
            SoundEngine::PlayMenuSound(SoundEngine::Event::ACCEPT);

            p->_Open(folder, _selector, altModeIndex);

            if (_freecamMode)
                _freecamFolder = p;
            else if (_starMode)
                _starred = p;
            else if (_altGameplayMode)
                _gameModesFolder = p;
            else
                _folder = p;

            _selector = 0;
        }
    }

    void PluginMenuHome::_showStarredBtn_OnClick(void)
    {
        static int bak = 0;
        std::swap(bak, _selector);

        if (!_starMode)
        {
            _gameModeBtn.Lock();
            _freecamBtn.Lock();
        }
        else
        {
            _gameModeBtn.Unlock();
            _freecamBtn.Unlock();
        }

        _starMode = !_starMode;

        MenuFolderImpl* f = _starMode ? _starred : _folder;
        if (f->ItemsCount() == 0)
        {
            _InfoBtn.Disable();
            _AddFavoriteBtn.Lock();
            _controllerBtn.Lock();

            _selectedTextSize = 0;
        }
        else
        {
            MenuEntryImpl* e = reinterpret_cast<MenuEntryImpl *>(f->_items[_selector]);

            if (e->note.size())
            {
                _InfoBtn.Enable();
                _InfoBtn.Unlock();
            }
            else
                _InfoBtn.Disable();

            _AddFavoriteBtn.Unlock();
            _AddFavoriteBtn.SetState(e->_IsStarred());

            _selectedTextSize = Renderer::GetTextSize(e->name.c_str());
            _maxScrollOffset = static_cast<float>(_selectedTextSize) - 200.f;
            _scrollClock.Restart();
            _scrollOffset = 0.f;
            _reverseFlow = false;
        }
    }

    void PluginMenuHome::_controllerBtn_OnClick(void)
    {
        MenuFolderImpl *folder = _folder;

        if (_freecamMode)
            folder = _freecamFolder;
        else if (_starMode)
            folder = _starred;
        else if (_altGameplayMode)
            folder = _gameModesFolder;

        MenuEntryImpl* e = reinterpret_cast<MenuEntryImpl *>(folder->_items[_selector]);
        MenuEntry *entry = e->_owner;

        if (entry != nullptr)
        {
            if (entry->Hotkeys.Count() == 1)
            {
                entry->Hotkeys[0].AskForKeys();
                if (entry->Hotkeys._callback != nullptr)
                    entry->Hotkeys._callback(entry, 0);
                entry->RefreshNote();
            }
            else if (entry->Hotkeys.Count())
            {
                entry->Hotkeys.AskForKeys();
            }
        }
    }

    void PluginMenuHome::_actionReplayBtn_OnClick(void)
    {
        _mode = 4;
    }

    void PluginMenuHome::_gameModeBtn_OnClick(void)
    {
        std::string msg = _altGameplayMode ? "If you exit this menu, any active Gameplay Modes will be disabled.\n\nWould you like to continue to the Main Menu?" : "Choosing a custom Gameplay Mode will auto-disable any codes that may cause conflicts (see the Wiki for details). You will also be unable to access most of the Main Menu until you quit.\n\nWould you like to proceed?\n";

        _gameModeBtn.SetState(true); // prevent flickering

        if (MessageBox("Gameplay Modes", msg, DialogType::DialogYesNo)())
            _altGameplayMode = !_altGameplayMode;

        MenuFolderImpl *folder = _folder;

        if (_freecamMode)
            folder = _freecamFolder;
        else if (_starMode)
            folder = _starred;
        else if (_altGameplayMode)
            folder = _gameModesFolder;

        if (folder->ItemsCount() == 0)
        {
            _InfoBtn.Disable();
            _AddFavoriteBtn.Lock();
            _controllerBtn.Lock();

            _selectedTextSize = 0;
        }
        else
        {
            MenuEntryImpl *e = reinterpret_cast<MenuEntryImpl *>(folder->_items[_selector]);

            if (e->note.size())
            {
                _InfoBtn.Enable();
                _InfoBtn.Unlock();
            }
            else
                _InfoBtn.Disable();

            _AddFavoriteBtn.Unlock();
            _AddFavoriteBtn.SetState(e->_IsStarred());

            _selectedTextSize = Renderer::GetTextSize(e->name.c_str());
            _maxScrollOffset = static_cast<float>(_selectedTextSize) - 200.f;
            _scrollClock.Restart();
            _scrollOffset = 0.f;
            _reverseFlow = false;
        }

        // Freecam remains selectable here
        if (_altGameplayMode)
        {
            _toolsBtn.Lock();
            _showStarredBtn.Lock();
            _AddFavoriteBtn.Lock();
            _arBtn.Lock();
            _settingsBtn.Lock();
        }
        else
        {
            _gameModeBtn.SetState(false);

            _toolsBtn.Unlock();
            _showStarredBtn.Unlock();
            _AddFavoriteBtn.Unlock();
            _arBtn.Unlock();
            _settingsBtn.Unlock();
        }
    }

    void PluginMenuHome::_freecamBtn_OnClick(void)
    {
        _freecamMode = !_freecamMode;

        MenuFolderImpl *folder = _folder;

        if (_freecamMode)
            folder = _freecamFolder;
        else if (_starMode)
            folder = _starred;
        else if (_altGameplayMode)
            folder = _gameModesFolder;

        if (folder->ItemsCount() == 0)
        {
            _InfoBtn.Disable();
            _AddFavoriteBtn.Lock();
            _controllerBtn.Lock();

            _selectedTextSize = 0;
        }
        else
        {
            MenuEntryImpl *e = reinterpret_cast<MenuEntryImpl *>(folder->_items[_selector]);

            if (e->note.size())
            {
                _InfoBtn.Enable();
                _InfoBtn.Unlock();
            }
            else
                _InfoBtn.Disable();

            _AddFavoriteBtn.Unlock();
            _AddFavoriteBtn.SetState(e->_IsStarred());

            _selectedTextSize = Renderer::GetTextSize(e->name.c_str());
            _maxScrollOffset = static_cast<float>(_selectedTextSize) - 200.f;
            _scrollClock.Restart();
            _scrollOffset = 0.f;
            _reverseFlow = false;
        }

        // if current folder is root, that means we are either in freecam, game mode, or home
        if (_freecamMode)
        {
            _gameModeBtn.SetState(false);

            _gameModeBtn.Lock();
            _toolsBtn.Lock();
            _showStarredBtn.Lock();
            _AddFavoriteBtn.Lock();
            _arBtn.Lock();
            _settingsBtn.Lock();
        }
        else
        {
            // freecam menu closed
            if (!_altGameplayMode)
            {
                _gameModeBtn.Unlock();
                _toolsBtn.Unlock();
                _showStarredBtn.Unlock();
                _AddFavoriteBtn.Unlock();
                _arBtn.Unlock();
                _settingsBtn.Unlock();
            }
            else // should still be in root folder here
            {
                _gameModeBtn.SetState(true);
                _gameModeBtn.Unlock();
            }
        }
    }

    void PluginMenuHome::_discordBtn_OnClick(void)
    {
        _mode = 1;
    }

    void PluginMenuHome::_faqBtn_OnClick(void)
    {
        _mode = 2;
    }

    void PluginMenuHome::_settingsBtn_OnClick(void)
    {
        _mode = 3;
    }

    void PluginMenuHome::_toolsBtn_OnClick(void)
    {
        _mode = 5;
    }

    void PluginMenuHome::_InfoBtn_OnClick(void)
    {
        if (_noteTB.IsOpen())
            _noteTB.Close();
        else
            _noteTB.Open();
    }

    void PluginMenuHome::_StarItem(void)
    {
        MenuFolderImpl *folder = _folder;

        if (_freecamMode)
            folder = _freecamFolder;
        else if (_starMode)
            folder = _starred;
        else if (_altGameplayMode)
            folder = _gameModesFolder;

        if (_selector >= static_cast<int>(folder->ItemsCount()))
            return;

        MenuItem* item = folder->_items[_selector];

        if (item)
        {
            if (item->IsEntry() && reinterpret_cast<MenuEntryImpl *>(item)->IsRestricted())
                return;

            if (item->IsFolder() && reinterpret_cast<MenuFolderImpl *>(item)->IsRestricted())
                return;

            bool star = item->_TriggerStar();

            if (star)
                _starredConst->Append(item, true);
            else
                UnStar(item);
        }
    }

    void PluginMenuHome::AddToGameModesMenu(MenuItem* item)
    {
        _gameModesFolder->Append(item, true);
    }

    void PluginMenuHome::AddToFreecamMenu(MenuItem* item)
    {
        _freecamFolder->Append(item, true);
    }

    void PluginMenuHome::UnStar(MenuItem* item)
    {
        MenuFolderImpl *folder = _folder;

        if (_freecamMode)
            folder = _freecamFolder;
        else if (_starMode)
            folder = _starred;
        else if (_altGameplayMode)
            folder = _gameModesFolder;

        if (item != nullptr)
        {
            item->Flags.isStarred = false;

            int count = _starredConst->ItemsCount();

            if (count == 1)
            {
                _starredConst->_items.clear();
            }
            else
            {
                for (int i = 0; i < count; i++)
                {
                    MenuItem* it = _starredConst->_items[i];

                    if (it == item)
                    {
                        _starredConst->_items.erase(_starredConst->_items.begin() + i);
                        break;
                    }
                }
            }
            if (_selector >= static_cast<int>(folder->ItemsCount()))
                _selector = std::max((int)folder->ItemsCount() - 1, 0);
        }
    }

    void PluginMenuHome::Init(void)
    {
        MenuFolderImpl *folder = _folder;

        if (_freecamMode)
            folder = _freecamFolder;
        else if (_starMode)
            folder = _starred;
        else if (_altGameplayMode)
            folder = _gameModesFolder;

        MenuItem *item = folder->ItemsCount() != 0 ? folder->_items[0] : nullptr;

        if (folder->ItemsCount() != 0)
            _AddFavoriteBtn.Unlock();

        if (item != nullptr && !item->GetNote().empty())
            _InfoBtn.Enable();
    }

    void PluginMenuHome::AddPluginVersion(bool isDev)
    {
        char buffer[100];

        sprintf(buffer, "%s.%s.%s", TRICORD_VERSION_MAJOR, TRICORD_VERSION_MINOR, TRICORD_VERSION_REV);
        _versionStr.clear();
        _versionStr = buffer;

        if (isDev)
            _versionStr.append("    (DEV)");
    }

    void PluginMenuHome::Close(MenuFolderImpl *folder)
    {
        if (folder != _root)
        {
            if(_folder == folder)
                _folder = _folder->_Close(_selector, 0);
            if (_starred == folder)
                _starred = _starred->_Close(_selector, 1);
            if (_freecamFolder == folder)
                _freecamFolder = _freecamFolder->_Close(_selector, 2);
            if (_gameModesFolder == folder)
                _gameModesFolder = _gameModesFolder->_Close(_selector, 3);
        }
    }

    void PluginMenuHome::UpdateNote(void)
    {
        MenuFolderImpl *folder = _folder;

        if (_freecamMode)
            folder = _freecamFolder;
        else if (_starMode)
            folder = _starred;
        else if (_altGameplayMode)
            folder = _gameModesFolder;

        if (!folder || !((*folder)[_selector]))
            return;

        if ((*folder)[_selector]->IsFolder())
        {
            MenuFolderImpl* e = reinterpret_cast<MenuFolderImpl*>((*folder)[_selector]);
            if (e->HasNoteChanged())
            {
                _noteTB.Update(e->firstName, e->GetNote());
                e->HandledNoteChanges();
            }
        }
        else if ((*folder)[_selector]->IsEntry())
        {
            MenuEntryImpl* e = reinterpret_cast<MenuEntryImpl*>((*folder)[_selector]);
            if (e->HasNoteChanged())
            {
                _noteTB.Update(e->firstName, e->GetNote());
                e->HandledNoteChanges();
            }
        }
    }
}
