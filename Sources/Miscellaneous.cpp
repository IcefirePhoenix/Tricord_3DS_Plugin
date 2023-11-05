#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "CTRPluginFramework/Menu/MenuEntryHotkeys.hpp"

#include "CTRPluginFrameworkImpl/Menu/HotkeysModifier.hpp"

#include "3ds.h"

namespace CTRPluginFramework
{
    void defaultMisc(MenuEntry* entry) {
        // handles any codes in this file that have a "default/vanilla" state
        // these are restored when entries are disabled

        if (!instantTextDisplay->IsActivated()) {
            Process::Write8(AddressList::TextBoxSpeed.addr, 0x01);
        }
    }

    u32 keys;
    void buttonSpammer(MenuEntry* entry) {
        // currently a mess; ignore for now

        // int chosenButtons = entry->Hotkeys.Count();
        // OSD::Notify(std::to_string(chosenButtons));
        // Controller::InjectKey(Controller::GetKeysDown(true));
    }

    void instantText(MenuEntry* entry) {
        // not using the max of 0xFF to avoid graphical bugs... 0x2D is fast enough
        Process::Write8(AddressList::TextBoxSpeed.addr, 0x2D);
    }
    
    void displayPhoto(MenuEntry* entry) {
        // TODO: draw toggle button
        // TODO: config touchscreen bounds
    }

    void beamCooldown(MenuEntry* entry) {
        // TODO: simple 32-bit write to beam address -> 0x0000001E
        // TODO: opening gear -> opens combo box with toggle switches

    }

    void seeChestContents(MenuEntry* entry) {
        if (entry->WasJustActivated()) {
            Process::Patch(AddressList::SeeChestContents.addr, 0xEA000017);
        } 
        if (!entry->IsActivated()) {
            Process::Patch(AddressList::SeeChestContents.addr, 0x0A000017);
        }
    }
}