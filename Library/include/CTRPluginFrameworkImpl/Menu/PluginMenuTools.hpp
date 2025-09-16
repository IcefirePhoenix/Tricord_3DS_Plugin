#ifndef CTRPLUGINFRAMEWORKIMPL_PLUGINMENUTOOLS_HPP
#define CTRPLUGINFRAMEWORKIMPL_PLUGINMENUTOOLS_HPP

#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFrameworkImpl/System.hpp"
#include "CTRPluginFrameworkImpl/Menu/Menu.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"

#include "CTRPluginFrameworkImpl/Menu/HexEditor.hpp"
#include "CTRPluginFrameworkImpl/Menu/GatewayRAMDumper.hpp"

#include <vector>
#include <string>
#include "MenuEntryTools.hpp"

namespace CTRPluginFramework
{
    class PluginMenuTools
    {
        using EventList = std::vector<Event>;
    public:
        PluginMenuTools(HexEditor &hexEditor);
        void UpdateSettings();
        void CreateMenu();
        ~PluginMenuTools(){}

        // Return true if the Close Button is pressed, else false
        bool    operator()(EventList &eventList, Time &delta);
    private:

        void    _ProcessEvent(Event &event);
        void    _RenderTop(void);
        void    _RenderBottom(void);
        void    _Update(Time delta);

        MenuFolderImpl  _devTools;
        HexEditor       &_hexEditor;
        GatewayRAMDumper _gatewayRamDumper;
        Menu            _menu;
        bool            _exit;
    };
}

#endif