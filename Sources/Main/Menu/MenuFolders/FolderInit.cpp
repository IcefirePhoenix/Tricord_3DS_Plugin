#include "Cheats.hpp"
#include "Helpers/MenuHelpers/FolderEntries.hpp"

namespace CTRPluginFramework
{

    void InitGameModes(PluginMenu &menu)
    {
        chaos = new MenuFolder("Chaos Mode by Glem");
        turbo = new MenuFolder("Turbo Mode by Peanut");

        /* populate Chaos sub-folder */
        effects = new MenuFolder("Chaos Events");
        *effects += new MenuEntry("(TODO) View selected events");
        *effects += new MenuEntry("(TODO) Edit selected events");
        *effects += new MenuEntry("(TODO) Reset event selection");

        *chaos += effects;
        *chaos += new MenuEntry("(TODO) Select event trigger interval");

        *turbo += new MenuEntry("Enable Turbo Mode", nullptr, TurboMode::enableTurboMode);
    }

    void InitWarpCodes(PluginMenu& menu)
    {
        *warp += new MenuEntry("Warp to any location", nullptr, Gameplay::instantWarp, DescUtils::getDesc("warp_note"));
        *warp += new MenuEntry("Warp to any stage in current Drablands level", nullptr, Gameplay::stageWarp, DescUtils::getDesc("warp_stage_note"));

        reWarp = new MenuEntry("Return to previous warp: None", nullptr, Gameplay::warpAgain, DescUtils::getDesc("rewarp_note"));
        *warp += reWarp;

        *warp += (EntryWithHotkey(new MenuEntry("Reset the current area via hotkey", Gameplay::resetRoom, DescUtils::getDesc("reset_room_note")),
            { Hotkey(Key::L | Key::R, "Reset current area") }));
    }

    void InitHealthFairyCodes(PluginMenu &menu)
    {
        *healthFairies += new MenuEntry("Set maximum heart containers", nullptr, Gameplay::maxHealthSet, DescUtils::getDesc("max_heart_note"));
        *healthFairies += new MenuEntry("Infinite health", Gameplay::infHealth, DescUtils::getDesc("inf_heart_note"));
        *healthFairies += new MenuEntry("Infinite fairies", Gameplay::infFairy, DescUtils::getDesc("inf_fairy_note"));
    }

    void InitEnergyCodes(PluginMenu &menu)
    {
        *energy += new MenuEntry("Infinite energy", Gameplay::infEnergy, DescUtils::getDesc("inf_energy_note"));
        *energy += new MenuEntry("Set maximum energy", nullptr, Gameplay::maxEnergySet, DescUtils::getDesc("max_energy_note"));
        *energy += new MenuEntry("Set energy consumption multiplier", nullptr, Gameplay::energyConsumeMultiplier, DescUtils::getDesc("energy_consumption_note"));
        *energy += new MenuEntry("Use large energy gauge graphic", nullptr, Gameplay::useLargeEnergyGauge, DescUtils::getDesc("large_energy_bar_note"));
    }

    void InitEnemyCodes(PluginMenu &menu)
    {
        *enemies += (EntryWithHotkey(new MenuEntry("Insta-kill all spawned enemies", Gameplay::autoKillEnemy, DescUtils::getDesc("instakill_note")),
            { Hotkey(Key::L | Key::B, "Insta-kill all spawned enemies") }));

        *enemies += (EntryWithHotkey(new MenuEntry("Make all spawned enemies invincible", Gameplay::enemyInvinci, DescUtils::getDesc("invinci_note")),
            { Hotkey(Key::R | Key::B, "Make all spawned enemies invincible") }));
    }

