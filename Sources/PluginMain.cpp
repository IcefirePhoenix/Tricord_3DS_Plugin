#include <3ds.h>
#include "cheats.hpp"
#include "csvc.h"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "CTRPluginFrameworkImpl/Menu/HotkeysModifier.hpp"

#include "CTRPluginFramework/Menu/MenuEntry.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryTools.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuTools.hpp"

#include <CTRPluginFramework.hpp>

#include <vector>

namespace CTRPluginFramework {

    // Menu Entries aren't given a name by default
    // If they need to be referenced elsewhere, they need a name
    MenuEntry* menuCostumeSlotA;
    MenuEntry* menuCostumeSlotB;
    MenuEntry* menuCostumeSlotC;
    MenuEntry* menuCostumeSlotD;

    MenuEntry* manageCatalogSize;
    MenuEntry* restoreGreatFairy;

    MenuEntry* initCustomCostumes;
    MenuEntry* writeCostumeIDToSlot;

    MenuEntry* resetMiscellaneous;
    MenuEntry* instantTextDisplay;

    MenuEntry* managePlayerCodes;


    // experimental -> hotkey menu entry
    static MenuEntry* EntryWithHotkey(MenuEntry* entry, const std::vector<Hotkey>& hotkeys) {
        if (entry != nullptr) {
            for (const Hotkey& hotkey : hotkeys)
                entry->Hotkeys += hotkey;
        }
        return entry;
    }


