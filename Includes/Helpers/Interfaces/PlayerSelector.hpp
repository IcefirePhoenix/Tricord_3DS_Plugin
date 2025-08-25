#ifndef PLAYERSELECTOR_HPP
#define PLAYERSELECTOR_HPP

#include "Helpers.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Button.hpp"

namespace CTRPluginFramework
{

    /**
     * For future reference:
     * 0 => 000	(No players)
     * 1 => 001	(Player 1)
     * 2 => 010	(Player 2)
     * 3 => 011	(Player 1 + Player 2)
     * 4 => 100	(Player 3)
     * 5 => 101	(Player 1 + Player 3)
     * 6 => 110	(Player 2 + Player 3)
     * 7 => 111	(Player 1 + Player 2 + Player 3)
     */

    enum PlayerMask
    {
        PLAYER_NONE = 0,
        PLAYER1 = 1 << 0,
        PLAYER2 = 1 << 1,
        PLAYER3 = 1 << 2,
        PLAYER_INVALID = -1
    };

    class PlayerSelector
    {
    public:
        PlayerSelector(bool useMultiSelect, u8 currStatus = PLAYER_NONE, std::string title = "Player Selector", std::string label = "");
        ~PlayerSelector(void);

        int operator()(void);
    private:
        void _DrawTop(void) const;
        void _DrawBottom(void);
        void _Update(void);

        bool _multiSel;
        Button _saveBtn;
        std::vector<Button> _players;
        std::string _label;
        std::string _title;
    };
}

#endif