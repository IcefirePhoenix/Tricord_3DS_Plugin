#include <fstream>
#include "CTRPluginFrameworkImpl/Menu/PluginMenuActionReplay.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuExecuteLoop.hpp"
#include "CTRPluginFrameworkImpl/ActionReplay/ARCode.hpp"
#include "CTRPluginFrameworkImpl/ActionReplay/MenuEntryActionReplay.hpp"
#include "CTRPluginFramework/Utils.hpp"
#include "CTRPluginFramework/Menu/Keyboard.hpp"
#include "CTRPluginFrameworkImpl/ActionReplay/ARCodeEditor.hpp"
#include "CTRPluginFramework/Menu/MessageBox.hpp"
#include "CTRPluginFrameworkImpl.hpp"
#include "CTRPluginFramework.hpp"

namespace CTRPluginFramework
{
    static inline const char *FindFolder(const char *path)
    {
        while (path && *path && (*path != '\\' && *path != '/'))
            ++path;

        if (!path || !*path)
            return nullptr;

        return path;
    }

    static void     ProcessPathString(u8 *out)
    {
        std::string &path = Preferences::CheatsFile;
        const char  *cpath = path.c_str();
        const u32   size = Utils::GetSize(path);

        if (size < 36)
        {
            *out++ = '.';
            if (*cpath != '/')
                *out++ = '/';
            while (*cpath)
                *out++ = *cpath++;

            *out = 0;
            return;
        }

        // Try to cut in folders
        u32 ssize = size;
        const char *cut = cpath;

        while (ssize >= 35)
        {
            // Skip the first '/' character
            ++cut;

            cut = FindFolder(cut);

            if (cut == nullptr)
                break;

            ssize = Utils::GetSize(cut);
        }

        // If we have a cut path which fits
        if (cut != nullptr)
        {
            *out++ = '.';
            *out++ = '/';
            *out++ = '.';
            *out++ = '.';
            while (*cut)
                *out++ = *cut++;
            *out = 0;
            return;
        }

        // Else, just display the end of the filename
        cut = cpath + path.size() - 2 - 34;

        while (*cut)
            *out++ = *cut++;
        *out = 0;
    }

    static PluginMenuActionReplay *__pmARinstance = nullptr;
    PluginMenuActionReplay::PluginMenuActionReplay() :
        _topMenu{ "Action Replay Codes" },
        _noteBtn(Button::Icon | Button::Toggle, IntRect(40, 125, 110, 28), Icon::DrawInfo),
        _editorBtn(Button::Icon, IntRect(40, 90, 110, 28), Icon::DrawEdit),
        _newBtn(Button::Icon, IntRect(170, 90, 110, 28), Icon::DrawPlus),
        _cutBtn(Button::Icon, IntRect(170, 162, 110, 28), Icon::DrawCut),
        _pasteBtn(Button::Icon, IntRect(170, 162, 110, 28), Icon::DrawClipboard),
        _duplicateBtn(Button::Icon, IntRect(170, 127, 110, 28), Icon::DrawDuplicate),
        _trashBtn(Button::Icon, IntRect(40, 160, 110, 28), Icon::DrawTrash),
        _openFileBtn(0, "Open", IntRect(30, 200, 34, 15)),

        _clipboard{ nullptr },
        _path{0}
    {
        __pmARinstance = this;
        ProcessPathString(_path);

        // Disable buttons
        _editorBtn.Disable();
        _cutBtn.Disable();
        _pasteBtn.Disable();
        _duplicateBtn.Disable();
        _trashBtn.Disable();
    }

    PluginMenuActionReplay::~PluginMenuActionReplay()
    {
        if (_clipboard)
            delete _clipboard;
    }

    void    PluginMenuActionReplay::Initialize(void)
    {
        Task    task([](void *arg)
        {
            ActionReplay_LoadCodes(__pmARinstance->_topMenu.GetFolder());
            return (s32)0;
        });

        // Async job
        task.Start();
    }