    void InitPhysicsCodes(PluginMenu &menu)
    {
        physicsSelG = new MenuEntry("Player 1 (Green): Not edited", nullptr, Gameplay::changePhysics, DescUtils::getDesc("physics_note"));
        physicsSelB = new MenuEntry("Player 2 (Blue): Not edited", nullptr, Gameplay::changePhysics, DescUtils::getDesc("physics_note"));
        physicsSelR = new MenuEntry("Player 3 (Red): Not edited", nullptr, Gameplay::changePhysics, DescUtils::getDesc("physics_note"));

        physicsEditAutoG = new MenuEntry("Write physics edit G (auto)", Gameplay::writePhysicsChanges);
        physicsEditAutoB = new MenuEntry("Write physics edit B (auto)", Gameplay::writePhysicsChanges);
        physicsEditAutoR = new MenuEntry("Write physics edit R (auto)", Gameplay::writePhysicsChanges);

        MenuEntry *physicsSlots[6] =
        {
            physicsEditAutoG,
            physicsSelG,
            physicsEditAutoB,
            physicsSelB,
            physicsEditAutoR,
            physicsSelR
        };

        for (int iterator = 0; iterator < 6; iterator++)
        {
            // assign ArgIDs for EditAuto and Sel -> green = 0, blue = 1, red = 2
            // integer division always rounds up, so iterator / 2 should always result in 0-2 values
            physicsSlots[iterator]->SetArg(reinterpret_cast<void *>(iterator / 2));

            // add the physicsSel entries to the menu
            if (iterator % 2 == 1)
                *physics += physicsSlots[iterator];
        }
    }

    void InitMoonJumpCodes(PluginMenu &menu)
    {
        // Radio group 1 for mutual exclusivity
        moonJumpEntry = (EntryWithHotkey(new MenuEntry(1, "Enable Moon Jump", Gameplay::moonJumpAndFlight, DescUtils::getDesc("moon_jump_note")),
        {
            Hotkey(Key::X, "Ascend"),
            Hotkey(Key::CPad, "Move laterally") // Unused but necessary for entry description
        }));


        flightEntry = (EntryWithHotkey(new MenuEntry(1, "Enable Flight", Gameplay::moonJumpAndFlight, DescUtils::getDesc("flight_note")),
        {
            Hotkey(Key::X, "Ascend"),
            Hotkey(Key::B, "Descend"),
            Hotkey(Key::CPad, "Move laterally") // Unused but necessary for entry description
        }));

        *moonJumpFlight += moonJumpEntry;
        *moonJumpFlight += flightEntry;

        *moonJumpFlight += new MenuEntry("Keep uncontrolled players hovering in place", Gameplay::forceHover, DescUtils::getDesc("doppel_hover_note"));

        ascentSpeedSet = new MenuEntry("Adjust ascent speed: Medium", nullptr, Gameplay::selAscentDescentEdit, DescUtils::getDesc("ascent_note"));
        descentSpeedSet = new MenuEntry("Adjust descent speed: Medium", nullptr, Gameplay::selAscentDescentEdit, DescUtils::getDesc("descent_note"));
        ascentSpeedSet->SetArg(reinterpret_cast<void *>(1));
        descentSpeedSet->SetArg(reinterpret_cast<void *>(2));

        *moonJumpFlight += ascentSpeedSet;
        *moonJumpFlight += descentSpeedSet;

        *moonJumpFlight += new MenuEntry("Adjust lateral speed: Medium", nullptr, Gameplay::adjustLateralSpeed, DescUtils::getDesc("lateral_note"));
    }

    void InitGameplayFolder(PluginMenu &menu)
    {
        gameplay = new MenuFolder("Gameplay");
        warp = new MenuFolder("Instant Warp");
        healthFairies = new MenuFolder("Health & Fairies");
        energy = new MenuFolder("Energy");
        enemies = new MenuFolder("Enemies");
        physics = new MenuFolder("Ground Physics");
        moonJumpFlight = new MenuFolder("Moon Jump & Flight");

        InitWarpCodes(menu);
        InitHealthFairyCodes(menu);
        InitEnergyCodes(menu);
        InitEnemyCodes(menu);
        InitPhysicsCodes(menu);
        InitMoonJumpCodes(menu);

        *gameplay += warp;
        *gameplay += healthFairies;
        *gameplay += energy;
        *gameplay += enemies;
        *gameplay += physics;
        *gameplay += moonJumpFlight;

        *gameplay += new MenuEntry("Control all players", nullptr, Gameplay::controlAllPlayers, DescUtils::getDesc("control_all_note"));
        *gameplay += new MenuEntry("Infinite time", Gameplay::infTime, DescUtils::getDesc("inf_time_note"));
        *gameplay += new MenuEntry("Set custom movement speed", nullptr, Gameplay::customSpeed, DescUtils::getDesc("move_speed_note"));

        // auto-managed by plugin; hidden from users...
        controlAllAuto = new MenuEntry("Control all players (auto)", Gameplay::writePlayerControl);
        doppelEnableAuto = new MenuEntry("Mid-warp Doppel Enable (auto)", Gameplay::midWarpDoppelEnable);
        challengeEditAuto = new MenuEntry("Challenge ID edit (auto)", Gameplay::writeChallengeEdit);
        healthMaxAuto = new MenuEntry("Set max heart containers (auto)", Gameplay::writeMaxHealth);
    }

