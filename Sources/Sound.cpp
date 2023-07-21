#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
    void bgmSet(MenuEntry* entry) {
        // TODO: 
        // 1) figure out BGM edits
        // 2) open up combo box with BGM types
        // 3) set edits
        // 4) profit??? LOL
    }

    void bgmVolSet(MenuEntry* entry) {
        // TODO: 
        // 1) open up numerical keyboard and get user input (do we set limits here)
        // 2) simple memory write to BGM volume address

        // Note: we need to convert from decimal to hex here
    }

    void lobbyBallSong(MenuEntry* entry) {
        // TODO: open up combo box with song list, and set user input to lobby ball song address
    }

    void voiceVol(MenuEntry* entry) {
        // TODO: 
        // 1) open up numerical keyboard and get user input (do we set limits here)
        // 2) simple memory write to Voice volume address

        // Note: we need to convert from decimal to hex here
    }
    
    void lowHPVol(MenuEntry* entry) {
        // TODO: 
        // 1) open up numerical keyboard and get user input (do we set limits here)
        // 2) simple memory write to HP SFX volume address 

        // Note: do we even have this address -- does it exist or do we default to a generic SFX address...?
        // Note: we need to convert from decimal to hex here
    }
}