#include "CTRPluginFrameworkImpl/Menu/DiscordInfo.hpp"

namespace CTRPluginFramework
{
    bool showModding = true;
    std::string currInvite = MODDING_INVITE;

    std::string moddingTitle = "TFH Modding Server";
    std::string MM_Title = "TFH Matchmaking Server";

    std::string moddingIntro = "This is the home of all things related to TFH modding! Here you can connect with the Tricord developers, explore new mods from GameBanana, find helpful guides, and chat with the modding community!";
    std::string MM_Intro = "This is the main TFH playerbase's community! Here you can find players to play with via online multiplayer on the Pretendo Network, hang out with other TFH/Zelda fans, and partake in occasional events hosted by the staff!";

    DiscordInfo::DiscordInfo(void):
        _textBox("", "", Window::TopWindow.GetRect()),
        _moddingBtn(Button::GameFont | Button::Toggle, "Modding", IntRect(180, 85, 120, 32), Icon::DrawMenuButton),
        _matchmakingBtn(Button::GameFont | Button::Toggle, "Matchmaking", IntRect(180, 125, 120, 32), Icon::DrawMenuButton)
    {
        _moddingBtn.SetState(true);
        _isOpen = false;
    }

    bool DiscordInfo::operator()(EventList &eventList, Time &delta)
    {
        std::string title = showModding ? moddingTitle : MM_Title;

        currInvite = showModding ? MODDING_INVITE : MATCHMAKING_INVITE;

        _text = showModding ? moddingIntro : MM_Intro;
        _text += "\n\nScan the QR Code on the bottom screen to easily get the invite link.";
        _text += Utils::Format("\n\nPrefer typing? Use: discord.gg/%s", currInvite.c_str());

        _isOpen = true;

        for (size_t i = 0; i < eventList.size(); i++)
            _ProcessEvent(eventList[i]);

        if (_moddingBtn())
            _moddingBtn_OnClick();
        else if (_matchmakingBtn())
            _matchmakingBtn_OnClick();

        _Update(delta);

        _textBox.Update(Utils::Format("Discord: %s", title.c_str()), _text);
        _textBox.Open();

        Draw();
        return (Window::BottomWindow.MustClose() || !_isOpen);
    }

    void DiscordInfo::_moddingBtn_OnClick(void)
    {
        showModding = true;
        _matchmakingBtn.SetState(false);
        _moddingBtn.SetState(true);
    }

    void DiscordInfo::_matchmakingBtn_OnClick(void)
    {
        showModding = false;
        _moddingBtn.SetState(false);
        _matchmakingBtn.SetState(true);

    }

    bool DiscordInfo::_ProcessEvent(Event &event)
    {
        bool shouldClose = event.key.code == Key::B;

        if (!_isOpen || shouldClose)
            return false;

        if (_textBox.IsOpen())
            _textBox.ProcessEvent(event);

        return true;
    }

    void drawInviteQR(void)
    {
        std::string text = "discord.com/invite/" + currInvite;
        std::vector<qrcodegen::QrSegment> indivSegments = qrcodegen::QrSegment::makeSegments(text.c_str());

        // note: encodeText() auto-chooses a QR version to best fit the link's size, but because the links are different lengths, the QR dimensions change... solution is to instead force version 6, with auto-masking via encodeSegments()
        qrcodegen::QrCode qrcode = qrcodegen::QrCode::encodeSegments(indivSegments, qrcodegen::QrCode::Ecc::MEDIUM, 3, 3, -1, true);

        int currXPos = 37, currYPos = 52;
        int rightBoundary = 190, bottomBoundary = 190;
        int qrSideLen = qrcode.getSize();
        int pixelsPerModule = (bottomBoundary - currYPos) / qrSideLen;
        int columnsDrawn = 0, modulesDrawn = 0;

        // draw background...
        const Screen &bottomScreen = OSD::GetBottomScreen();
        bottomScreen.DrawRect(30, 45, 130, 130, Color::White);

        Renderer::SetTarget(BOTTOM);
        while (currXPos < rightBoundary && columnsDrawn < qrSideLen)
        {
            modulesDrawn = 0;
            currYPos = 52;

            while (currYPos < bottomBoundary && modulesDrawn < qrSideLen)
            {
                bottomScreen.DrawRect(currXPos, currYPos, pixelsPerModule, pixelsPerModule, qrcode.getModule(columnsDrawn, modulesDrawn) ? Color::Black : Color::White);
                currYPos += pixelsPerModule;
                modulesDrawn++;
            }
            columnsDrawn++;
            currXPos += pixelsPerModule;
        }
    }

    bool DiscordInfo::Draw(void)
    {
        if (!_isOpen)
            return (false);

        int posY = 58;

        Window::BottomWindow.Draw();

        if (_textBox.IsOpen())
        {
            Renderer::SetTarget(TOP);
            _textBox.Draw();

            Renderer::SetTarget(BOTTOM);
            Renderer::DrawGameFontString("Discord Server:", 180, posY, 320, Color::Gainsboro);
            Renderer::DrawLine(182, posY, 103, Color::Gainsboro);

            drawInviteQR();

            _moddingBtn.Draw();
            _matchmakingBtn.Draw();
        }

        return true;
    }

    void DiscordInfo::_Update(Time delta)
    {
        bool isTouched = Touch::IsDown();
        IntVector touchPos(Touch::GetPosition());

        _moddingBtn.Update(isTouched, touchPos);
        _matchmakingBtn.Update(isTouched, touchPos);

        Window::BottomWindow.Update(isTouched, touchPos);
    }


    void    DiscordInfo::Open(void)
    {
        _isOpen = true;
    }

    void    DiscordInfo::Close(void)
    {
        _isOpen = false;
    }
}