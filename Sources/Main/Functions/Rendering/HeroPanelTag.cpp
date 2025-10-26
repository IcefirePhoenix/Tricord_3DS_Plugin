#include "Helpers.hpp"
#include "Cheats.hpp"

constexpr u32 devID1 = DEV_ID1;
constexpr u32 devID2 = DEV_ID2;
constexpr u32 devID3 = DEV_ID3;

namespace CTRPluginFramework
{
    MenuEntry *starMarkManager = new MenuEntry("", "");
    MenuEntry *starMarkInit = new MenuEntry("", "");

    int heroPanelOrder[3][3] = {{1, 0, 2}, {0, 1, 2}, {0, 2, 1}};
    u32 heroPanelIconPtrs[3] = {0};

    /* ------------------ */

#ifdef DEV_BUILD
    constexpr u32 devID_Active = DEV_ID_ACTIVE;
    bool devSelf = true;

    void Rendering::setDevID_onConnection(MenuEntry *entry)
    {
        for (u32 addr : ID_ADDRESSES)
            Process::Write32(addr, devID_Active);
    }
#else
    bool devSelf = false;
#endif

    bool verifyDev(u32 ID)
    {
        switch (ID)
        {
            case devID1:
            case devID2:
            case devID3:
                return true;
            default:
                return false;
        }
    }

    // Places hero panel base addresses in array for easier access
    void Rendering::initHeroPanelBasePtrs(void)
    {
        heroPanelIconPtrs[0] = AddressList::getAddress("HeroPanelTagIconLeft");
        heroPanelIconPtrs[1] = AddressList::getAddress("HeroPanelTagIconCenter");
        heroPanelIconPtrs[2] = AddressList::getAddress("HeroPanelTagIconRight");
    }

    void Rendering::checkDevID(MenuEntry *entry)
    {
        Multistatus connectionStatus = GeneralHelpers::checkMultiStatus();
        if (connectionStatus == Multistatus::INET_LOCAL)
        {
            u32 ID = 0x0;
            u32 addr = AddressList::getAddress("MultiplayerInfo");
            int currLink = GeneralHelpers::getCurrLink();

            for (int index = 0, offset = 0x0; index < 3; index++, offset += 0x4)
            {
                Process::Read32(addr + offset, ID);

                if ((devSelf && currLink == index) || verifyDev(ID))
                {
                    IconMngr::overrideGraphic(heroPanelIconPtrs[heroPanelOrder[currLink][index]], IconMngr::retrieveTexPtr("StaffMark00"), 16, 16, CompressionFormat::ETC1_A4);
                    IconMngr::setGraphicOrientation(heroPanelIconPtrs[heroPanelOrder[currLink][index]], 0x4);
                }
                else
                {
                    IconMngr::overrideGraphic(heroPanelIconPtrs[heroPanelOrder[currLink][index]], IconMngr::retrieveTexPtr("RoyalS_TMark00"), 32, 32, CompressionFormat::B4G4R4A4_UNORM);
                    IconMngr::setGraphicOrientation(heroPanelIconPtrs[heroPanelOrder[currLink][index]], 0x2);
                }
            }
        }
        else if (connectionStatus == Multistatus::NOT_CONNECTED)
        {
            for (int player = 0; player < 3; player++)
            {
                IconMngr::toggleVisibility(heroPanelIconPtrs[player], false);
            }
        }
    }
}