#include "Helpers.hpp"
#include "Cheats.hpp"
#include "csvc.h"
#include <3ds.h>

#include "CTRPluginFrameworkImpl/Menu/HotkeysModifier.hpp"

#include "CTRPluginFramework/Menu/MenuEntry.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryTools.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuHome.hpp"

#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuTools.hpp"

#include <CTRPluginFramework.hpp>

#include <vector>

namespace CTRPluginFramework {

    MenuFolder* emotes;
    MenuFolder* chaos;
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
        InitGameplayCodes(menu);
        InitEmoteFolder(menu);
        InitColorFolder(menu);
        InitCostumeFolder(menu);
        InitMiscFolder(menu);
        InitPlayerFolder(menu);
        InitItemsFolder(menu);
        InitRenderFolder(menu);
        InitSaveFolder(menu);
        InitSoundFolder(menu);

        AddAllFolders(menu);
    }

    void AddAllFolders(PluginMenu& menu)
    {
        menu += chaos;
        menu += costume;
        menu += emotes;
        // menu += linkcolor;
        menu += miscellaneous;
        menu += player;
        // menu += energy;
        menu += items;
        menu += render;
        menu += save;
        menu += sound;
    }

    void InitGameplayCodes(PluginMenu& menu)
    {
        chaos = new MenuFolder("Chaos Mode by Glem");
    }
    
    void InitEmoteFolder (PluginMenu& menu) 
    {
        emotes = new MenuFolder("Emote Codes");

        *emotes += (EntryWithHotkey(new MenuEntry("Enable Drablands Emote Swapper", Emotes::drablandEmoteSwapper), {
            Hotkey(Key::DPadLeft, "Swap to original emote set"),
            Hotkey(Key::DPadRight, "Swap to alternative emote set")
            }));

        *emotes += (EntryWithHotkey(new MenuEntry("Enable Lobby Emote Swapper", Emotes::lobbyEmoteSwapper), {
            Hotkey(Key::DPadLeft, "Swap to original emote set"),
            Hotkey(Key::DPadRight, "Swap to alternative emote set")
            }));

        // *emotes += new MenuEntry("Use Custom Emotes", customEmotes);
    }

    void InitFreecamCodes(PluginMenu& menu)
    {
        // TODO: touchscreen-based controls
        menuFreecam = (EntryWithHotkey(new MenuEntry("Use Freecam button controls", Freecam::useFreecam), {
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

        editFreecamControls = new MenuEntry("Edit button controls", nullptr, Freecam::editHotkeys);
        editFreecamSen = new MenuEntry("Edit sensitivity", nullptr, Freecam::editSensitivity);
        swapZoom = new MenuEntry("Swap to Orthographic Zoom", nullptr, Freecam::setZoomType);

        editFreecamControls->SetAltIcon(false);

        menu += menuFreecam;
        menu += editFreecamControls;
        menu += editFreecamSen;
        menu += swapZoom;
    }

    void InitColorFolder(PluginMenu& menu)
    {
        // presets?
        linkcolor = new MenuFolder("Custom Link Colors");

        *linkcolor += new MenuEntry("Set Green Link color", nullptr, LinkColor::customColor);
        *linkcolor += new MenuEntry("Set Blue Link color", nullptr, LinkColor::customColor);
        *linkcolor += new MenuEntry("Set Red Link color", nullptr, LinkColor::customColor);
        *linkcolor += new MenuEntry("Use re-colored level textures", LinkColor::recolorLevelTex);
    }

    void InitCostumeFolder(PluginMenu& menu)
    {
        costume = new MenuFolder("Costume Codes");

        triggerCostumeSlots = new MenuEntry("Open Custom Costume Slots", nullptr, Costume::openCustomCostumeSlots);
        menuCostumeSlotA = new MenuEntry("   Set custom costume slot A", nullptr, Costume::selectCostumeID, 
            "This setting is not accessible if Restore Great Fairy Costume is enabled.");
        menuCostumeSlotB = new MenuEntry("   Set custom costume slot B", nullptr, Costume::selectCostumeID);
        menuCostumeSlotC = new MenuEntry("   Set custom costume slot C", nullptr, Costume::selectCostumeID);
        menuCostumeSlotD = new MenuEntry("   Set custom costume slot D", nullptr, Costume::selectCostumeID);
        restoreGreatFairy = new MenuEntry("Restore Great Fairy Costume", Costume::greatFairyEnable);

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

        menuCostumeSlotA->SetAltIcon(true);
        menuCostumeSlotB->SetAltIcon(true);
        menuCostumeSlotC->SetAltIcon(true);
        menuCostumeSlotD->SetAltIcon(true);

        *costume += new MenuEntry("Change Player Costume", nullptr, Costume::changeLinkCostume);

        // create costume sub-folders
        MenuFolder* costumeEffects = new MenuFolder("Costume Effect(s)");
        *costumeEffects += new MenuEntry("(TODO) Set Costume Effects", nullptr, Costume::selCostumeEffect);

        MenuFolder* extraCustomConfig = new MenuFolder("Additional Customizations");
        // *extraCustomConfig += new MenuEntry("Set number of Tingle Balloons", nullptr, tingleBalloonNumber);
        // *extraCustomConfig += new MenuEntry("Set Cheetah walking speed", nullptr, cheetahSpeed);
        // *extraCustomConfig += new MenuEntry("Set luck percentage", nullptr, luckPercent);
        // *extraCustomConfig += new MenuEntry("Set number of additional Hearts", nullptr, addHeart);
        // *extraCustomConfig += new MenuEntry("Set sword beam type", nullptr, swordBeamConfig);

        *costume += costumeEffects;
        *extraCustomConfig += costumeEffects;

        // NOT added to main menu -- these are auto-managed by the plugin and don't need to be accessed by the user
        manageCatalogSize = new MenuEntry("Manage Catalog Size (auto)", Costume::manageCatalogSizeAuto);
        initCustomCostumes = new MenuEntry("Init Custom Costume list (auto)", Costume::initCustomCostumesAuto);
        writeCostumeIDToSlot = new MenuEntry("Write to costume slots (auto)", Costume::writeCostumeSlot);
    }

    void InitMiscFolder(PluginMenu& menu)
    {
        miscellaneous = new MenuFolder("Miscellaneous Codes");

        // broken...
        *miscellaneous += (EntryWithHotkey(new MenuEntry("(TODO) Enable button spam", Miscellaneous::buttonSpammer, "When any of the selected keys are\npressed down, they will automatically spam.\nGood for in-game manuevers that require\nstrict timing of button input(s).\n\nDefault keys: A, B, X, Y, L, R."), {
            Hotkey(Key::A | Key::B | Key::X | Key::Y | Key::L | Key::R , "Button Spammer")
        }));
        
        *miscellaneous += new MenuEntry("Use photo viewer touchscreen toggle", Miscellaneous::managePhotoDisp);
        *miscellaneous += new MenuEntry("Toggle sword beam cooldown", nullptr, Miscellaneous::selectLinkBeam);
        *miscellaneous += new MenuEntry("Force instant text boxes", nullptr, Miscellaneous::instantText);
      
        autoBeamCooldown = new MenuEntry("Set Beam Cooldown (auto)", Miscellaneous::setBeamCooldown);
    }

    void InitPlayerFolder(PluginMenu& menu)
    {
        player = new MenuFolder("Player Codes");

        *player += new MenuEntry("Bypass Doppel Master dialogue cutscene", Player::bypassDoppelDemo);
        *player += new MenuEntry("Enable position save/load touchscreen toggle", Player::posEditor);
        *player += new MenuEntry("Toggle collision edits (experimental!)", nullptr, Player::setCollisionChanges);
        *player += new MenuEntry("Toggle animation storage edits", nullptr, Player::setAnimStorageChanges);
        *player += new MenuEntry("Toggle player spawn edits", nullptr, Player::setSpawnChanges);
        *player += new MenuEntry("Toggle jinxed model edits", nullptr, Player::setJinxChanges);
        *player += new MenuEntry("Toggle player visibility edits", nullptr, Player::setVisibilityChanges);
        *player += new MenuEntry("Toggle player invincibility edits", nullptr, Player::setInvincibilityChanges);
        *player += new MenuEntry("Toggle PvP damage edits", nullptr, Player::setPVPChanges);
        *player += new MenuEntry("Set custom player model size", nullptr, Player::setSizeChanges);
        *player += new MenuEntry("Set custom sword model(s)", nullptr, Player::setSwordChanges);
   
        jinxEditAuto = new MenuEntry("Write Jinx edits (auto)", Player::writeJinxChanges);
        spawnEditAuto = new MenuEntry("Write Spawn edits (auto)", Player::writeSpawnChanges);
        visibleEditAuto = new MenuEntry("Write Visibility edits (auto)", Player::writeVisibilityChanges);
        invinciEditAuto = new MenuEntry("Write Invincibility edits (auto)", Player::writeInvincibilityChanges);
        pvpEditAuto = new MenuEntry("Write PVP edits (auto)", Player::writePVPChanges);
        animStoreEditAuto = new MenuEntry("Write Anim edits (auto)", Player::writeAnimStorageChanges);
        colEditAuto = new MenuEntry("Write Collision edits (auto)", Player::writeCollisionChanges);
        swordEditAuto = new MenuEntry("Write Sword edits (auto)", Player::writeSwordChanges);
        sizeEditAuto = new MenuEntry("Write Size edits (auto)", Player::writeSizeChanges);
    }

    void InitItemsFolder(PluginMenu& menu)
    {
        items = new MenuFolder("Item Codes");

        *items += new MenuEntry("Set current item", nullptr, Item::itemOpt);
        *items += new MenuEntry("Set Shadow Link item", nullptr, Item::shadowItemOpt);
        *items += new MenuEntry("Set strafing speeds", nullptr, Item::strafingSpeedSet);
        *items += new MenuEntry("Freeze current items", Item::freezeCurrItems);
        // TODO: *items += new MenuEntry("Always use upgraded Items", Item::upgradeItemAlways);
    }

    void InitRenderFolder(PluginMenu& menu) 
    {
        render = new MenuFolder("Rendering Codes");

        *render += new MenuEntry("Hide HUD", Rendering::triggerHideHUD);
        *render += new MenuEntry("Disable fog effects", Rendering::disableFog);
        *render += new MenuEntry("Display current respawn location", Rendering::respawnIndicator);
        *render += new MenuEntry("Disable top screen scrolling text", nullptr, Rendering::disableScrollingText);
        *render += new MenuEntry("Force visibility of Treasure Chest contents", nullptr, Rendering::seeChestContents);
    }

    void InitSaveFolder(PluginMenu& menu)
    {
        save = new MenuFolder("Savefile Codes");

        *save += new MenuEntry("Use Voiceless", Save::useVoiceless);
        *save += new MenuEntry("Set Main Voice", nullptr, Save::mainVoice);
        *save += new MenuEntry("Set Hero Point count", nullptr, Save::heroPointCountSet);
        *save += new MenuEntry("Set Coliseum Win count", nullptr, Save::coliseumWinCountSet);
        *save += new MenuEntry("Edit Level Completion", nullptr, Save::selLevelCompletion);


        MenuFolder* merchant = new MenuFolder("Street Merchant Codes");
            merchantA = new MenuEntry("Set 1st material slot", nullptr, Save::merchantSlotA);
            merchantB = new MenuEntry("Set 2nd material slot", nullptr, Save::merchantSlotB);
            merchantC = new MenuEntry("Set 3rd material slot", nullptr, Save::merchantSlotC);
            merchantD = new MenuEntry("Set 4th material slot", nullptr, Save::merchantSlotD);
            merchantE = new MenuEntry("Set 5th material slot", nullptr, Save::merchantSlotE);
            *merchant += new MenuEntry("Force re-stock Street Merchant stall", nullptr, Save::resetMerchant);

            merchantA->SetAltIcon(true);
            merchantB->SetAltIcon(true);
            merchantC->SetAltIcon(true);
            merchantD->SetAltIcon(true);
            merchantE->SetAltIcon(true);

            *merchant += merchantA;
            *merchant += merchantB;
            *merchant += merchantC;
            *merchant += merchantD;
            *merchant += merchantE;

        *save += merchant;
    }

    void InitSoundFolder(PluginMenu& menu)
    {
        sound = new MenuFolder("BGM and SFX Codes");

        *sound += new MenuEntry("Master volume: 100%", nullptr, BGM_SFX::masterVolSet);
        *sound += new MenuEntry("BGM volume: 100%", nullptr, BGM_SFX::bgmVolSet);
        //*sound += new MenuEntry("Override current BGM", nullptr, BGM_SFX::bgmSet);
        *sound += new MenuEntry("(TODO) Link Voice volume: 100%", nullptr, BGM_SFX::voiceVol);
        *sound += new MenuEntry("(TODO) Low Health Alert volume: 100%", nullptr, BGM_SFX::lowHPVol);
        //*sound += new MenuEntry("Set Lobby Ball volume", BGM_SFX::lobbyBallVol);
        *sound += new MenuEntry("Choose Lobby Ball song", nullptr, BGM_SFX::lobbyBallSong);
        // *sound += new MenuEntry("Set Level Completion Fanfare volume", BGM_SFX::levelDoneVol);

        lobbyBallAuto = new MenuEntry("Write lobby ball edits (auto)", BGM_SFX::writeLobbyBallSel);
    }

    void ToggleMenuChange(Time time)
    {
        if (PluginMenu::GetRunningInstance()->FreecamToggle)
        {
            
            menuFreecam->Show();
            editFreecamControls->Show();
            editFreecamSen->Show();
            swapZoom->Show();

            chaos->HideWithoutDisable();
            costume->HideWithoutDisable();
            emotes->HideWithoutDisable();
            //linkcolor->HideWithoutDisable();
            miscellaneous->HideWithoutDisable();
            player->HideWithoutDisable();
            //energy->HideWithoutDisable();
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
                chaos->Hide();
                costume->Show();
                emotes->Show();
                //linkcolor->Show();
                miscellaneous->Show();
                player->Show();
                //energy->Show();
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

                chaos->Show();
                costume->HideWithoutDisable();
                emotes->HideWithoutDisable();
                //linkcolor->HideWithoutDisable();
                miscellaneous->HideWithoutDisable();
                player->HideWithoutDisable();
                //energy->HideWithoutDisable();
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

        if (Preferences::IsEnabled(Preferences::HIDToggle))
            settings.UseGameHidMemory = true;

        settings.CachedDrawMode = true;

        ToggleTouchscreenForceOn();

    }

    // Called when the process exits
    void    OnProcessExit(void)
    {
        ToggleTouchscreenForceOn();
    }
}
