#include <3ds.h>
#include "cheats.hpp"
#include "csvc.h"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/Level.hpp"

#include "CTRPluginFrameworkImpl/Menu/HotkeysModifier.hpp"

#include "CTRPluginFramework/Menu/MenuEntry.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryTools.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/Menu.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuHome.hpp"

#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuTools.hpp"

#include <CTRPluginFramework.hpp>

#include <vector>

namespace CTRPluginFramework {

    // Menu Entries aren't given a name by default
    // If they need to be referenced elsewhere, they need a name
    MenuEntry* triggerCostumeSlots;

    MenuEntry* menuCostumeSlotA;
    MenuEntry* menuCostumeSlotB;
    MenuEntry* menuCostumeSlotC;
    MenuEntry* menuCostumeSlotD;

    MenuEntry* manageCatalogSize;
    MenuEntry* restoreGreatFairy;

    MenuEntry* initCustomCostumes;
    MenuEntry* writeCostumeIDToSlot;

    MenuEntry* autoBeamCooldown;
    MenuEntry* instantTextDisplay;

    MenuEntry* managePlayerCodes;

    MenuEntry* menuFreecam;
    MenuEntry* editFreecamControls;
    MenuEntry* editFreecamSen;
    MenuEntry* swapZoom;

    MenuFolder* emotes;
    MenuFolder* gameplay;
    MenuFolder* linkcolor;
    MenuFolder* costume;
    MenuFolder* miscellaneous;
    MenuFolder* player;
    MenuFolder* energy;
    MenuFolder* items;
    MenuFolder* render;
    MenuFolder* save;
    MenuFolder* sound;

    static MenuEntry* EntryWithHotkey(MenuEntry* entry, const std::vector<Hotkey>& hotkeys) {
        if (entry != nullptr) {
            for (const Hotkey& hotkey : hotkeys)
                entry->Hotkeys += hotkey;
        }
        return entry;
    }

    void    InitMenu(PluginMenu& menu)
    {
        InitFreecamCodes(menu);
        InitEmoteFolder(menu);
        InitColorFolder(menu);
        InitCostumeFolder(menu);
        InitMiscFolder(menu);
        InitPlayerFolder(menu);
        InitEnergyFolder(menu);
        InitItemsFolder(menu);
        InitRenderFolder(menu);
        InitSaveFolder(menu);
        InitSoundFolder(menu);

        AddAllFolders(menu);
    }

