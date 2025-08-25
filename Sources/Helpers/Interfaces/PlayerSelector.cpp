#include "Helpers.hpp"

namespace CTRPluginFramework
{
    u8 selectedPlayers = PLAYER_NONE;
    int xOffset = 90; // button width + spacer => 75 + 15 = 90
    int posY = 115;

    PlayerSelector::PlayerSelector(bool useMultiSelect, u8 currStatus, std::string title, std::string label) :
        _multiSel(useMultiSelect),
        _saveBtn(Button::GameFont, "Save changes", IntRect(190, 200, 120, 32), Icon::DrawMenuButton),
        _label(label),
        _title(title)
    {
        int posX = 33;
        std::string spacer = "        ";

        if (_multiSel)
            selectedPlayers = currStatus;
        else
            selectedPlayers = PLAYER_NONE;

        for (int player = 0; player < 3; player++)
        {
            _players.push_back(Button(Button::GameFont | Button::Toggle | Button::Icon, Utils::Format("%sP%s", spacer.c_str(), std::to_string(player + 1).c_str()), IntRect(posX, posY, 75, 40)));
            _players[player].SetState(selectedPlayers & (1 << player));
            posX += xOffset;
        }
    }

    PlayerSelector::~PlayerSelector()
    {
    }

    int PlayerSelector::operator()(void)
    {
        bool mustclose = false;
        bool sleepClose = false;

        while (((!Window::BottomWindow.MustClose() && !mustclose)) && !sleepClose)
        {
            Controller::Update();
            mustclose = Controller::IsKeyPressed(Key::B);
            sleepClose = SystemImpl::IsSleeping();

            if (mustclose)
            {
                if (!_multiSel && selectedPlayers == PLAYER_NONE)
                    return PLAYER_INVALID; // single select, premature exit; this case MUST be checked by caller!
                else
                    return selectedPlayers; // multi-select
            }

            for (int currPlayer = 0; currPlayer < _players.size(); currPlayer++)
            {
                if (_players[currPlayer]())
                {
                    if (_multiSel)
                        selectedPlayers ^= static_cast<PlayerMask>(1 << currPlayer);
                    else
                    {
                        selectedPlayers = static_cast<PlayerMask>(1 << currPlayer);
                        mustclose = true;
                    }
                }

            }

            if (_saveBtn())
                mustclose = true;

            _DrawTop();
            _DrawBottom();

            Renderer::EndFrame();
            _Update();
        }

        return selectedPlayers; // somewhat redundant, but just in case...
    }

    void PlayerSelector::_DrawTop(void) const
    {
        int posY = 61;
        std::string intro = _multiSel ? Utils::Format("Use the toggles below to change the %s status for each player.\n\n", _label.c_str()) : "Select a player.\n\n";

        Renderer::SetTarget(TOP);
        Window::TopWindow.Draw();

        if (_multiSel)
        {
            std::string label = _label;

            for (int currPlayer = 0; currPlayer < _players.size(); currPlayer++)
            {
                label[0] = std::toupper((unsigned char)label[0]);

                intro += Utils::Format("Player %d: ", currPlayer + 1);
                intro += _players[currPlayer].GetState() ? Utils::Format("%s enabled\n", label.c_str()) : Utils::Format("%s not enabled\n", label.c_str());
            }
        }

        TextBox(_title.c_str(), intro.c_str(), IntRect(30, 20, 340, 200)).Draw();
    }

    void PlayerSelector::_DrawBottom(void)
    {
        Renderer::SetTarget(BOTTOM);
        Window::BottomWindow.Draw();

        int posX = 40;
        int posY_Icon = posY + 3;
        int posY_Switch = posY - 20;

        for (int currPlayer = 0; currPlayer < _players.size(); currPlayer++)
        {
            posY_Switch = posY - 20;

            _players[currPlayer].Draw();
            Icon::DrawLinkFace(posX, posY_Icon, currPlayer);

            if (_multiSel)
            {
                _saveBtn.Draw();

                if (_players[currPlayer].GetState())
                    Renderer::DrawGameFontString(ENABLED_SLIDER, posX + 18, posY_Switch, 320, Color::LimeGreen);
                else
                    Renderer::DrawGameFontString(DISABLED_SLIDER, posX + 18, posY_Switch, 320, Color::Red);
            }

            posX += xOffset;
        }
    }

    void PlayerSelector::_Update(void)
    {
        bool isTouched = Touch::IsDown();
        IntVector touchPos(Touch::GetPosition());

        for (auto iterator = _players.begin(); iterator != _players.end(); iterator++)
            (*iterator).Update(isTouched, touchPos);

        _saveBtn.Update(isTouched, touchPos);

        Window::BottomWindow.Update(isTouched, touchPos);
    }
}
