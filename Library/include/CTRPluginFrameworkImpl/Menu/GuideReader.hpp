#ifndef CTRPLUGINFRAMEWORKIMPL_GUIDEREADER_HPP
#define CTRPLUGINFRAMEWORKIMPL_GUIDEREADER_HPP

#include "types.h"
#include "CTRPluginFrameworkImpl/Menu/Menu.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/System/Events.hpp"
#include "CTRPluginFramework/System/Time.hpp"

#include <vector>
#include <map>


namespace CTRPluginFramework
{
    class GuideReader
    {
        using EventList = std::vector<Event>;
    public:
        GuideReader(void);
        ~GuideReader(void){};

        static void AddTextEntry(const std::string &entryName, const std::string &content);

        bool Draw(void);
        void Open(void);
        void Close(void);
        bool IsOpen(void);

        // Return true if the Close Button is pressed, else false
        bool operator()(EventList &eventList, Time &delta);

    private:
        bool _ProcessEvent(Event &event);
        void _Update(Time delta);

        bool _isOpen;
        Menu _menu;
        TextBox _guideTB;
        std::string _text;
        MenuEntryImpl *_last;
    };
}

#endif