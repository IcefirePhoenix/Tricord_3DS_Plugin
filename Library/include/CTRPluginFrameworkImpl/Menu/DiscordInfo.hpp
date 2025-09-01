#ifndef CTRPLUGINFRAMEWORKIMPL_DISCORDINFO_HPP
#define CTRPLUGINFRAMEWORKIMPL_DISCORDINFO_HPP

#include "types.h"
#include "CTRPluginFrameworkImpl/Menu/Menu.hpp"
#include "CTRPluginFrameworkImpl/System/Events.hpp"
#include "CTRPluginFramework/System/Time.hpp"

namespace CTRPluginFramework
{
    class DiscordInfo
    {
        using EventList = std::vector<Event>;
    public:
        DiscordInfo(void);
        ~DiscordInfo(void){};

        bool Draw(void);
        void Open(void);
        void Close(void);
        void DrawInviteQR(void);

        // Return true if the Close Button is pressed, else false
        bool operator()(EventList &eventList, Time &delta);

    private:
        bool _ProcessEvent(Event &event);
        void _Update(Time delta);

        void _moddingBtn_OnClick(void);
        void _matchmakingBtn_OnClick(void);

        TextBox _textBox;
        Button _moddingBtn;
        Button _matchmakingBtn;

        qrcodegen::QrCode _moddingQR;
        qrcodegen::QrCode _matchmakingQR;

        std::string _text;
        bool _isOpen;
    };
}

#endif