    bool    PluginMenuActionReplay::operator()(EventList &eventList, const Time &delta)
    {
        static Task     top([](void *arg)
        {
            static_cast<PluginMenuActionReplay *>(arg)->_topMenu.Draw();

            return static_cast<s32>(0);
        }, this, Task::AppCores);

        // Process events
        _ProcessEvent(eventList);

        // Update components
        _Update(delta);

        // Check if note state must be changed
        if (_noteBtn())
        {
            if (!_topMenu.IsNoteOpen())
            {
                if (!_topMenu.ShowNote())
                    _noteBtn.Disable();
            }
            else
                _topMenu.CloseNote();
        }

        // Check buttons
        if (_editorBtn()) _EditorBtn_OnClick();
        if (_newBtn()) _NewBtn_OnClick();
        if (_cutBtn()) _CutBtn_OnClick();
        if (_pasteBtn()) _PasteBtn_OnClick();
        if (_duplicateBtn()) _DuplicateBtn_OnClick();
        if (_trashBtn()) _TrashBtn_OnClick();
        if (_openFileBtn()) _OpenFileBtn_OnClick();

        // Draw menu on top screen
        top.Start();

        // Draw bottom screen
        _DrawBottom();

        // Wait until rendering on Top Screen is finished
        top.Wait();

        // Return if user want to close the window
        return (Window::BottomWindow.MustClose());
    }

    void    PluginMenuActionReplay::_DrawBottom(void)
    {
        Renderer::SetTarget(BOTTOM);
        Window::BottomWindow.Draw();

        Renderer::DrawRect(IntRect(30, 85, 259, 110), Color::Magenta, true);

        _cutBtn.Draw();
        _pasteBtn.Draw();
        _openFileBtn.Draw();

        int posX = 30 + 34 + 5;
        int posY = 200;

        // y positions are not consistent?
        int yCoordA = 95;
        int yCoordB = 132;
        int cutPasteY = 168;

        Renderer::DrawRect(posX, posY, 220, 15, Color::Maroon);
        posY += 3;
        Renderer::DrawString((const char *)_path, posX + 2, posY, Color::Gainsboro);
        
        const char* labels[6] =
        {
            "Create New", "Duplicate", "Open Editor", "", "Delete", "Read Note"
        };

        Renderer::DrawSysString(labels[0], 205, yCoordA, 290, Preferences::Settings.MainTextColor);
        _newBtn.Draw();

        if (_topMenu.GetSelectedItem())
        {
            if (_clipboard == nullptr) {
                _newBtn.Draw();
                _noteBtn.Draw();
                _editorBtn.Draw();
                _duplicateBtn.Draw();
                _trashBtn.Draw();

                Renderer::DrawSysString(labels[1], 205, yCoordB, 290, Preferences::Settings.MainTextColor);
            
                if (!_topMenu.GetSelectedItem()->note.empty())
                    Renderer::DrawSysString(labels[5], 75, yCoordB -= 17, 290, Preferences::Settings.MainTextColor);

                for (int i = 2, yCoord2 = 95; i < 5; ++i, yCoord2 += 28)
                    Renderer::DrawSysString(labels[i], 75, yCoord2, 290, Preferences::Settings.MainTextColor);
            }

            // cut/paste label
            const char* swapLabel = (_clipboard != nullptr) ? "Paste" : "Cut";
            Renderer::DrawSysString(swapLabel, 205, cutPasteY, 290, Preferences::Settings.MainTextColor);
        }

    }

    void    PluginMenuActionReplay::_ProcessEvent(EventList &eventList)
    {
        for (Event &event : eventList)
        {
            // Process top menu's event
            MenuItem *item = nullptr;
            int action = _topMenu.ProcessEvent(event, &item);

            // If user want to exit the current menu
            if (action == MenuEvent::MenuClose)
                Window::BottomWindow.Close();
        }
    }

