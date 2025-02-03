#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // Disables the fog filter that blocks the screen when the camera is zoomed out
    void Rendering::disableFog(MenuEntry *entry)
    {
        /**
         * TODO: once enough info is known, change to proper labels for addrs + vals
         * TODO: confirm if fog resets itself during loading zones?
         */

        Process::Write32((AddressList::getAddress("FogA")), 0xFF700FFF);
        Process::Write32((AddressList::getAddress("FogB")), 0x453B8000);
        Process::Write32((AddressList::getAddress("FogC")), 0x459C4000);
    }
}