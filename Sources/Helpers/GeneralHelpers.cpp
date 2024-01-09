#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/GameData.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/GeneralHelpers.hpp"

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework
{
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
}