    void    PluginMenuActionReplay::_Update(const Time &delta)
    {
        _topMenu.Update(delta);
        Window::BottomWindow.Update(Touch::IsDown(), IntVector(Touch::GetPosition()));

        bool        touchIsDown = Touch::IsDown();
        IntVector   touchPos(Touch::GetPosition());
        MenuItem    *item = _topMenu.GetSelectedItem();

        _noteBtn.Enable(item && !item->note.empty());
        _noteBtn.SetState(_topMenu.IsNoteOpen());
        _noteBtn.Update(touchIsDown, touchPos);

        _editorBtn.Enable(item != nullptr);
        _editorBtn.Update(touchIsDown, touchPos);

        _cutBtn.Enable(item != nullptr && _clipboard == nullptr);
        _pasteBtn.Enable(_clipboard != nullptr);
        _duplicateBtn.Enable(item != nullptr && !item->IsFolder());
        _trashBtn.Enable(item != nullptr);

        _newBtn.Update(touchIsDown, touchPos);
        _cutBtn.Update(touchIsDown, touchPos);
        _pasteBtn.Update(touchIsDown, touchPos);
        _duplicateBtn.Update(touchIsDown, touchPos);
        _trashBtn.Update(touchIsDown, touchPos);
        _openFileBtn.Update(touchIsDown, touchPos);
    }

    static bool ActionReplay_GetInput(std::string &ret)
    {
        Keyboard    keyboard;

        /*keyboard.SetCompareCallback([](const void *in, std::string &error)
        {
            std::string &input = *(std::string *)(in);
            if (input.empty())
                return false;
            ActionReplay_ProcessString(input, false);
            return true;
        });*/

        return keyboard.Open(ret, ret) != -1;
    }

    void    PluginMenuActionReplay::_EditorBtn_OnClick(void)
    {
        MenuItem    *item = _topMenu.GetSelectedItem();
        Keyboard   optKbd;
        std::vector<std::string>    options = { "Name", "Note" };

        if (!item)
            return;
        if (!item->IsFolder())
            options.push_back("Code");

        optKbd.Populate(options);
        int choice = optKbd.Open();
        if (choice >= 0)
        {
            // Name edition
            if (choice == 0)
            {
                ActionReplay_GetInput(item->name);
            }
            // Note edition
            else if (choice == 1)
            {
                ActionReplay_GetInput(item->note);
                ActionReplay_ProcessString(item->note);
            }
            // Code edition
            else if (choice == 2)
            {
                MenuEntryActionReplay *e = reinterpret_cast<MenuEntryActionReplay *>(item);

                // Disable the hard way :p
                PluginMenuExecuteLoop::RemoveAR(e);

                // Edit code
                ARCodeEditor::Edit(e->context);
                e->context.Update();
            }
        }
    }

    void    PluginMenuActionReplay::_NewBtn_OnClick(void)
    {
        Keyboard        kbd("", {"Code", "Folder"});
        MenuFolderImpl  *f = __pmARinstance->_topMenu.GetRootFolder();

        if (f == nullptr)
            return;

        int choice = kbd.Open();

        if (choice == -1)
            return;

        std::string name;

        if (!ActionReplay_GetInput(name))
            return;

        // Create a new code
        if (choice == 0)
        {
            MenuEntryActionReplay *entry = new MenuEntryActionReplay(name);
            f->Append(entry);
        }

        // Create a new folder
        if (choice == 1)
        {
            MenuFolderImpl *folder = new MenuFolderImpl(name);
            f->Append(folder);
        }
    }

    void    PluginMenuActionReplay::_CutBtn_OnClick(void)
    {
        // If clipboard already exists, abort
        if (_clipboard)
            return;
        _clipboard = _topMenu.Pop();
    }

    void    PluginMenuActionReplay::_PasteBtn_OnClick(void)
    {
        if (!_clipboard)
            return;
        _topMenu.Insert(_clipboard);
        _clipboard = nullptr;
    }