    void InitEmoteFolder(PluginMenu &menu)
    {
        emotes = new MenuFolder("Emotes");

        *emotes += (EntryWithHotkey(new MenuEntry("Enable Drablands Emote Swapper", Emotes::drablandEmoteSwapper, DescUtils::getDesc("drabland_emote_note")),
        {
            Hotkey(Key::DPadLeft, "Swap to original emote set"),
            Hotkey(Key::DPadRight, "Swap to alternative emote set")
        }));

        *emotes += (EntryWithHotkey(new MenuEntry("Enable Lobby Emote Swapper", Emotes::lobbyEmoteSwapper, DescUtils::getDesc("lobby_emote_note")),
        {
            Hotkey(Key::DPadLeft, "Swap to original emote set"),
            Hotkey(Key::DPadRight, "Swap to alternative emote set"),
            Hotkey(Key::DPadUp, "Use blank emote")
        }));
    }

    void InitFreecamCodes(PluginMenu &menu)
    {
        menuFreecam = (EntryWithHotkey(new MenuEntry("Use Freecam button controls", Freecam::useFreecam, DescUtils::getDesc("freecam_note")),
        {
            Hotkey(Key::L, "Enable/Disable Freecam"),
            Hotkey(Key::Start, "Toggle camera lock"),
            Hotkey(Key::R, "Reset camera"),
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
            Hotkey(Key::CPadLeft, "Rotate counterclockwise (Y-axis)"),
            Hotkey(Key::CPadRight, "Rotate clockwise (Y-axis)")
        }));


        editFreecamControls = new MenuEntry("Edit button controls", nullptr, Freecam::editHotkeys, DescUtils::getDesc("edit_freecam_hotkey_note"));
        editFreecamSen = new MenuEntry("Edit sensitivity", nullptr, Freecam::editSensitivity, DescUtils::getDesc("freecam_sensitivity_note"));
        swapZoom = new MenuEntry("Swap to Perspective Zoom", nullptr, Freecam::setZoomType, DescUtils::getDesc("zoom_swap_note"));

        editFreecamControls->SetAltIcon(false);

        menu += menuFreecam;
        menu += editFreecamControls;
        menu += editFreecamSen;
        menu += swapZoom;
    }