    void AddAllFolders(PluginMenu& menu)
    {
        menu += gameplay;
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

    void InitEmoteFolder (PluginMenu& menu) 
    {
        emotes = new MenuFolder("Emote Codes");

        *emotes += (EntryWithHotkey(new MenuEntry("Enable Drablands Emote Swapper", drablandEmoteSwapper), {
            Hotkey(Key::DPadLeft, "Swap to original emote set"),
            Hotkey(Key::DPadRight, "Swap to alternative emote set")
            }));

        *emotes += (EntryWithHotkey(new MenuEntry("Enable Lobby Emote Swapper", lobbyEmoteSwapper), {
            Hotkey(Key::DPadLeft, "Swap to original emote set"),
            Hotkey(Key::DPadRight, "Swap to alternative emote set")
            }));

        // *emotes += new MenuEntry("Use Custom Emotes", customEmotes);
    }

    void InitFreecamCodes(PluginMenu& menu)
    {
        // TODO: touchscreen-based controls
        menuFreecam = (EntryWithHotkey(new MenuEntry("Use Freecam button controls", useFreecam), {
                Hotkey(Key::L, "Enable/Disable Freecam"),
                Hotkey(Key::R, "Toggle camera lock"),
                Hotkey(Key::L | Key::R, "Reset camera"),
                Hotkey(Key::X, "Shift camera north"),
                Hotkey(Key::B, "Shift camera south"),
                Hotkey(Key::A, "Shift camera east"),
                Hotkey(Key::Y, "Shift camera west"),
                Hotkey(Key::DPadLeft, "Zoom in"),
                Hotkey(Key::DPadRight, "Zoom out"),
                Hotkey(Key::DPadUp, "Raise camera"),
                Hotkey(Key::DPadDown, "Lower camera"),
                Hotkey(Key::CPadDown, "Rotate counterclockwise (X-axis)"),
                Hotkey(Key::CPadUp, "Rotate clockwise (X-axis)"),
                Hotkey(Key::CPadLeft, "Rotate counterclockwise (Z-axis)"),
                Hotkey(Key::CPadRight, "Rotate clockwise (Z-axis)")
        }));

        editFreecamControls = new MenuEntry("Edit button controls", nullptr, editHotkeys);
        editFreecamSen = new MenuEntry("Edit sensitivity", nullptr, editSensitivity);
        swapZoom = new MenuEntry("Swap to Orthographic Zoom", nullptr, setZoomType);

        menu += menuFreecam;
        menu += editFreecamControls;
        menu += editFreecamSen;
        menu += swapZoom;
    }

    void InitColorFolder(PluginMenu& menu)
    {
        // presets?
        linkcolor = new MenuFolder("Custom Link Colors");

        *linkcolor += new MenuEntry("Set Green Link color", nullptr, customColor);
        *linkcolor += new MenuEntry("Set Blue Link color", nullptr, customColor);
        *linkcolor += new MenuEntry("Set Red Link color", nullptr, customColor);
        *linkcolor += new MenuEntry("Use re-colored level textures", recolorLevelTex);
    }

    void InitCostumeFolder(PluginMenu& menu)
    {
        costume = new MenuFolder("Costume Codes");

        triggerCostumeSlots = new MenuEntry("Open Custom Costume Slots", nullptr, openCustomCostumeSlots);
        menuCostumeSlotA = new MenuEntry("   Set custom costume slot A", nullptr, selectCostumeID, 
            "This setting is not accessible if Restore Great Fairy Costume is enabled.");
        menuCostumeSlotB = new MenuEntry("   Set custom costume slot B", nullptr, selectCostumeID);
        menuCostumeSlotC = new MenuEntry("   Set custom costume slot C", nullptr, selectCostumeID);
        menuCostumeSlotD = new MenuEntry("   Set custom costume slot D", nullptr, selectCostumeID);
        restoreGreatFairy = new MenuEntry("Restore Great Fairy Costume", greatFairyEnable);

        // add to costume folder + hide by default
        *costume += triggerCostumeSlots;
        *costume += menuCostumeSlotA;
        *costume += menuCostumeSlotB;
        *costume += menuCostumeSlotC;
        *costume += menuCostumeSlotD;
        *costume += restoreGreatFairy;

        menuCostumeSlotA->Hide();
        menuCostumeSlotB->Hide();
        menuCostumeSlotC->Hide();
        menuCostumeSlotD->Hide();

        *costume += new MenuEntry("Change Player Costume", nullptr, changeLinkCostume);

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

        // NOT added to main menu -- these are auto-managed by the plugin and don't need to be accessed by the user
        manageCatalogSize = new MenuEntry("Manage Catalog Size (auto)", manageCatalogSizeAuto);
        initCustomCostumes = new MenuEntry("Init Custom Costume list (auto)", initCustomCostumesAuto);
        writeCostumeIDToSlot = new MenuEntry("Write to costume slots (auto)", writeCostumeSlot);
    }

    void InitMiscFolder(PluginMenu& menu)
    {
        miscellaneous = new MenuFolder("Miscellaneous Codes");

        // broken...
        *miscellaneous += (EntryWithHotkey(new MenuEntry("Enable button spam", buttonSpammer, "When any of the selected keys are\npressed down, they will automatically spam.\nGood for in-game manuevers that require\nstrict timing of button input(s).\n\nDefault keys: A, B, X, Y, L, R."), {
            Hotkey(Key::A | Key::B | Key::X | Key::Y | Key::L | Key::R , "Button Spammer")
        }));
        
        *miscellaneous += new MenuEntry("Display photo on top screen", managePhotoDisp);
        *miscellaneous += new MenuEntry("Toggle sword beam cooldown", nullptr, selectLinkBeam);
        *miscellaneous += new MenuEntry("Force visibility of Treasure Chest contents", nullptr, seeChestContents);
        *miscellaneous += new MenuEntry("Force instant text boxes", nullptr, instantText);
      
        autoBeamCooldown = new MenuEntry("Set Beam Cooldown (auto)", setBeamCooldown);
    }

    void InitPlayerFolder(PluginMenu& menu)
    {
        player = new MenuFolder("Player Codes");
        // hotkeys -> posEditor (?)

        *player += new MenuEntry("Bypass Doppel Master dialogue cutscene", bypassDoppelDemo);
        *player += new MenuEntry("Display current respawn location", respawnIndicator);
        *player += new MenuEntry("Enable position editor menu", posEditor);
        *player += new MenuEntry("Disable Collision (experimental!)", nullptr, removeCollision);
        *player += new MenuEntry("Enable Water Storage", nullptr, waterStorage);
        *player += new MenuEntry("Spawn/Despawn Links", nullptr, spawnOpt);
        *player += new MenuEntry("Set Link model size", nullptr, linkSize);
        *player += new MenuEntry("Set Sword Types", nullptr, swordModelOpt);
        *player += new MenuEntry("Toggle Jinxed model", nullptr, jinxOpt);
        *player += new MenuEntry("Toggle Link Visibility", nullptr, visibilityOpt);
        *player += new MenuEntry("Toggle Link Invincibility", nullptr, invincibleOpt);
        *player += new MenuEntry("Toggle PvP damage", nullptr, pvpMode);

        managePlayerCodes = new MenuEntry("Set Player edits (auto)", setPlayerChanges);
    }

    void InitEnergyFolder(PluginMenu& menu)
    {
        energy = new MenuFolder("Energy Codes");

        *energy += new MenuEntry("Infinite energy", infEnergy);
        *energy += new MenuEntry("Set max energy amount", nullptr, maxEnergySet);
        *energy += new MenuEntry("Set energy consumption multiplier", nullptr, energyConsumeMultiplier);
    }

    void InitItemsFolder(PluginMenu& menu)
    {
        items = new MenuFolder("Item Codes");

        *items += new MenuEntry("Set current item", itemOpt);
        *items += new MenuEntry("Set Shadow Link item", shadowItemOpt);
        *items += new MenuEntry("Set strafing speeds", nullptr, strafingSpeedSet);
        *items += new MenuEntry("Always use upgraded Items", upgradeItemAlways);
    }

    void InitRenderFolder(PluginMenu& menu) 
    {
        render = new MenuFolder("Rendering Codes");

        *render += new MenuEntry("Hide HUD", triggerHideHUD);
        *render += new MenuEntry("Disable fog effects", disableFog);
        *render += new MenuEntry("Toggle scrolling messages", nullptr, disableScrollingText);
    }

    void InitSaveFolder(PluginMenu& menu)
    {
        save = new MenuFolder("Savefile Codes");

        *save += new MenuEntry("Use Voiceless", useVoiceless);
        *save += new MenuEntry("Set Main Voice", nullptr, mainVoice);
        *save += new MenuEntry("Set Hero Point count", nullptr, heroPointCountSet);
        *save += new MenuEntry("Set Coliseum Win count", nullptr, coliseumWinCountSet);

        MenuFolder* merchant = new MenuFolder("Street Merchant Codes");
            *merchant += new MenuEntry("Set 1st material slot", nullptr, merchantSlotA);
            *merchant += new MenuEntry("Set 2nd material slot", nullptr, merchantSlotB);
            *merchant += new MenuEntry("Set 3rd material slot", nullptr, merchantSlotC);
            *merchant += new MenuEntry("Set 4th material slot", nullptr, merchantSlotD);
            *merchant += new MenuEntry("Set 5th material slot", nullptr, merchantSlotE);
            *merchant += new MenuEntry("Force re-stock Street Merchant stall", nullptr, resetMerchant);

        *save += merchant;
    }

    void InitSoundFolder(PluginMenu& menu)
    {
        sound = new MenuFolder("BGM and SFX Codes");

        *sound += new MenuEntry("Set current BGM", bgmSet);
        *sound += new MenuEntry("Set BGM volume", nullptr, bgmVolSet);
        *sound += new MenuEntry("Choose Lobby Ball song", lobbyBallSong);
        //*sound += new MenuEntry("Set Lobby Ball volume", lobbyBallVol);
        *sound += new MenuEntry("Set Link Voice volume", nullptr, voiceVol);
        *sound += new MenuEntry("Set Low Health Alert volume", nullptr, lowHPVol);
        // *sound += new MenuEntry("Set Level Completion Fanfare volume", levelDoneVol);
    }

    void ToggleMenuChange(Time time)
    {
        if (PluginMenu::GetRunningInstance()->FreecamToggle) 
        {
            menuFreecam->Show();
            editFreecamControls->Show();
            editFreecamSen->Show();
            swapZoom->Show();

            costume->HideWithoutDisable();
            gameplay->HideWithoutDisable();
            emotes->HideWithoutDisable();
            linkcolor->HideWithoutDisable();
            miscellaneous->HideWithoutDisable();
            player->HideWithoutDisable();
            energy->HideWithoutDisable();
            items->HideWithoutDisable();
            render->HideWithoutDisable();
            save->HideWithoutDisable();
            sound->HideWithoutDisable();
        }
        else
        {          
            menuFreecam->HideWithoutDisable();
            editFreecamControls->HideWithoutDisable();
            editFreecamSen->HideWithoutDisable();
            swapZoom->HideWithoutDisable();

            if (!PluginMenu::GetRunningInstance()->GameplayToggle)
            {
                costume->Show();
                emotes->Show();
                linkcolor->Show();
                miscellaneous->Show();
                player->Show();
                energy->Show();
                items->Show();
                render->Show();
                save->Show();
                sound->Show();
            }
            else
            {
                menuFreecam->HideWithoutDisable();
                editFreecamControls->HideWithoutDisable();
                editFreecamSen->HideWithoutDisable();
                swapZoom->HideWithoutDisable();

                gameplay->Show();
                costume->HideWithoutDisable();
                emotes->HideWithoutDisable();
                linkcolor->HideWithoutDisable();
                miscellaneous->HideWithoutDisable();
                player->HideWithoutDisable();
                energy->HideWithoutDisable();
                items->HideWithoutDisable();
                render->HideWithoutDisable();
                save->HideWithoutDisable();
                sound->HideWithoutDisable();
            }
        }
    }

    // this function only runs once at plugin startup
    int main(void)
    {
        PluginMenu* menu = new PluginMenu("Tricord", 0, 5, 0,
            "An advanced, region-free cheat plugin made for\nThe Legend of Zelda: Tri Force Heroes.\n\nForked from the original CTRPluginFramework\nblank template repository.");

        menu->SynchronizeWithFrame(true);
        InitMenu(*menu);

        // this callback serves a secondary purpose
        // allows ANY menu-based change to render on new frame
        menu->OnNewFrame = ToggleMenuChange;

        // init auto functions
        // resetCostume->Enable();
        autoBeamCooldown->Enable();
        managePlayerCodes->Enable();

        menu->Run();

        // Exit plugin
        delete menu;
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
    void    OnProcessExit(void)
    {
        ToggleTouchscreenForceOn();
    }
}
