#include "cheats.hpp"
#include "csvc.h"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/PlayerHelper.hpp"
#include "CTRPluginFrameworkImpl/Menu/Converter.hpp"

#include "CTRPluginFrameworkImpl/Menu.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/KeyboardImpl.hpp"

#include <CTRPluginFramework.hpp>

#include "3ds.h"

namespace CTRPluginFramework
{
    // column 1-3 = link statuses
    // column 4 = whether entry is enabled/disabled
    bool PlayerStatuses[7][4] = {
        {false, false, false, false}, // jinx
        {false, false, false, false}, // spawn
        {false, false, false, false}, // visibility
        {false, false, false, false}, // invincibility
        {false, false, false, false}, // water storage
        {false, false, false, false}, // collision
        {false, false, false, false} // pvp mode
    };

    // col 1-3 = statuses
    // col 4 = u8 used as bool
    u8 SwordStatuses[4] = {NULL, NULL, NULL, 0};

    // col 1 = statuses
    // col 2 = float used as bool
    float PlayerSizes[2] = { NULL, 0.0 };

    void resetPlayer(MenuEntry* entry) {
        // restore default values upon disabling of checkbox entries, if applicable

        // reset link size, sword model (do check the current costume first.. or is this unnecessary?), pvp mode, collision (?), 
    }

