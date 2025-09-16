#ifndef CTRPLUGINFRAMEWORKIMPL_PLUGINMENUHOME_HPP
#define CTRPLUGINFRAMEWORKIMPL_PLUGINMENUHOME_HPP

#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFrameworkImpl/System.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include "CTRPluginFramework/Menu/PluginMenu.hpp"

#include <cstdio>
#include <vector>

namespace CTRPluginFramework
{
    class PluginMenuHome
    {
        using EventList = std::vector<Event>;
    public:
        PluginMenuHome(std::string &name);
        ~PluginMenuHome(){}

        // Return true if the Close Button is pressed, else false
        bool operator()(EventList &eventList, int &mode, Time &delta);
        void Append(MenuItem *item) const;
        void AddToHidden(MenuItem *item) const;
        void Refresh(void);
        void UnStar(MenuItem* item);
        void Init(void);

        void AddPluginVersion(void);
        void Close(MenuFolderImpl *folder);

        void UpdateNote(void);

    private:
        friend class PluginMenuImpl;
        friend std::string &PluginMenu::Title(void);

        void _ProcessEvent(Event &event);
        void _RenderTop(void);
        void _RenderBottom(void);
        void _Update(Time delta);

        void _StarItem(void);
        void _TriggerEntry(void);

        void _showStarredBtn_OnClick(void);
        void _controllerBtn_OnClick(void);
        void _discordBtn_OnClick(void);

        void _freecamBtn_OnClick(void);
        void _gameModeBtn_OnClick(void);

        void _actionReplayBtn_OnClick(void); // 4
        void _faqBtn_OnClick(void); // 2
        void _settingsBtn_OnClick(void); // 3
        void _toolsBtn_OnClick(void);  // 5
        void _InfoBtn_OnClick(void); // note

        // Members
        MenuFolderImpl* _root;
        MenuFolderImpl* _hidden;
        MenuFolderImpl* _folder;
        MenuFolderImpl* _returnFolder;
        MenuFolderImpl* _starred;
        MenuFolderImpl* _starredConst;

        int _mode;
        bool _starMode;

        int _selector;
        int _selectedTextSize;
        float _maxScrollOffset;
        float _scrollOffset;
        Clock _scrollClock;
        bool _reverseFlow;
        bool _closedRootFolder;
        int _versionPosX;
        std::string _versionStr;

        TextBox _noteTB;

        // Mode buttons
        Button _showStarredBtn;
        Button _freecamBtn;
        Button _arBtn;
        Button _gameModeBtn;
        Button _settingsBtn;
        Button _toolsBtn;
        Button _faqBtn;

        // Toggle buttons
        Button _discordBtn;
        Button _controllerBtn;
        Button _AddFavoriteBtn;
        Button _InfoBtn;
    };
}

#endif
