#ifndef CTRPLUGINFRAMEWORKIMPL_PLUGINMENUSETTINGS_HPP
#define CTRPLUGINFRAMEWORKIMPL_PLUGINMENUSETTINGS_HPP

#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFrameworkImpl/Menu/Menu.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include "CTRPluginFrameworkImpl/System.hpp"
#include "MenuEntryTools.hpp"
#include <string>
#include <vector>

namespace CTRPluginFramework
{
    class PluginMenuSettings
    {
        using EventList = std::vector<Event>;

    public:
        PluginMenuSettings(void);
        ~PluginMenuSettings() {}

        void UpdateSettings();
        void CreateMenu();

        bool operator()(EventList &eventList, Time &delta);

    private:
        void _ProcessEvent(Event &event);
        void _RenderTop(void);
        void _RenderBottom(void);
        void _Update(Time delta);

        MenuFolderImpl _settingsMenu;
        MenuFolderImpl _screenshotMenu;
        MenuFolderImpl _AR_BackupsMenu;
        MenuFolderImpl _menuEntryOpts;
        Menu _menu;
        bool _exit;
    };
}

#endif