#include "Helpers.hpp"

namespace CTRPluginFramework
{
    int GeneralHelpers::chooseLink(void)
    {
        switch (PlayerSelector(false)())
        {
            case PlayerMask::PLAYER1:
                return 0;
            case PlayerMask::PLAYER2:
                return 1;
            case PlayerMask::PLAYER3:
                return 2;
            default:
                return -1;
        }
    }

    int GeneralHelpers::chooseSword(void)
    {
        Keyboard swordMenu("Sword Model Selection", "Choose a sword model.");
        swordMenu.Populate(GameData::swordList);

        return swordMenu.Open();
    }

    bool GeneralHelpers::showMsgKbd(std::string title, std::string text, DialogType digtype)
    {
        Keyboard kbd(title, text);
        StringVector opts;

        switch (digtype)
        {
            case CTRPluginFramework::DialogType::DialogOk:
                opts = { "Ok" };
                break;
            case CTRPluginFramework::DialogType::DialogOkCancel:
                opts = { "Ok", "Cancel" };
                break;
            case CTRPluginFramework::DialogType::DialogYesNo:
                opts = { "Yes", "No" };
                break;
            default:
                break;
        }
        kbd.Populate(opts);
        return kbd.Open() == 0;
    }



    bool GeneralHelpers::isNullPointer(u32 address)
    {
        return address == 0x00000000;
    }

    void GeneralHelpers::managePlayerLock(bool isLocked)
    {
        int lock = isLocked ? 0x10 : 0x00;
        Process::Write8(AddressList::getAddress("LockMovement"), lock);
    }

    void GeneralHelpers::forceDoppelStatus(bool enable)
    {
        u8 status = enable ? 0x1 : 0x0;
        Process::Write8(AddressList::getAddress("DoppelsEnabled"), status);
    }

    bool GeneralHelpers::isSinglePlayer(void)
    {
        u8 mode;
        Process::Read8(AddressList::getAddress("DoppelsEnabled"), mode);

        return mode;
    }

    bool GeneralHelpers::isSoloActiveGameplay(void)
    {
        return Level::isInDrablands() && isSinglePlayer() && !isPauseScreen() && !isLoadingScreen(false);
    }

    bool GeneralHelpers::isLoadingScreen(bool checkInitStateOnly)
    {
        u32 address = checkInitStateOnly ? AddressList::getAddress("LoadingInitState") : AddressList::getAddress("LoadingActiveStatus");
        u8 type;

        Process::Read8(address, type);
        return type;
    }

    bool GeneralHelpers::isPauseScreen(void)
    {
        u8 status;
        Process::Read8(AddressList::getAddress("PauseStatus"), status);

        return status == 0x3;
    }

    bool GeneralHelpers::isTitleScreen(void)
    {
        // locate title screen data -> used to ensure plugin is not
        // usable during the title screen

        return true;
    }

    std::string GeneralHelpers::getLinkColorAsStr(int ID)
    {
        std::string color= "";

        switch (ID)
        {
            case 0:
                color = "Green";
                break;
            case 1:
                color = "Blue";
                break;
            case 2:
                color = "Red";
                break;
        }
        return color;
    }

    std::string GeneralHelpers::getPlayerAsStr(int ID)
    {
        std::string player = "";
        switch (ID) {
            case 0:
                player = "Player 1 (Green)";
                break;
            case 1:
                player = "Player 2 (Blue)";
                break;
            case 2:
                player = "Player 3 (Red)";
                break;
        }
        return player;
    }

    int GeneralHelpers::getCurrLink(void)
    {
        u8 currLink;
        Process::Read8(AddressList::getAddress("ActiveLink"), currLink);

        return static_cast<int>(currLink);
    }

    int GeneralHelpers::getHP(void)
    {
        u8 currHP;
        Process::Read8(AddressList::getAddress("HealthCurrent"), currHP);

        return static_cast<int>(currHP);
    }

    void GeneralHelpers::setCurrLink(int playerID)
    {
        Process::Write8(AddressList::getAddress("ActiveLink"), playerID);
    }

    void GeneralHelpers::setPlayerCam(int playerID)
    {
        Process::Write8(AddressList::getAddress("CameraLinkFocus"), playerID);
    }

    // Checks online connectivity by verifying if host data is present
    Multistatus GeneralHelpers::checkMultiStatus(void)
    {
        u32 hostData;
        Process::Read32(AddressList::getAddress("HostDataStart"), hostData);

        switch (hostData)
        {
            case 0xFFFFFFFD:
                return Multistatus::NOT_CONNECTED;
            default:
                if (hostData > 0xFFFFFFFD)
                    return Multistatus::UNKNOWN;
                else
                    return Multistatus::INET_LOCAL;
        }
    }
}

