#include "Helpers.hpp"

namespace CTRPluginFramework
{
    int GeneralHelpers::chooseLink(void)
    {
        Keyboard player("Choose a Link:");
        static const StringVector linkList =
        {
            "Player 1 (Green)",
            "Player 2 (Blue)",
            "Player 3 (Red)"
        };
        player.Populate(linkList);

        return player.Open();
    }

    int GeneralHelpers::chooseShadowLink(void)
    {
        Keyboard shadow("Choose a Shadow Link:");
        static const StringVector shadowList =
        {
            "Shadow Link A",
            "Shadow Link B",
            "Shadow Link C"
        };
        shadow.Populate(shadowList);

        return shadow.Open();
    }

    int GeneralHelpers::chooseSword(void)
    {
        Keyboard swordMenu("Choose a sword:");
        swordMenu.Populate(GameData::swordList);

        return swordMenu.Open();
    }

    // borrowed from OnionFS
    bool GeneralHelpers::showMsgKbd(std::string text, DialogType digtype)
    {
        Keyboard kbd(text);
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
        return Level::isInDrablands() && isSinglePlayer() && !isPauseScreen() && !isLoadingScreen();
    }

    bool GeneralHelpers::isLoadingScreen(void)
    {
        u8 type;
        Process::Read8(AddressList::getAddress("LoadingStatus"), type);

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
}

