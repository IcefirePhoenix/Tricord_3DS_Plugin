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

    bool PlayerStatuses[7][3] = {
        {false, false, false }, // jinx
        {false, false, false }, // spawn
        {false, false, false }, // visibility
        {false, false, false }, // invincibility
        {false, false, false }, // water storage
        {false, false, false }, // collision
        {false, false, false }  // pvp mode
    };

    bool isJinxEdited, isSpawnEdited, isVisibleEdited, isInvinciEdited, isWaterEdited, isCollisionEdited, isPVPEdited, isSizeEdited, isSwordEdited;

    u8 SwordStatuses[3] = { NULL, NULL, NULL };
    float PlayerSizes = NULL;

    void resetPlayer(MenuEntry* entry) {
        // restore default values upon disabling of checkbox entries, if applicable
        // reset link size, sword model (do check the current costume first.. or is this unnecessary?), pvp mode, collision (?), 
    }

    void setPlayerChanges(MenuEntry* entry) {

        // this should be used to actually apply the changes since we're not actually using checkboxes

        if (isJinxEdited) { // check if jinxed is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[0][i]) {
                    Process::Write32((AddressList::IsJinxed.addr + (i * 0x10000)), 0x00000000); // jinxed
                }
                else {
                    Process::Write32((AddressList::IsJinxed.addr + (i * 0x10000)), 0xFFFFFFFF); // not jinxed
                }
            }
        }

        if (isSpawnEdited) { // check if spawned is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[1][i]) {
                    Process::Write8((AddressList::IsSpawned.addr + (i * 0x10000)), 0x1); // spawned
                }
                else {
                    Process::Write8((AddressList::IsSpawned.addr + (i * 0x10000)), 0x0); // not spawned
                }
            }
        }

        if (isVisibleEdited) { // check if visibility is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[2][i]) {
                    Process::Write8((AddressList::IsVisible.addr + (i * 0x10000)), 0x0); // visibility
                }
                else {
                    Process::Write8((AddressList::IsVisible.addr + (i * 0x10000)), 0x1); // not visible
                }
            }
        }

        if (isInvinciEdited) { // check if invinci is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[3][i]) {
                    Process::Write8((AddressList::IsInvincible.addr + (i * 0x10000)), 0x16); // invinci
                }
                else {
                    Process::Write8((AddressList::IsInvincible.addr + (i * 0x10000)), 0x00); // not invinci
                }
            }
        }

        if (isWaterEdited) { // check if water storage is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[4][i]) {
                    Process::Write8((AddressList::IsWaterStorage.addr + (i * 0x10000)), 0x0); // water storage
                    Process::Write16((AddressList::IsWaterStorage.addr + (i * 0x10000) + 0x1), 0x0); // there's no 24-bit write here
                }
                else {
                    Process::Write8((AddressList::IsWaterStorage.addr + (i * 0x10000)), 0xFF); // not water storage
                    Process::Write16((AddressList::IsWaterStorage.addr + (i * 0x10000) + 0x1), 0xFF);
                }
            }
        }

        if (isCollisionEdited) { // check if collision is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[5][i]) {
                    Process::Write8((AddressList::NoCollision.addr + (i * 0x10000)), 0xEA); // no collision
                }
                else {
                    Process::Write8((AddressList::NoCollision.addr + (i * 0x10000)), 0x10); // collision
                }
            }
        }

        if (isPVPEdited) { // check if pvp is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (PlayerStatuses[6][i]) {
                    Process::Write8((AddressList::PVPMode.addr + (i * 0x10000)), 0x1); // pvp
                }
                else {
                    Process::Write8((AddressList::PVPMode.addr + (i * 0x10000)), 0x0); // no pvp
                }
            }
        }

        if (isSwordEdited) { // check if sword type is set to be edited
            for (int i = 0x0; i < 0x3; i++) { // traverse through links
                if (SwordStatuses[i] != NULL) {
                    Process::Write8((AddressList::SwordType.addr + (i * 0x10000)), SwordStatuses[i]); // set sword
                }
            }
        }


        if (isSizeEdited) { // check if size is set to be edited
            if (PlayerSizes > 0.0 || PlayerSizes != NULL) {
                Process::WriteFloat(AddressList::PlayerModelSize.addr, PlayerSizes); // set size
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
    void openToggleMenu(int CurrentStatus, std::string entryTitle, bool& setStatus, std::string enabledText, std::string disabledText) {

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
            title = entryTitle + "\n\n";
            title.append("Player 1: " << ((PlayerStatuses[CurrentStatus][0]) ? (Color::LimeGreen << enabledText) : (Color::Red << disabledText)) << Color::White << "\n");
            title.append("Player 2: " << ((PlayerStatuses[CurrentStatus][1]) ? (Color::LimeGreen << enabledText) : (Color::Red << disabledText)) << Color::White << "\n");
            title.append("Player 3: " << ((PlayerStatuses[CurrentStatus][2]) ? (Color::LimeGreen << enabledText) : (Color::Red << disabledText)) << Color::White << "\n");

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
                setStatus = true;

                // end loop; exit menu
                loop = false;
                break;
            }
            default:
            {
                // disable address writes
                setStatus = false;

                // end loop; exit menu
                loop = false;
                break;
            }
            }
        }
    }

    void jinxOpt(MenuEntry* entry) {
        openToggleMenu(0, "Jinx Options", isJinxEdited, "Jinx model in-use", "Disabled"); // jinx data is located on row 1
    }

    void spawnOpt(MenuEntry* entry) {
        openToggleMenu(1, "Player Spawn Options", isSpawnEdited, "Spawned", "Not spawned"); // spawn data is located on row 2
    }

    void visibilityOpt(MenuEntry* entry) {
        openToggleMenu(2, "Player Visibility Options", isVisibleEdited, "Not visible", "Visible"); // visibility data is located on row 3
    }

    void invincibleOpt(MenuEntry* entry) {
        openToggleMenu(3, "Player Invincibility Options", isInvinciEdited, "Invincible", "Not invincible"); // invinci data is located on row 4
    }

    void waterStorage(MenuEntry* entry) {
        openToggleMenu(4, "Water Storage Options ", isWaterEdited, "Water Storage enabled", "Water Storage disabled"); // water storage data is located on row 5
    }

    void removeCollision(MenuEntry* entry) {
        openToggleMenu(5, "Player Collision Options (experimental!)", isCollisionEdited, "No collision", "Collision enabled"); // collision data is located on row 6
    }

    void pvpMode(MenuEntry* entry) {
        openToggleMenu(6, "PvP Options", isPVPEdited, "PvP enabled", "PvP Disabled"); // pvp data is located on row 7
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
            title = "Player Sword Options:\n\n";
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
                isSwordEdited = true;
                loop = false;

                break;
            }
            default:
            {
                // disable address writes
                isSwordEdited = false;

                // end loop = exit the menu
                loop = false;
                break;
            }
            }
        }
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
            title = "Player Size Options Menu:\n\n";
            title.append("Current size: " << ((PlayerSizes == NULL) ? Color::White << "No changes currently set\n" : Color::White << "Current size: " + std::to_string(PlayerSizes)));

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
            case 0:
            {
                float result;

                Keyboard sizeKB("Set player size:");
                sizeKB.IsHexadecimal(false);
                sizeKB.Open(result);

                if (result < 0.0) {
                    MessageBox(Color::Gainsboro << "Error", "Player Sizes cannot be negative.")();
                }
                else {
                    PlayerSizes = result;
                }
                break;

            }
            case 1:
            {
                // enable address writes
                isSizeEdited = true;

                // end loop = exit the menu
                loop = false;
                break;
            }
            default:
            {
                // disable address writes
                isSizeEdited = false;

                // end loop = exit the menu
                loop = false;
                break;
            }
            }
        }
    }

    void respawnIndicator(MenuEntry* entry) {
        float respawnCoords[3][3] = {
            {NULL, NULL, NULL}, // green
            {NULL, NULL, NULL}, // blue
            {NULL, NULL, NULL}  // red
        };

        // grab respawn coords
        for (int iterateColor = 0; iterateColor < 3; iterateColor++) {
            Process::ReadFloat((AddressList::RespawnPositionX.addr + (iterateColor * 0x10000)), respawnCoords[iterateColor][0]);
            Process::ReadFloat((AddressList::RespawnPositionY.addr + (iterateColor * 0x10000)), respawnCoords[iterateColor][1]);
            Process::ReadFloat((AddressList::RespawnPositionZ.addr + (iterateColor * 0x10000)), respawnCoords[iterateColor][2]);
        }

        // top arrow = G, mid arrow = B, bot arrow = R
        Process::Write8(AddressList::ArrowIndicatorColorTop.addr, 0x0);
        Process::Write8(AddressList::ArrowIndicatorColorMid.addr, 0x1);
        Process::Write8(AddressList::ArrowIndicatorColorBot.addr, 0x2);

        Process::Write32(AddressList::ArrowIndicatorVisibilityTop.addr, 0x10FFFF00);
        Process::Write32(AddressList::ArrowIndicatorVisibilityMid.addr, 0x10FFFF00);
        Process::Write32(AddressList::ArrowIndicatorVisibilityBot.addr, 0x10FFFF00);

        for (int iterateArray = 0; iterateArray < 3; iterateArray++) {
            Process::WriteFloat((AddressList::ArrowIndicatorLocationTop.addr + (iterateArray * 0x4)), respawnCoords[0][iterateArray]);
            Process::WriteFloat((AddressList::ArrowIndicatorLocationMid.addr + (iterateArray * 0x4)), respawnCoords[1][iterateArray]);
            Process::WriteFloat((AddressList::ArrowIndicatorLocationBot.addr + (iterateArray * 0x4)), respawnCoords[2][iterateArray]);
        }
    }

    void bypassDoppelDemo(MenuEntry* entry) {
        u8 currLevelID, currStageID;
        u32 elapsedTime;

        Process::Read8(AddressList::CurrLevelID.addr, currLevelID);
        Process::Read8(AddressList::CurrStageID.addr, currStageID);
        Process::Read32(AddressList::TimeElapsed.addr, elapsedTime);

        if (currLevelID && currStageID == 0x01) {
            Process::Write8(AddressList::DoppelsEnabled.addr, 0x01);

            if (elapsedTime == 0x00000000) {
                // set blue to triforce
                Process::WriteFloat((AddressList::PositionX.addr + 0x10000), -0.8);
                Process::WriteFloat((AddressList::PositionY.addr + 0x10000), 1.1452);
                Process::WriteFloat((AddressList::PositionZ.addr + 0x10000), -9.95);

                // set red to triforce
                Process::WriteFloat((AddressList::PositionX.addr + 0x20000), 0.8);
                Process::WriteFloat((AddressList::PositionY.addr + 0x20000), 1.1452);
                Process::WriteFloat((AddressList::PositionZ.addr + 0x20000), -9.95);
            }
        }
    }
}