    void InitCostumeFolder(PluginMenu &menu)
    {
        costume = new MenuFolder("Costumes");
        MenuFolder *cosmetic = new MenuFolder("Cosmetic Costumes");

        *cosmetic += new MenuEntry("Enable Cosmetic Costumes", nullptr, Costume::enableCosmeticCostume, DescUtils::getDesc("choose_cosmetic_note"));
        *cosmetic += new MenuEntry("Set Cosmetic Costume", nullptr, Costume::setCosmeticCostume, DescUtils::getDesc("set_cosmetic_note"));
        *costume += cosmetic;

        // TODO: restore after custom costume model loaders are ready...
        triggerCostumeSlots = new MenuEntry("Open Custom Costume Slots", nullptr, Costume::openCustomCostumeSlots);
        menuCostumeSlotA = new MenuEntry("   Set custom costume slot A", nullptr, Costume::selectCostumeID, DescUtils::getDesc("slot_A_note"));
        menuCostumeSlotB = new MenuEntry("   Set custom costume slot B", nullptr, Costume::selectCostumeID);
        menuCostumeSlotC = new MenuEntry("   Set custom costume slot C", nullptr, Costume::selectCostumeID);
        menuCostumeSlotD = new MenuEntry("   Set custom costume slot D", nullptr, Costume::selectCostumeID);
        restoreGreatFairy = new MenuEntry("Restore Great Fairy Costume", Costume::greatFairyEnable, DescUtils::getDesc("restore_fairy_note"));

        // add to costume folder + hide by default
        // *costume += triggerCostumeSlots;
        // *costume += menuCostumeSlotA;
        // *costume += menuCostumeSlotB;
        // *costume += menuCostumeSlotC;
        // *costume += menuCostumeSlotD;
        *costume += restoreGreatFairy;

        menuCostumeSlotA->Hide();
        menuCostumeSlotB->Hide();
        menuCostumeSlotC->Hide();
        menuCostumeSlotD->Hide();

        menuCostumeSlotA->SetAltIcon(true);
        menuCostumeSlotB->SetAltIcon(true);
        menuCostumeSlotC->SetAltIcon(true);
        menuCostumeSlotD->SetAltIcon(true);

        *costume += new MenuEntry("Change Player Costume", nullptr, Costume::changeLinkCostume, DescUtils::getDesc("change_costume_note"));
        *costume += new MenuEntry("Prevent Doppel Costume resets", nullptr, Costume::preventDoppelLobbyReset, DescUtils::getDesc("prevent_doppel_reset_note"));
        *costume += new MenuEntry("Set custom sword model(s)", nullptr, Player::setSwordChanges, DescUtils::getDesc("sword_model_note"));
        *costume += new MenuEntry("Force Bear Minimum / Maximum", nullptr, Costume::forceBearMinMax, DescUtils::getDesc("bear_minMax_note"));

        // create costume sub-folders...
        // TODO: need to merge fixes from test branch...
        // MenuFolder *costumeEffects = new MenuFolder("Costume Effects");
        // *costumeEffects += new MenuEntry("(TODO) Set Costume Effects", nullptr, Costume::selCostumeEffect);
        // *costume += costumeEffects;

        *costume += new MenuEntry("Costume Randomizers", nullptr, Costume::costumeRandomizer, DescUtils::getDesc("costume_randomizers_note"));

        // auto-managed by plugin; hidden from users...
        swordEditAuto = new MenuEntry("Write Sword edits (auto)", Player::writeSwordChanges);
        cosmeticCostumeAuto = new MenuEntry("Write cosmetic costumes (auto)", Costume::writeCosmeticCostume);
        manageCatalogSize = new MenuEntry("Manage Catalog Size (auto)", Costume::manageCatalogSizeAuto);
        initCustomCostumes = new MenuEntry("Init Custom Costume list (auto)", Costume::initCustomCostumesAuto);
        writeCostumeIDToSlot = new MenuEntry("Write to costume slots (auto)", Costume::writeToCostumeSlot);
        costumeRandomizerAuto = new MenuEntry("Costume Randomizers (auto)", Costume::writeRandomCostume);

    }

    void InitMiscFolder(PluginMenu &menu)
    {
        miscellaneous = new MenuFolder("Miscellaneous");
        MenuFolder *buttonSpam = new MenuFolder("Button Spammer");
        MenuFolder *camera = new MenuFolder("Screenshot");

        *buttonSpam += new MenuEntry("Enable button spam", Miscellaneous::buttonSpammer, DescUtils::getDesc("button_spam_note"));
        *buttonSpam += new MenuEntry("Set custom timer interval: 10 ms", nullptr, Miscellaneous::selSpamInterval, DescUtils::getDesc("set_spam_interval_note"));
        *buttonSpam += new MenuEntry("Set button spammer keys", nullptr, Miscellaneous::selectSpamKeys, DescUtils::getDesc("set_spam_button_note"));

        *camera += new MenuEntry("Toggle camera on X button: No edits", nullptr, Miscellaneous::toggleCameraButton, DescUtils::getDesc("toggle_X_cam_note"));
        *camera += new MenuEntry("Disable camera shutter", nullptr, Miscellaneous::toggleCameraShutter, DescUtils::getDesc("shutter_note"));
        *camera += new MenuEntry("Use photo viewer touchscreen toggle", Miscellaneous::managePhotoDisp, DescUtils::getDesc("viewer_toggle_note"));

        *miscellaneous += buttonSpam;
        *miscellaneous += camera;

        *miscellaneous += new MenuEntry("Toggle sword beam cooldown", nullptr, Miscellaneous::selectLinkBeam, DescUtils::getDesc("beam_cooldown_note"));
        *miscellaneous += new MenuEntry("Force instant text boxes", nullptr, Miscellaneous::manageInstantText, DescUtils::getDesc("instant_text_note"));

        // auto-managed by plugin; hidden from users...
        autoWriteCameraStatus = new MenuEntry("Toggle camera status (auto)", Miscellaneous::keepCameraEdits);
        autoDisableCamShutter = new MenuEntry("Disable camera shutter (auto)", Miscellaneous::writeShutterDisable);
        autoBeamCooldown = new MenuEntry("Set Beam Cooldown (auto)", Miscellaneous::setBeamCooldown);
    }

