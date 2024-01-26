#ifndef LEVELSTATUSEDITOR_HPP
#define LEVELSTATUSEDITOR_HPP

#include "CTRPluginFrameworkImpl/Graphics/Button.hpp"
#include "cheats.hpp"
#include <vector>
#include <string>

namespace CTRPluginFramework
{
    class LevelStatusEditor
    {
    public:
        LevelStatusEditor(const std::string &message, StringVector levelNames, int world, int _playMode);
        ~LevelStatusEditor(void);

        void    operator()(void);
        void    setCheckboxes(int world);
        int     getBit(int index, int rowNum);
        void    copyBitstrings(int playMode);
        u32     getBitstringAddress(int bitstringID, int playMode);
        void    writeUpdates(int bitstringID);

    private:

        void    _DrawTop(void) const;
        void    _DrawBottom(void);
        void    _Update(void);

        int                     _world;
        int                     _playMode;
        std::vector<Button>     _checkboxes;
        const std::string       _message;
        StringVector            _levelNames;
    };
}

#endif