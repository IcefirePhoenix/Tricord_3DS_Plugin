#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/GameData.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/GeneralHelpers.hpp"

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework
{
    std::string GeneralHelpers::enabledSlider = Color::LimeGreen << "\u2282\u25CF";
    std::string GeneralHelpers::disabledSlider = Color::Red << "\u25CF\u2283";

    u32 GeneralHelpers::chooseLink(void) {
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

    u32 GeneralHelpers::chooseShadowLink(void) {
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

    u8 GeneralHelpers::chooseSword(void) {
        Keyboard swordMenu("Choose a sword:");
        swordMenu.Populate(GameData::swordList);

        return swordMenu.Open();
    }

    // borrowed from OnionFS
    bool GeneralHelpers::showMsgKbd(std::string text, DialogType digtype) {
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

    bool GeneralHelpers::isNullPointer(u32 address) {
        return address == 0x00000000;
    }

    void GeneralHelpers::managePlayerLock(bool isLocked) {
        int lock = isLocked ? 0x10 : 0x00;
        Process::Write8(AddressList::LockMovement.addr, lock);
    }

    void GeneralHelpers::forceEnableDoppels(void) {
        Process::Write8(AddressList::DoppelsEnabled.addr, 0x01);
    }

    bool GeneralHelpers::isSinglePlayer(void) {
        u8 mode;
        Process::Read8(AddressList::DoppelsEnabled.addr, mode);

        return mode == 0x01;
    }

    bool GeneralHelpers::isLoadingScreen(void) {
        u8 type;
        Process::Read8(AddressList::LoadingStatus.addr, type);

        return type == 0x01;
    }

    bool GeneralHelpers::isPauseScreen(void) {
        u8 status;
        Process::Read8(AddressList::PauseStatus.addr, status);

        return status == 0x03;
    }
}