    void    InitMenu(PluginMenu& menu)
    {
        // create emote folder
        MenuFolder* emotes = new MenuFolder("Emote Codes");
        {
            *emotes += new MenuEntry("Enable Emote Swapper", emoteSwapper);
            *emotes += new MenuEntry("Use Custom Emotes", customEmotes);
        }

        // create linkcolor folder
        MenuFolder* linkcolor = new MenuFolder("Custom Link Colors");
        {
            // implement presets...?
            *linkcolor += new MenuEntry("Set Green Link color", nullptr, customColor);
            *linkcolor += new MenuEntry("Set Blue Link color", nullptr, customColor);
            *linkcolor += new MenuEntry("Set Red Link color", nullptr, customColor);
            *linkcolor += new MenuEntry("Use re-colored level textures", recolorLevelTex);
        }

        // create costume folder
        MenuFolder* costume = new MenuFolder("Costume Codes");
        {
            *costume += new MenuEntry("Trigger Custom Costume Slots", nullptr, openCustomCostumeSlots);

            // Menu Entries defined above
            menuCostumeSlotA = new MenuEntry("Set custom costume slot A", nullptr, selectCostumeID, "This setting is not accessible if Restore Great Fairy Costume is enabled.");
            menuCostumeSlotB = new MenuEntry("Set custom costume slot B", nullptr, selectCostumeID);
            menuCostumeSlotC = new MenuEntry("Set custom costume slot C", nullptr, selectCostumeID);
            menuCostumeSlotD = new MenuEntry("Set custom costume slot D", nullptr, selectCostumeID);
            restoreGreatFairy = new MenuEntry("Restore Great Fairy Costume", greatFairyEnable);

            *costume += menuCostumeSlotA;
            *costume += menuCostumeSlotB;
            *costume += menuCostumeSlotC;
            *costume += menuCostumeSlotD;
            *costume += restoreGreatFairy;

            // by default these should be hidden 
            menuCostumeSlotA->Hide();
            menuCostumeSlotB->Hide();
            menuCostumeSlotC->Hide();
            menuCostumeSlotD->Hide();

            // these are not added to the menu -- these are auto-managed by the plugin
            manageCatalogSize = new MenuEntry("Manage Catalog Size (auto)", manageCatalogSizeAuto);
            initCustomCostumes = new MenuEntry("Init Custom Costume list (auto)", initCustomCostumesAuto);
            writeCostumeIDToSlot = new MenuEntry("Write to costume slots (auto", writeCostumeSlot);

            *costume += new MenuEntry("Change Player Costume", nullptr, changeLinkCostume);
        }

        // create costume sub-folders
        MenuFolder* costumeEffects = new MenuFolder("Costume Effect(s)");
        *costumeEffects += new MenuEntry("Set Costume Effects", nullptr, selCostumeEffect);

        MenuFolder* extraCustomConfig = new MenuFolder("Additional Customizations");
        // *extraCustomConfig += new MenuEntry("Set number of Tingle Balloons", nullptr, tingleBalloonNumber);
        // *extraCustomConfig += new MenuEntry("Set Cheetah walking speed", nullptr, cheetahSpeed);
        // *extraCustomConfig += new MenuEntry("Set luck percentage", nullptr, luckPercent);
        // *extraCustomConfig += new MenuEntry("Set number of additional Hearts", nullptr, addHeart);
        // *extraCustomConfig += new MenuEntry("Set sword beam type", nullptr, swordBeamConfig);

        *costume += costumeEffects;
        *extraCustomConfig += costumeEffects;

        // create misc folder
        MenuFolder* miscellaneous = new MenuFolder("Miscellaneous Codes");
        {
            resetMiscellaneous = new MenuEntry("Reset Miscellaneous codes (auto)", defaultMisc);
            instantTextDisplay = new MenuEntry("Force instant text boxes", instantText);

            // test hotkey entry
            *miscellaneous += (EntryWithHotkey(new MenuEntry("Enable button spam", buttonSpammer, "When any of the selected keys are\npressed down, they will automatically spam.\nGood for in-game manuevers that require\nstrict timing of button input(s).\n\nDefault keys: A, B, X, Y, L, R."), {
                Hotkey(Key::A | Key::B | Key::X | Key::Y | Key::L | Key::R , "Button Spammer")
                }));

            *miscellaneous += new MenuEntry("Disable sword beam cooldown", beamCooldown);
            *miscellaneous += new MenuEntry("Display photo on top screen", displayPhoto);
            *miscellaneous += instantTextDisplay;
        }

        // create player folder
        MenuFolder* player = new MenuFolder("Player Codes");
        {
            // hotkeys -> posEditor (?)
            *player += new MenuEntry("Enable position editor menu", posEditor);
            *player += new MenuEntry("Enable Water Storage", nullptr, waterStorage);
            *player += new MenuEntry("Spawn/Despawn Links", nullptr, spawnOpt);
            *player += new MenuEntry("Set Link model size", nullptr, linkSize);
            *player += new MenuEntry("Set Sword Types", nullptr, swordModelOpt);
            *player += new MenuEntry("Toggle Collision", nullptr, removeCollision);
            *player += new MenuEntry("Toggle Jinxed model", nullptr, jinxOpt);
            *player += new MenuEntry("Toggle Link Visibility", nullptr, visibilityOpt);
            *player += new MenuEntry("Toggle Link Invincibility", nullptr, invincibleOpt);
            *player += new MenuEntry("Toggle PvP damage", nullptr, pvpMode);

            // this is not added to the player folder
            managePlayerCodes = new MenuEntry("Set Player edits (auto)", setPlayerChanges);
        }

        // create energy folder
        MenuFolder* energy = new MenuFolder("Energy Codes");
        {
            *energy += new MenuEntry("Infinite energy", infEnergy);
            *energy += new MenuEntry("Set max energy amount", nullptr, maxEnergySet);
            *energy += new MenuEntry("Set energy consumption multiplier", nullptr, energyConsumeMultiplier);
        }

        // create item folder
        MenuFolder* items = new MenuFolder("Item Codes");
        {
            *items += new MenuEntry("Set current item", itemOpt);
            *items += new MenuEntry("Set Shadow Link item", shadowItemOpt);
            *items += new MenuEntry("Set strafing speeds", nullptr, strafingSpeedSet);
            *items += new MenuEntry("Always use upgraded Items", upgradeItemAlways);
        }

        MenuFolder* render = new MenuFolder("Rendering Codes");
        {
            *render += new MenuEntry("Hide HUD", hideHUD);
            *render += new MenuEntry("Disable fog effects", disableFog);
        }

        // create save folder
        MenuFolder* save = new MenuFolder("Savefile Codes");
        {
            *save += new MenuEntry("Set Main Voice", nullptr, mainVoice);
            *save += new MenuEntry("Set Hero Point count", nullptr, heroPointCountSet);
            *save += new MenuEntry("Set Coliseum Win count", nullptr, coliseumWinCountSet);
            *save += new MenuEntry("Edit Merchant stock", openMerchantSlots);
            *save += new MenuEntry("Set 1st slot", merchantSlotA);
            *save += new MenuEntry("Set 2nd slot", merchantSlotB);
            *save += new MenuEntry("Set 3rd slot", merchantSlotC);
            *save += new MenuEntry("Set 4th slot", merchantSlotD);
            *save += new MenuEntry("Set 5th slot", merchantSlotE);
            *save += new MenuEntry("Reset Merchant stock availability", nullptr, resetMerchant);
        }

        // create sound folder
        MenuFolder* sound = new MenuFolder("BGM and SFX Codes");
        {
            *sound += new MenuEntry("Set current BGM", bgmSet);
            *sound += new MenuEntry("Set BGM volume", nullptr, bgmVolSet);
            *sound += new MenuEntry("Choose Lobby Ball song", lobbyBallSong);
            *sound += new MenuEntry("Set Lobby Ball volume", lobbyBallVol);
            *sound += new MenuEntry("Set Link Voice volume", nullptr, voiceVol);
            *sound += new MenuEntry("Set Low Health Alert volume", nullptr, lowHPVol);
            // unnecessary?
            // *sound += new MenuEntry("Set Level Completion Fanfare volume", levelDoneVol);
        }

        // is this correct syntax?
        //menu += emotes, linkcolor, costume, miscellaneous, player, energy, items, render, save, sound;

        // add folders to menu
        menu += emotes;
        menu += linkcolor;
        menu += costume;
        menu += miscellaneous;
        menu += player;
        menu += energy;
        menu += items;
        menu += render;
        menu += save;
        menu += sound;
    }