    void InitPlayerFolder(PluginMenu &menu)
    {
        player = new MenuFolder("Player");

        *player += new MenuEntry("Bypass Doppel Master dialogue cutscene", Player::bypassDoppelDemo, DescUtils::getDesc("bypass_doppel_note"));
        *player += new MenuEntry("Enable position save/load touchscreen toggle", Player::posEditor, DescUtils::getDesc("save_load_pos_note"));

        *player += (EntryWithHotkey(new MenuEntry("Bring Doppels to your position via hotkey", Player::doppelsToPlayer, DescUtils::getDesc("doppel_summon_note")),
                                    {Hotkey(Key::X | Key::DPadDown, "Bring Doppels to you")}));

        *player += new MenuEntry("Toggle collision edits (experimental!)", nullptr, Player::setCollisionChanges, DescUtils::getDesc("collision_note"));
        *player += new MenuEntry("Toggle animation storage edits", nullptr, Player::setAnimStorageChanges, DescUtils::getDesc("anim_storage_note"));
        *player += new MenuEntry("Toggle player spawn edits", nullptr, Player::setSpawnChanges, DescUtils::getDesc("spawn_note"));
        *player += new MenuEntry("Toggle jinxed model edits", nullptr, Player::setJinxChanges, DescUtils::getDesc("jinxed_note"));
        *player += new MenuEntry("Toggle player visibility edits", nullptr, Player::setVisibilityChanges, DescUtils::getDesc("visible_note"));
        *player += new MenuEntry("Toggle player invincibility edits", nullptr, Player::setInvincibilityChanges, DescUtils::getDesc("player_invinci_note"));
        *player += new MenuEntry("Toggle PvP damage edits", nullptr, Player::setPVPChanges, DescUtils::getDesc("pvp_note"));
        *player += new MenuEntry("Set custom player model size", nullptr, Player::setSizeChanges, DescUtils::getDesc("player_size_note"));

        // auto-managed by plugin; hidden from users...
        jinxEditAuto = new MenuEntry("Write Jinx edits (auto)", Player::writeJinxChanges);
        spawnEditAuto = new MenuEntry("Write Spawn edits (auto)", Player::writeSpawnChanges);
        visibleEditAuto = new MenuEntry("Write Visibility edits (auto)", Player::writeVisibilityChanges);
        invinciEditAuto = new MenuEntry("Write Invincibility edits (auto)", Player::writeInvincibilityChanges);
        pvpEditAuto = new MenuEntry("Write PVP edits (auto)", Player::writePVPChanges);
        animStoreEditAuto = new MenuEntry("Write Anim edits (auto)", Player::writeAnimStorageChanges);
        colEditAuto = new MenuEntry("Write Collision edits (auto)", Player::writeCollisionChanges);
    }

    void InitItemsFolder(PluginMenu &menu)
    {
        items = new MenuFolder("Items");
        MenuFolder *playerItem = new MenuFolder("Player Items");
        MenuFolder *shadowItem = new MenuFolder("Shadow Link Items");

        *playerItem += new MenuEntry("Set current item", nullptr, Item::itemOpt, DescUtils::getDesc("curr_item_note"));
        *playerItem += new MenuEntry("Set strafing speeds", nullptr, Item::strafingSpeedSet, DescUtils::getDesc("strafe_note"));
        *playerItem += new MenuEntry("Remove current items", nullptr, Item::manageItems, DescUtils::getDesc("remove_item_note"));
        *playerItem += new MenuEntry("Freeze current items", Item::manageItems, DescUtils::getDesc("freeze_item_note"));

        forceShadowSwordOnly = new MenuEntry("Disable Shadow Link item usage", Item::toggleShadowForceSwordUse, DescUtils::getDesc("shadow_link_disable_item_note"));
        *shadowItem += forceShadowSwordOnly;

        *shadowItem += new MenuEntry("Set Shadow Link item", nullptr, Item::shadowItemOpt, DescUtils::getDesc("shadow_link_note"));
        *shadowItem += new MenuEntry("Reset Shadow Link items", nullptr, Item::resetShadowItems, DescUtils::getDesc("shadow_link_reset_note"));

        *items += playerItem;
        *items += shadowItem;

        // TODO: *items += new MenuEntry("Always use upgraded Items", Item::upgradeItemAlways);
    }

