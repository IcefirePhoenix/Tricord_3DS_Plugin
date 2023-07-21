#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
    void buttonSpammer(MenuEntry* entry) {
        // TODO: replicate current ABXY spammer
        // code and verify that it works as intended (as in,
        // execution isn't too fast and spamming does indeed work...
        // also check online use)
    }


    void instantText(MenuEntry* entry) {
        // TODO: simple 8-bit write to text speed address
    }


    void beamCooldown(MenuEntry* entry) {
        // TODO: simple 32-bit write to beam address -> 0x0000001E
        // and do the same for blue and red link
    }
}