    int main(void)
    {
        // title is left blank since the name is already set within the lib
        PluginMenu* menu = new PluginMenu("", 0, 5, 0,
            "An advanced, region-free cheat plugin made for\nThe Legend of Zelda: Tri Force Heroes.\n\nForked from the original CTRPluginFramework\nblank template repository.");

        // Synchronize the menu with frame event
        menu->SynchronizeWithFrame(true);

        // Init menu entries & folders
        InitMenu(*menu);

        // init reset functions
        // resetCostume->Enable();
        resetMiscellaneous->Enable();

        // init continuous functions
        managePlayerCodes->Enable();

        // Launch menu and mainloop
        menu->Run();

        delete menu;

        // Exit plugin
        return (0);
    }

    // Patch NFC disabling the touchscreen when scanning an amiibo
    static void    ToggleTouchscreenForceOn(void)
    {
        static u32 original = 0;
        static u32* patchAddress = nullptr;

        if (patchAddress && original)
        {
            *patchAddress = original;
            return;
        }

        static const std::vector<u32> pattern =
        {
            0xE59F10C0, 0xE5840004, 0xE5841000, 0xE5DD0000,
            0xE5C40008, 0xE28DD03C, 0xE8BD80F0, 0xE5D51001,
            0xE1D400D4, 0xE3510003, 0x159F0034, 0x1A000003
        };

        Result  res;
        Handle  processHandle;
        s64     textTotalSize = 0;
        s64     startAddress = 0;
        u32* found;

        if (R_FAILED(svcOpenProcess(&processHandle, 16)))
            return;

        svcGetProcessInfo(&textTotalSize, processHandle, 0x10002);
        svcGetProcessInfo(&startAddress, processHandle, 0x10005);
        if (R_FAILED(svcMapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, processHandle, (u32)startAddress, textTotalSize)))
            goto exit;

        found = (u32*)Utils::Search<u32>(0x14000000, (u32)textTotalSize, pattern);

        if (found != nullptr)
        {
            original = found[13];
            patchAddress = (u32*)PA_FROM_VA((found + 13));
            found[13] = 0xE1A00000;
        }

        svcUnmapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, textTotalSize);
    exit:
        svcCloseHandle(processHandle);
    }

    void    PatchProcess(FwkSettings& settings)
    {
        AddressList::InitAddresses();

        ToggleTouchscreenForceOn();
    }

    // Called when the process exits
    // Useful to save settings, undo patches or clean up things
    void    OnProcessExit(void)
    {
        ToggleTouchscreenForceOn();
    }
}