    void InitRenderFolder(PluginMenu &menu)
    {
        render = new MenuFolder("Rendering");

        MenuFolder *overlay = new MenuFolder("Overlays");
        MenuFolder *texture = new MenuFolder("Textures");
        MenuFolder *env = new MenuFolder("Environment & Other");
        MenuFolder *face = new MenuFolder("Facial Expressions");
        MenuFolder *scrollText = new MenuFolder("Top Screen Scrolling Messages");

        *scrollText += new MenuEntry("Edit top screen scrolling text colors", nullptr, Rendering::editLiveMsgColor, DescUtils::getDesc("scroll_text_color_note")); // TODO: settings
        *scrollText += new MenuEntry("Disable top screen scrolling text", nullptr, Rendering::disableScrollingText, DescUtils::getDesc("disable_scroll_text_note"));
        *overlay += scrollText;

        *overlay += new MenuEntry("Hide HUD", Rendering::triggerHideHUD, DescUtils::getDesc("hide_HUD_note"));
        *overlay += new MenuEntry("Display current respawn location", Rendering::respawnIndicator, DescUtils::getDesc("respawn_locator_note"));
        *overlay += new MenuEntry("Force visibility of Treasure Chest contents", nullptr, Rendering::seeChestContents, DescUtils::getDesc("chest_visible_note"));
        *overlay += new MenuEntry("Swap single player loading screen", nullptr, Rendering::swapSPLoadingScreen, DescUtils::getDesc("load_sp_screen_note"));

        // *face += new MenuEntry("Enable custom facial expressions", FaceSelMenu::maintainEdits, DescUtils::getDesc("face_expr_enable_note"));
        // *face += new MenuEntry("Edit idle facial expression", nullptr, Rendering::editFaceExpr, DescUtils::getDesc("face_expr_sel_note"));
        *texture += face;

        *texture += new MenuEntry("Swap Link textures", nullptr, Rendering::swapLinkTexture, DescUtils::getDesc("swap_link_tex_note"));
        *texture += new MenuEntry("Force visibility of Cheer Outfit pom poms", nullptr, Rendering::forcePomPom, DescUtils::getDesc("cheer_pom_note"));

        *env += new MenuEntry("Disable fog effects", Rendering::disableFog, DescUtils::getDesc("fog_disable_note"));
        *env += new MenuEntry("Force costume aura visibility", nullptr, Rendering::forceAura, DescUtils::getDesc("aura_visible_note"));
        *env += new MenuEntry("Force Sword Suit blue sword particles", nullptr, Rendering::forceBlueSwordParticles, DescUtils::getDesc("sword_ptcl_note"));
        *env += new MenuEntry("Disable player light sources in dark stages", nullptr, Rendering::disablePlayerLight, DescUtils::getDesc("disable_light_source_note"));

        *render += overlay;
        *render += texture;
        *render += env;
    }