    void    PluginMenuActionReplay::_DuplicateBtn_OnClick(void)
    {
        MenuItem *current = _topMenu.GetSelectedItem();

        if (!current || current->IsFolder())
            return;

        MenuEntryActionReplay *old = reinterpret_cast<MenuEntryActionReplay *>(current);
        MenuEntryActionReplay *dup = new MenuEntryActionReplay(current->name, current->note);

        dup->context = old->context;
        _topMenu.Insert(dup);
    }

    void    PluginMenuActionReplay::_TrashBtn_OnClick(void)
    {
        MenuItem *item = _topMenu.GetSelectedItem();

        if (item == nullptr)
            return;

        if (!(MessageBox(Color::Orange << "Warning", "Do you really want to delete: " << item->name, DialogType::DialogYesNo )()))
            return;

        item = _topMenu.Pop();

        delete item;
    }

    void    PluginMenuActionReplay::_OpenFileBtn_OnClick(void)
    {
        if (!__pmARinstance)
            return;

        // Backup current codes
        if (MessageBox(Color::Orange << "Warning", "Do you want to save all changes to current file?", DialogType::DialogYesNo)())
            SaveCodes();

        std::string newPath;

        if (Utils::FilePicker(newPath, ".txt") == -1)
            return;

        // Set path
        Preferences::CheatsFile = newPath;

        MenuFolderImpl *root = __pmARinstance->_topMenu.GetRootFolder();

        if (root->ItemsCount() > 0)
        {
            if (MessageBox(Color::Orange << "Warning", "Do you want to clear the current code list?", DialogType::DialogYesNo)())
            {
                // Ensure we're at the root of the menu
                __pmARinstance->_topMenu.CloseAll();

                // Delete all codes
                root->Clear();
            }
        }

        // Open file and load codes
        ActionReplay_LoadCodes(root);

        // Update path
        ProcessPathString(_path);
    }

    void    PluginMenuActionReplay::SaveCodes(void)
    {
        if (!__pmARinstance)
            return;

        MenuFolderImpl *folder = __pmARinstance->_topMenu.GetRootFolder();

        // Open a temporary file
        // note: every time AR.temp is updated, it is also cleared before the write because of the TRUNCATE flag
        File        file("AR.temp", File::RWC | File::TRUNCATE);
        LineWriter  writer(file);

        if (!file.IsOpen())
            return;

        // Write all codes to the temporary file
        MenuFolderImpl &f = *folder;
        for (u32 i = 0; i < f.ItemsCount(); i++)
            ActionReplay_WriteToFile(writer, f[i]);

        writer.Close();

        // refresh the AR menu with the current data (aka what was just written to AR.temp)
        ActionReplay_OpenCheatsFile(file, true);

        std::string path = file.GetFullName(); // by default this is cheats/titleID.txt
        file.Close();

        // delete the current cheat file and rename the temporary file
        File::Remove(path);
        File::Rename("AR.temp", path); 
    }