    void setPlayerChanges(MenuEntry* entry) {
        // this should be used to actually apply the changes since we're not actually using checkboxes

        if (PlayerStatuses[0][3]) { // check if jinxed is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[0][i]) {
                    Process::Write32((AddressList::IsJinxed.addr + (i * 0x10000)), 0x00000000); // jinxed
                }
                else {
                    Process::Write32((AddressList::IsJinxed.addr + (i * 0x10000)), 0xFFFFFFFF); // not jinxed
                }
            }
        }

        if (PlayerStatuses[1][3]) { // check if spawned is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[1][i]) {
                    Process::Write8((AddressList::IsSpawned.addr + (i * 0x10000)), 0x1); // spawned
                }
                else {
                    Process::Write8((AddressList::IsSpawned.addr + (i * 0x10000)), 0x0); // not spawned
                }
            }
        }

        if (PlayerStatuses[2][3]) { // check if visibility is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[2][i]) {
                    Process::Write8((AddressList::IsVisible.addr + (i * 0x10000)), 0x0); // visibility
                }
                else {
                    Process::Write8((AddressList::IsVisible.addr + (i * 0x10000)), 0x1); // not visible
                }
            }
        }

        if (PlayerStatuses[3][3]) { // check if invinci is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[3][i]) {
                    Process::Write8((AddressList::IsInvincible.addr + (i * 0x10000)), 0x16); // invinci
                }
                else {
                    Process::Write8((AddressList::IsInvincible.addr + (i * 0x10000)), 0x00); // not invinci
                }
            }
        }

        if (PlayerStatuses[4][3]) { // check if water storage is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[4][i]) {
                    Process::Write8((AddressList::IsWaterStorage.addr + (i * 0x10000)), 0x0); // water storage
                    Process::Write16((AddressList::IsWaterStorage.addr + (i * 0x10000) + 0x1 ), 0x0); // there's no 24-bit write here
                }
                else {
                    Process::Write8((AddressList::IsWaterStorage.addr + (i * 0x10000)), 0xFF); // not water storage
                    Process::Write16((AddressList::IsWaterStorage.addr + (i * 0x10000) + 0x1), 0xFF);
                }
            }
        }

        if (PlayerStatuses[5][3]) { // check if collision is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[4][i]) {
                    Process::Write8((AddressList::NoCollision.addr + (i * 0x10000)), 0x16); // no collision
                }
                else {
                    Process::Write8((AddressList::NoCollision.addr + (i * 0x10000)), 0x00); // collision
                }
            }
        }

        if (PlayerStatuses[6][3]) { // check if pvp is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[4][i]) {
                    Process::Write8((AddressList::PVPMode.addr + (i * 0x10000)), 0x1); // pvp
                }
                else {
                    Process::Write8((AddressList::PVPMode.addr + (i * 0x10000)), 0x0); // no pvp
                }
            }
        }

        if (SwordStatuses[3] == 1) { // check if sword type is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (SwordStatuses[i] != NULL) {
                    Process::Write8((AddressList::SwordType.addr + (i * 0x10000)), SwordStatuses[i]); // set sword
                }
            }
        }


        if (PlayerSizes[1] == 1.0) { // check if size is set to be edited
            if (PlayerSizes[0] > 0.0 || PlayerSizes[0] != NULL) {
                Process::Write32(AddressList::PlayerModelSize.addr, PlayerSizes[0]); // set size
            }
        }
    }

    void posEditor(MenuEntry* entry) {
        // TODO:
        // not sure how to implement this
        // 
        // could do:
        // - passive touchscreen controls -> Tricord does NOT need to be open at all times
        // - graphics should preferably come from current assets, though custom ones 
        // can be necessary
        // - graphics need to be moved around -> universal edits are preferable 
        // - the graphical edits should be applied even if a new area is loaded in 
        // - also the edits should be loaded in dynamically where they do disappear in case
        // additional grsphics that are not part of the usual set are loaded in (selection
        // screens, etc.)
        // - two new graphics are drawn on the touchscreen: load and save buttons... also need to figure out what graphics to draw...
        // - to save position: swap to desired Link, then hit save button -- this will store the X, Y, and Z coords
        // - to load position: swap to desired Link, the hit load button -- this will write stored values to pos addresses
        // 
        // todo: figure out how to draw to the screen and load in the custom graphics etc
        // 
        // - note 1: if no position was saved for the current Link AND load button was pressed, display OSD notif on 
        // top screen saying that there's no saved position to load
        // - note 2: the code should check current Link first

    }

    // this opens a toggle menu similar to OnionFS
    void openToggleMenu(int CurrentStatus) {

        // set up bottom menu options
        std::string enSlid = Color::LimeGreen << "\u2282\u25CF";
        std::string disSlid = Color::Red << "\u25CF\u2283";
        std::string title;

        // placeholders
        Keyboard kbd("dummy text");
        StringVector opts;

        // this menu stays open regardless of input UNLESS the user specifies they wish to exit
        bool loop = true;
        kbd.CanAbort(false);

        while (loop) {
            // update top screen info
            // should the current values be checked first
            title = "Editing player options:\n\n";
            title.append("Player 1: " << ((PlayerStatuses[CurrentStatus][0]) ? (Color::LimeGreen << "Enabled") : (Color::Red << "Disabled")) << Color::White << "\n");
            title.append("Player 2: " << ((PlayerStatuses[CurrentStatus][1]) ? (Color::LimeGreen << "Enabled") : (Color::Red << "Disabled")) << Color::White << "\n");
            title.append("Player 3: " << ((PlayerStatuses[CurrentStatus][2]) ? (Color::LimeGreen << "Enabled") : (Color::Red << "Disabled")) << Color::White << "\n");

            // update bottom screen info
            opts.clear();
            opts.push_back(std::string("Player 1 ") << ((PlayerStatuses[CurrentStatus][0]) ? enSlid : disSlid));
            opts.push_back(std::string("Player 2 ") << ((PlayerStatuses[CurrentStatus][1]) ? enSlid : disSlid));
            opts.push_back(std::string("Player 3 ") << ((PlayerStatuses[CurrentStatus][2]) ? enSlid : disSlid));
            opts.push_back("Save changes");
            opts.push_back("Disable entry"); 

            // display top screen info
            kbd.GetMessage() = title;

            // populate bottom screen options
            kbd.Populate(opts);

            // begin watching for changes
            int chose;
            switch (chose = kbd.Open())
            { 
                // toggle functionality -> swaps current status (enabled/disabled)
                case 0:
                {
                    PlayerStatuses[CurrentStatus][0] = !PlayerStatuses[CurrentStatus][0];
                    break;
                }
                case 1:
                {
                    PlayerStatuses[CurrentStatus][1] = !PlayerStatuses[CurrentStatus][1];
                    break;
                }
                case 2:
                {
                    PlayerStatuses[CurrentStatus][2] = !PlayerStatuses[CurrentStatus][2];
                    break;
                }
                case 3:
                {
                    // enable address writes
                    PlayerStatuses[CurrentStatus][3] = true;
                    loop = false;
                    break;
                }
                default:
                {
                    // disable address writes
                    PlayerStatuses[CurrentStatus][3] = false;

                    // end loop = exit the menu
                    loop = false;
                    break;
                }
            }
        }
    }

    void jinxOpt(MenuEntry* entry) {
        openToggleMenu(0); // jinx data is located on row 1
    }

    void spawnOpt(MenuEntry* entry) {
        openToggleMenu(1); // spawn data is located on row 2
    }

    void visibilityOpt(MenuEntry* entry) {
        openToggleMenu(2); // visibility data is located on row 3
    }

    void invincibleOpt(MenuEntry* entry) {
        openToggleMenu(3); // invinci data is located on row 4
    }

    void waterStorage(MenuEntry* entry) {
        openToggleMenu(4); // water storage data is located on row 5
    }

    void removeCollision(MenuEntry* entry) {
        openToggleMenu(5); // collision data is located on row 6
    }

    void pvpMode(MenuEntry* entry) {
        openToggleMenu(6); // pvp data is located on row 7
    }

    void swordModelOpt(MenuEntry* entry) {

        // placeholders
        Keyboard kbd("dummy text");
        std::string title;
        StringVector opts;

        // this menu stays open regardless of input UNLESS the user specifies they wish to exit
        bool loop = true;
        kbd.CanAbort(false);

        while (loop) {
            // update top screen info
            title = "Editing player swords:\n\n";
            title.append("Player 1: " << ((SwordStatuses[0] == NULL) ? Color::White << "No changes currently set\n" : Color::White << (swordList[SwordStatuses[0]] + "\n")));
            title.append("Player 2: " << ((SwordStatuses[1] == NULL) ? Color::White << "No changes currently set\n" : Color::White << (swordList[SwordStatuses[1]] + "\n")));
            title.append("Player 3: " << ((SwordStatuses[2] == NULL) ? Color::White << "No changes currently set\n" : Color::White << (swordList[SwordStatuses[2]] + "\n")));

            // update bottom screen info
            opts.clear();
            opts.push_back(std::string("Set Player 1 sword"));
            opts.push_back(std::string("Set Player 2 sword"));
            opts.push_back(std::string("Set Player 3 sword"));
            opts.push_back("Save changes");
            opts.push_back("Disable entry");

            // display top screen info
            kbd.GetMessage() = title;

            // populate bottom screen options
            kbd.Populate(opts);

            // begin watching for changes
            int chose;
            switch (chose = kbd.Open()) {
                // toggle functionality -> swaps current status (enabled/disabled)
                case 0:
                {
                    SwordStatuses[0] = chooseSword();
                    break;
                }
                case 1:
                {
                    SwordStatuses[1] = chooseSword();
                    break;
                }
                case 2:
                {
                    SwordStatuses[2] = chooseSword();
                    break;
                }
                case 3:
                {
                    // enable address writes
                    SwordStatuses[3] = 1;
                    loop = false;

                    break;
                }
                default:
                {
                    // disable address writes
                    SwordStatuses[3] = 0;

                    // end loop = exit the menu
                    loop = false;
                    break;
                }
            }
        }
    }

    // might need to be moved to a helper file
    u8 chooseSword(void) {
        Keyboard swordMenu("Choose a sword:");
        swordMenu.Populate(swordList);

        return swordMenu.Open();
    }

    void linkSize(MenuEntry* entry) {
        // placeholders 
        Keyboard kbd("dummy text");
        std::string title;
        StringVector opts;

        // this menu stays open regardless of input UNLESS the user specifies they wish to exit
        bool loop = true;
        kbd.CanAbort(false);

        while (loop) {
            // update top screen info
            title = "Editing size for ALL players:\n\n";
            title.append("Current size: " << ((PlayerSizes[0] == NULL) ? Color::White << "No changes currently set\n" : Color::White << "Current size: " + std::to_string(PlayerSizes[0])));
     
            // update bottom screen info
            opts.clear();
            opts.push_back(std::string("Set size"));
            opts.push_back("Save changes");
            opts.push_back("Disable entry");
            
            // display top screen info
            kbd.GetMessage() = title;

            // populate bottom screen options
            kbd.Populate(opts);

            // begin watching for changes
            int chose;
            switch (chose = kbd.Open()) {
                // toggle functionality -> swaps current status (enabled/disabled)
                case 0:
                {
                    u32 testsize;
                    Process::Read32(AddressList::PlayerModelSize.addr, testsize);
                    OSD::Notify(std::to_string(testsize));

                    // currently broken: this whole section needs to be changed to convert input to u32 hex

                    float result;

                    Keyboard sizeKB("Set player size:");
                    sizeKB.IsHexadecimal(false);
                    sizeKB.SetMaxLength(3);

                    sizeKB.Open(result);

                    PlayerSizes[0] = result;

                    OSD::Notify(std::to_string(PlayerSizes[0]));
                    break;
                
                }
                case 1:
                {
                    // enable address writes
                    PlayerSizes[1] = 1.0;

                    // end loop = exit the menu
                    loop = false;
                    break;
                }
                default:
                {
                    // disable address writes
                    PlayerSizes[1] = 0.0;

                    // end loop = exit the menu
                    loop = false; 
                    break;
                }
            }
        }
    }
}