    void InitSaveFolder(PluginMenu &menu)
    {
        save = new MenuFolder("Save Data Editors");

        *save += new MenuEntry("Use Voiceless", Save::useVoiceless, DescUtils::getDesc("voiceless_note"));
        *save += new MenuEntry("Set Main Voice", nullptr, Save::mainVoice, DescUtils::getDesc("main_voice_note"));
        *save += new MenuEntry("Set Hero Point count", nullptr, Save::heroPointCountSet, DescUtils::getDesc("hero_point_note"));
        *save += new MenuEntry("Set Coliseum Win count", nullptr, Save::coliseumWinCountSet, DescUtils::getDesc("coliseum_win_note"));
        *save += new MenuEntry("Edit Level Completion", nullptr, Save::selLevelCompletion, DescUtils::getDesc("level_completion_note"));

        MenuFolder *merchant = new MenuFolder("Street Merchant Codes");
        merchantA = new MenuEntry("Set 1st material slot", nullptr, Save::selMerchantSlot, DescUtils::getDesc("merchant_mat_note"));
        merchantB = new MenuEntry("Set 2nd material slot", nullptr, Save::selMerchantSlot, DescUtils::getDesc("merchant_mat_note"));
        merchantC = new MenuEntry("Set 3rd material slot", nullptr, Save::selMerchantSlot, DescUtils::getDesc("merchant_mat_note"));
        merchantD = new MenuEntry("Set 4th material slot", nullptr, Save::selMerchantSlot, DescUtils::getDesc("merchant_mat_note"));
        merchantE = new MenuEntry("Set 5th material slot", nullptr, Save::selMerchantSlot, DescUtils::getDesc("merchant_mat_note"));
        *merchant += new MenuEntry("Force re-stock Street Merchant stall", nullptr, Save::resetMerchant, DescUtils::getDesc("merchant_restock_note"));

        MenuEntry *slots[5] =
        {
            merchantA,
            merchantB,
            merchantC,
            merchantD,
            merchantE
        };

        for (int iterator = 0; iterator < 5; iterator++)
        {
            slots[iterator]->SetAltIcon(true);                               // remove icon
            slots[iterator]->SetArg(reinterpret_cast<void *>(iterator * 2)); // slot IDs are even numbers
            *merchant += slots[iterator];
        }

        *save += merchant;
    }

    void InitSoundFolder(PluginMenu &menu)
    {
        sound = new MenuFolder("BGM & SFX");

        // incomplete -- messy for now...
        masterVol = new MenuEntry("Master volume: 100%", nullptr, BGM_SFX::volSet, DescUtils::getDesc("master_vol_note"));
        BGMVol = new MenuEntry("BGM volume: 100%", nullptr, BGM_SFX::volSet, DescUtils::getDesc("bgm_vol_note"));
        //*sound += new MenuEntry("Override current BGM", nullptr, BGM_SFX::bgmSet);
        voiceVol = new MenuEntry("(TODO) Link Voice volume: 100%", nullptr, BGM_SFX::volSet, DescUtils::getDesc("voice_vol_note"));
        lowHPVol = new MenuEntry("(TODO) Low Health Alert volume: 100%", nullptr, BGM_SFX::volSet, DescUtils::getDesc("low_HP_vol_note"));
        // emoteVol += new MenuEntry("(TODO) Emote volume: 100%", nullptr, BGM_SFX::?);
        //*sound += new MenuEntry("Set Lobby Ball volume", BGM_SFX::?);
        //*sound += new MenuEntry("Set Lobby Ball counter", BGM_SFX::?);
        *sound += new MenuEntry("Choose Lobby Ball song", nullptr, BGM_SFX::lobbyBallSong, DescUtils::getDesc("lobby_ball_song"));

        MenuEntry *soundEntries[4] =
        {
            masterVol,
            BGMVol,
            voiceVol,
            lowHPVol,
            // emoteVol
        };

        for (int iterator = 0; iterator < 4; iterator++)
        {
            soundEntries[iterator]->SetArg(reinterpret_cast<void *>(iterator)); // store entry IDs
            *sound += soundEntries[iterator];
        }

        *sound += new MenuEntry("Force normal or 8-bit BGM", nullptr, BGM_SFX::forceNormal8bit, DescUtils::getDesc("8bit_note"));
        *sound += new MenuEntry("Disable Timeless Tunic voice filter", nullptr, BGM_SFX::disable8bitVoice, DescUtils::getDesc("disable_timeless_voice_note"));
        *sound += new MenuEntry("Force 8-bit sword SFX", nullptr, BGM_SFX::force8bitSword, DescUtils::getDesc("sword_8bit_note"));

        // auto-managed by plugin; hidden from users...
        lobbyBallAuto = new MenuEntry("Write lobby ball edits (auto)", BGM_SFX::writeLobbyBallSel);
    }

    void InitTFH_Settings(PluginMenu &menu)
    {
        pretendoOnlinePatchManager = new MenuEntry("Pretendo Patch (auto)", Miscellaneous::applyPretendoPatch);
        rotationOffsetManager = new MenuEntry("Disable rotation offset (auto)", Player::disableOffset);
        doppelCostumeStageResetManager = new MenuEntry("Disable Doppel costume stage reset (auto)", Costume::preventDoppelStageReset);
    }
}