    // TODO: decide whether backup folder should be placed under default "cheats" folder or under "Tricord"
    void    PluginMenuActionReplay::BackupCodes(bool manualMode)
    {
        std::string titleID;
        Process::GetTitleID(titleID);

        std::string sourcePath = Preferences::CheatsFile;
        std::string backupPath = manualMode ? "/cheats/TricordBackup/manual/" : "/cheats/TricordBackup/auto/";
        std::string backupFileName = "";
        std::string dateStr = Time::GetDate();

        if (!Directory::IsExists(backupPath))
            Directory::Create(backupPath);

        if (!manualMode)
        {
            // backup files have their "last updated" date appended to their filename, like so: titleID-MM.DD.YY.txt
            // since the filenames are dynamic, locating backup files relies on titleID
            Directory folder;
            Directory::Open(folder, backupPath);

            std::vector<std::string> files;
            folder.ListFiles(files, ".txt");

            // search for backup file via matching titleID substring
            for (const auto& fileLocator : files) {
                if (fileLocator.find(titleID) != std::string::npos) 
                {
                    backupFileName = fileLocator;
                    break; 
                }
            }

            if (!backupFileName.empty()) // backup file found
                backupPath.append(backupFileName);
            else 
                backupPath.append(titleID + "-" + dateStr + ".txt"); // no backup found; create new 
        }
        else 
        {
            backupPath.append(titleID + "/");

            if (!Directory::IsExists(backupPath))
                Directory::Create(backupPath);
                
            // have user choose custom name when creating manual backups from settings menu
            Keyboard setName("Name your backup file.");
            setName.DisplayTopScreen = true;
            setName.CanAbort(true);

            if (setName.Open(backupFileName) == 0)
                backupPath.append(backupFileName + ".txt"); 
            else
                return;
        }
        
        if (!File::Exists(backupPath)) 
            File::Create(backupPath);
    
        // prep read processes
        File sourceFile(sourcePath, File::READ);
        LineReader reader(sourceFile);

        if (sourceFile.GetSize() > 0)
        {
            File backupFile(backupPath, File::WRITE | File::TRUNCATE); // will clear the file upon open
            LineWriter writer(backupFile);

            if (!sourceFile.IsOpen() || !backupFile.IsOpen() || !File::Exists(sourcePath)) 
            {
                MessageBox msg("Error", "Backup of Action Replay codes failed.", DialogType::DialogOk);
                msg();                
                return;
            }

            std::string nextLine;
            while (reader(nextLine)) 
                writer << nextLine << writer.endl();
        
            // end LineWriter process
            writer.Close();

            // close files
            sourceFile.Close();
            backupFile.Close();

            if (manualMode)
            {
                MessageBox msg("Success", "Action Replay cheats have been successfully backed up.", DialogType::DialogOk);
                msg();
            }
            else 
                // rename the backup file with date
                File::Rename(backupPath, "/cheats/TricordBackup/auto/" + titleID + "-" + dateStr + ".txt");
        }
        else
        {
            MessageBox restoreNotif("The Action Replay cheat file is empty. Would you like to restore from a backup?", DialogType::DialogYesNo);
            if (restoreNotif())
                RestoreFromBackup(true, Time::ParseDate(dateStr));
        }
    }

    void    PluginMenuActionReplay::RestoreFromBackup(bool fromAuto, std::string autoDate)
    {
        std::string currCheatFilePath = Preferences::CheatsFile;

        MessageBox  msgBox("This operation will overwrite your current Action Replay code list. Are you sure you want to proceed?", DialogType::DialogYesNo);
        bool fromManual;

        if (!fromAuto)
            fromManual = msgBox();

        if (fromManual || fromAuto)
        {
            std::string chosenFilePath;
            if (Utils::FilePicker(chosenFilePath, ".txt", autoDate) == 0)
                Preferences::CheatsFile = chosenFilePath;

            MenuFolderImpl *root = __pmARinstance->_topMenu.GetRootFolder();
            if (root->ItemsCount() > 0)
            {
                __pmARinstance->_topMenu.CloseAll();
                root->Clear();
            }

            ActionReplay_LoadCodes(root); 
            Preferences::CheatsFile = currCheatFilePath;

            MessageBox errBox("Success", "Action Replay cheats have been restored from backup.", DialogType::DialogOk);
            errBox();
        }
    }

    void    PluginMenuActionReplay::NewARCode(u8 type, u32 address, u32 value)
    {
        if (!__pmARinstance)
            return;

        std::string name;

        if (!ActionReplay_GetInput(name))
            return;

        MenuEntryActionReplay *ar = new MenuEntryActionReplay(name);

        u32 offset = address & 0xFF000000;
        address &= 0xFFFFFF;
        ar->context.codes.push_back(ARCode(0xD3, 0, offset));
        ar->context.codes.push_back(ARCode(type, address, value));
        ar->context.codes.push_back(ARCode(0xD2, 0, 0));

        ar->context.Update();
        MenuFolderImpl *f = __pmARinstance->_topMenu.GetRootFolder();

        if (f)
            f->Append(ar);
        ARCodeEditor::Edit(ar->context);
        ar->context.Update();
    }
}
