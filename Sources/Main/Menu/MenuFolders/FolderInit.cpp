#include "Helpers/MenuHelper.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    static MenuEntry *EntryWithHotkey(MenuEntry *entry, const std::vector<Hotkey> &hotkeys)
    {
        if (entry != nullptr)
        {
            for (const Hotkey &hotkey : hotkeys)
                entry->Hotkeys += hotkey;
        }
        return entry;
    }

    void InitGameModes(PluginMenu &menu)
    {
        /* first custom game mode */
        effects = new MenuFolder("Chaos Events");

        *effects += new MenuEntry("(TODO) View selected events");
        *effects += new MenuEntry("(TODO) Edit selected events");
        *effects += new MenuEntry("(TODO) Reset event selection");

        chaos = new MenuFolder("Chaos Mode by Glem");
        *chaos += effects;
        *chaos += new MenuEntry("(TODO) Select event trigger interval");

        /* second custom game mode */
        turbo = new MenuFolder("Turbo Mode by Peanut");
        *turbo += new MenuEntry("Enable Turbo Mode", nullptr, TurboMode::enableTurboMode);
    }

    void InitWarpCodes(PluginMenu &menu)
    {
        *warp += new MenuEntry("Warp to any location", nullptr, Gameplay::instantWarp);
        *warp += new MenuEntry("Warp to any stage in current Drablands level", nullptr, Gameplay::stageWarp);

        // needs to be defined globally to allow entry name edits during runtime...
        reWarp = new MenuEntry("Return to previous warp: None", nullptr, Gameplay::warpAgain);
        *warp += reWarp;

        *warp += (EntryWithHotkey(new MenuEntry("Reset the current area (trigger via hotkey)", Gameplay::resetRoom),
            { Hotkey(Key::L | Key::R, "Reset current area") }));
    }

    void InitHealthFairyCodes(PluginMenu &menu)
    {
        std::string maxHeartNote = "Affects your base heart containers. Costume effects that add or remove heart containers still apply.";
        std::string infHealthNote = "Freezes your health to match your maximum heart containers. May not be consistent if a costume that adds or removes heart containers is worn.";

        *healthFairies += new MenuEntry("Set maximum heart containers", nullptr, Gameplay::maxHealthSet, maxHeartNote);
        *healthFairies += new MenuEntry("Infinite health", Gameplay::infHealth, infHealthNote);
        *healthFairies += new MenuEntry("Infinite fairies", Gameplay::infFairy);
    }

    void InitEnergyCodes(PluginMenu &menu)
    {
        std::string largeEnergyNote = "Switches the energy gauge in the UI to the Energy Gear / Cheer Outfit / Tri Suit energy gauge. Does NOT affect your max energy.";

        *energy += new MenuEntry("Infinite energy", Gameplay::infEnergy);
        *energy += new MenuEntry("Set maximum energy", nullptr, Gameplay::maxEnergySet);
        *energy += new MenuEntry("Set energy consumption multiplier", nullptr, Gameplay::energyConsumeMultiplier);
        *energy += new MenuEntry("Use large energy gauge graphic", nullptr, Gameplay::useLargeEnergyGauge, largeEnergyNote);
    }

    void InitEnemyCodes(PluginMenu &menu)
    {
        *enemies += (EntryWithHotkey(new MenuEntry("Insta-kill all spawned enemies", Gameplay::autoKillEnemy),
            { Hotkey(Key::L | Key::B, "Insta-kill all spawned enemies") }));

        *enemies += (EntryWithHotkey(new MenuEntry("Make all spawned enemies invincible", Gameplay::enemyInvinci),
            { Hotkey(Key::R | Key::B, "Make all spawned enemies invincible") }));
    }

    void InitPhysicsCodes(PluginMenu &menu)
    {
        physicsSelG = new MenuEntry("Player 1 (Green): Not edited", nullptr, Gameplay::changePhysics);
        physicsSelB = new MenuEntry("Player 2 (Blue): Not edited", nullptr, Gameplay::changePhysics);
        physicsSelR = new MenuEntry("Player 3 (Red): Not edited", nullptr, Gameplay::changePhysics);

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
        moonJumpEntry = (EntryWithHotkey(new MenuEntry(1, "Enable Moon Jump", Gameplay::moonJumpAndFlight),
        {
            Hotkey(Key::CPadUp, "Move North"),
            Hotkey(Key::CPadDown, "Move South"),
            Hotkey(Key::CPadRight, "Move East"),
            Hotkey(Key::CPadLeft, "Move West"),
            Hotkey(Key::X, "Ascend")
        }));


        flightEntry = (EntryWithHotkey(new MenuEntry(1, "Enable Flight", Gameplay::moonJumpAndFlight),
        {
            Hotkey(Key::CPadUp, "Move North"),
            Hotkey(Key::CPadDown, "Move South"),
            Hotkey(Key::CPadRight, "Move East"),
            Hotkey(Key::CPadLeft, "Move West"),
            Hotkey(Key::X, "Ascend"),
            Hotkey(Key::B, "Descend")
        }));

        *moonJumpFlight += moonJumpEntry;
        *moonJumpFlight += flightEntry;

        *moonJumpFlight += new MenuEntry("Keep uncontrolled players hovering in place", Gameplay::forceHover);

        ascentSpeedSet = new MenuEntry("Adjust ascent speed: Medium", nullptr, Gameplay::selAscentDescentEdit);
        descentSpeedSet = new MenuEntry("Adjust descent speed: Medium", nullptr, Gameplay::selAscentDescentEdit);
        ascentSpeedSet->SetArg(reinterpret_cast<void *>(1));
        descentSpeedSet->SetArg(reinterpret_cast<void *>(2));

        *moonJumpFlight += ascentSpeedSet;
        *moonJumpFlight += descentSpeedSet;

        *moonJumpFlight += new MenuEntry("Adjust lateral speed: Medium", nullptr, Gameplay::adjustLateralSpeed);
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

        std::string moveSpeedNote = "Work in progress. Currently only affects walking speed.";

        *gameplay += new MenuEntry("Control all players", nullptr, Gameplay::controlAllPlayers);
        *gameplay += new MenuEntry("Infinite time", Gameplay::infTime);
        *gameplay += new MenuEntry("Set custom movement speed", nullptr, Gameplay::customSpeed, moveSpeedNote);

        // auto-managed by plugin; hidden from users...
        controlAllAuto = new MenuEntry("Control all players (auto)", Gameplay::writePlayerControl);
        doppelEnableAuto = new MenuEntry("Mid-warp Doppel Enable (auto)", Gameplay::midWarpDoppelEnable);
        challengeEditAuto = new MenuEntry("Challenge ID edit (auto)", Gameplay::writeChallengeEdit);
        healthMaxAuto = new MenuEntry("Set max heart containers (auto)", Gameplay::writeMaxHealth);
    }

    void InitEmoteFolder(PluginMenu &menu)
    {
        emotes = new MenuFolder("Emotes");

        *emotes += (EntryWithHotkey(new MenuEntry("Enable Drablands Emote Swapper", Emotes::drablandEmoteSwapper),
        {
            Hotkey(Key::DPadLeft, "Swap to original emote set"),
            Hotkey(Key::DPadRight, "Swap to alternative emote set")
        }));

        *emotes += (EntryWithHotkey(new MenuEntry("Enable Lobby Emote Swapper", Emotes::lobbyEmoteSwapper),
        {
            Hotkey(Key::DPadLeft, "Swap to original emote set"),
            Hotkey(Key::DPadRight, "Swap to alternative emote set"),
            Hotkey(Key::DPadUp, "Use blank emote")
        }));
    }

    void InitFreecamCodes(PluginMenu &menu)
    {
        menuFreecam = (EntryWithHotkey(new MenuEntry("Use Freecam button controls", Freecam::useFreecam),
        {
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

    void InitCostumeFolder(PluginMenu &menu)
    {
        costume = new MenuFolder("Costumes");
        MenuFolder *cosmetic = new MenuFolder("Cosmetic Costumes");

        std::string slotANote = "This setting is not accessible if Restore Great Fairy Costume is enabled.";
        std::string preventDoppelResetNote = "Determines whether Doppel costumes reset to the Hero's Tunic when entering or returning to the single player lobby.";
        std::string cosmeticNote = "Determines whether choices under Set Cosmetic Costume get applied or not.";
        std::string cosmeticUsageNote = "Select a costume to gain the cosmetics of without changing the effects of the currently worn costume hiding underneath. Any usages of the \"Change Player Costume\" code or normal in-game costume changes after activating this will only change your costume's effect.";

        *cosmetic += new MenuEntry("Enable Cosmetic Costumes", nullptr, Costume::enableCosmeticCostume, cosmeticNote);
        *cosmetic += new MenuEntry("Set Cosmetic Costume", nullptr, Costume::setCosmeticCostume, cosmeticUsageNote);
        *costume += cosmetic;

        triggerCostumeSlots = new MenuEntry("Open Custom Costume Slots", nullptr, Costume::openCustomCostumeSlots);
        menuCostumeSlotA = new MenuEntry("   Set custom costume slot A", nullptr, Costume::selectCostumeID, slotANote);
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
        *costume += new MenuEntry("Prevent Doppel Costume resets", nullptr, Costume::preventLobbyReset, preventDoppelResetNote);
        *costume += new MenuEntry("Set custom sword model(s)", nullptr, Player::setSwordChanges);
        *costume += new MenuEntry("Force Bear Minimum / Maximum", nullptr, Costume::forceBearMinMax);

        // create costume sub-folders...
        // TODO: need to merge fixes from test branch...
        // MenuFolder *costumeEffects = new MenuFolder("Costume Effects");
        // *costumeEffects += new MenuEntry("(TODO) Set Costume Effects", nullptr, Costume::selCostumeEffect);
        // *costume += costumeEffects;

        // auto-managed by plugin; hidden from users...
        swordEditAuto = new MenuEntry("Write Sword edits (auto)", Player::writeSwordChanges);
        writeCosmeticCostumeID = new MenuEntry("Write cosmetic costumes (auto)", Costume::writeCosmeticCostume);
        manageCatalogSize = new MenuEntry("Manage Catalog Size (auto)", Costume::manageCatalogSizeAuto);
        initCustomCostumes = new MenuEntry("Init Custom Costume list (auto)", Costume::initCustomCostumesAuto);
        writeCostumeIDToSlot = new MenuEntry("Write to costume slots (auto)", Costume::writeToCostumeSlot);

    }

    void InitMiscFolder(PluginMenu &menu)
    {
        miscellaneous = new MenuFolder("Miscellaneous");
        MenuFolder *buttonSpam = new MenuFolder("Button Spammer");
        MenuFolder *camera = new MenuFolder("Screenshot");

        *buttonSpam += new MenuEntry("Enable button spam", Miscellaneous::buttonSpammer);
        *buttonSpam += new MenuEntry("Set custom timer interval: 10 ms", nullptr, Miscellaneous::selSpamInterval);
        *buttonSpam += new MenuEntry("Set button spammer keys", nullptr, Miscellaneous::selectSpamKeys);

        *camera += new MenuEntry("Toggle camera on X button: No edits", nullptr, Miscellaneous::toggleCameraButton);
        *camera += new MenuEntry("Disable camera shutter", nullptr, Miscellaneous::toggleCameraShutter);
        *camera += new MenuEntry("Use photo viewer touchscreen toggle", Miscellaneous::managePhotoDisp);

        *miscellaneous += buttonSpam;
        *miscellaneous += camera;

        *miscellaneous += new MenuEntry("Toggle sword beam cooldown", nullptr, Miscellaneous::selectLinkBeam);
        *miscellaneous += new MenuEntry("Force instant text boxes", nullptr, Miscellaneous::manageInstantText);

        // auto-managed by plugin; hidden from users...
        autoWriteCameraStatus = new MenuEntry("Toggle camera status (auto)", Miscellaneous::writeCameraEdits);
        autoDisableCamShutter = new MenuEntry("Disable camera shutter (auto)", Miscellaneous::writeShutterDisable);
        autoBeamCooldown = new MenuEntry("Set Beam Cooldown (auto)", Miscellaneous::setBeamCooldown);
    }

    void InitPlayerFolder(PluginMenu &menu)
    {
        player = new MenuFolder("Player");

        *player += new MenuEntry("Bypass Doppel Master dialogue cutscene", Player::bypassDoppelDemo);
        *player += new MenuEntry("Enable position save/load touchscreen toggle", Player::posEditor);

        *player += (EntryWithHotkey(new MenuEntry("Bring Doppels to your position via hotkey", Player::doppelsToPlayer),
            { Hotkey(Key::X | Key::DPadDown, "Bring Doppels to you") }));

        *player += new MenuEntry("Toggle collision edits (experimental!)", nullptr, Player::setCollisionChanges);
        *player += new MenuEntry("Toggle animation storage edits", nullptr, Player::setAnimStorageChanges);
        *player += new MenuEntry("Toggle player spawn edits", nullptr, Player::setSpawnChanges);
        *player += new MenuEntry("Toggle jinxed model edits", nullptr, Player::setJinxChanges);
        *player += new MenuEntry("Toggle player visibility edits", nullptr, Player::setVisibilityChanges);
        *player += new MenuEntry("Toggle player invincibility edits", nullptr, Player::setInvincibilityChanges);
        *player += new MenuEntry("Toggle PvP damage edits", nullptr, Player::setPVPChanges);
        *player += new MenuEntry("Set custom player model size", nullptr, Player::setSizeChanges);

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

        *items += new MenuEntry("Set current item", nullptr, Item::itemOpt);
        *items += new MenuEntry("Set Shadow Link item", nullptr, Item::shadowItemOpt);
        *items += new MenuEntry("Set strafing speeds", nullptr, Item::strafingSpeedSet);
        *items += new MenuEntry("Remove current items", nullptr, Item::manageItems);
        *items += new MenuEntry("Freeze current items", Item::manageItems);

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

        *scrollText += new MenuEntry("Edit top screen scrolling text colors", nullptr, Rendering::editLiveMsgColor); // TODO: settings
        *scrollText += new MenuEntry("Disable top screen scrolling text", nullptr, Rendering::disableScrollingText);
        *overlay += scrollText;

        *overlay += new MenuEntry("Hide HUD", Rendering::triggerHideHUD);
        *overlay += new MenuEntry("Display current respawn location", Rendering::respawnIndicator);
        *overlay += new MenuEntry("Force visibility of Treasure Chest contents", nullptr, Rendering::seeChestContents);
        *overlay += new MenuEntry("Swap single player loading screen", nullptr, Rendering::swapSPLoadingScreen);

        *face += new MenuEntry("Enable custom facial expressions", FaceSelMenu::maintainEdits);
        *face += new MenuEntry("Edit facial expressions", nullptr, Rendering::editFaceExpr);
        *texture += face;

        *texture += new MenuEntry("Swap Link textures", nullptr, Rendering::swapLinkTexture);
        *texture += new MenuEntry("Force visibility of Cheer Outfit pom poms", nullptr, Rendering::forcePomPom);

        *env += new MenuEntry("Disable fog effects", Rendering::disableFog);
        *env += new MenuEntry("Force costume aura visibility", nullptr, Rendering::forceAura);
        *env += new MenuEntry("Force Sword Suit blue sword particles", nullptr, Rendering::forceBlueSwordParticles);
        *env += new MenuEntry("Disable player light sources in dark stages", nullptr, Rendering::disablePlayerLight);

        *render += overlay;
        *render += texture;
        *render += env;
    }

    void InitSaveFolder(PluginMenu &menu)
    {
        save = new MenuFolder("Save Data Editors");

        *save += new MenuEntry("Use Voiceless", Save::useVoiceless);
        *save += new MenuEntry("Set Main Voice", nullptr, Save::mainVoice);
        *save += new MenuEntry("Set Hero Point count", nullptr, Save::heroPointCountSet);
        *save += new MenuEntry("Set Coliseum Win count", nullptr, Save::coliseumWinCountSet);
        *save += new MenuEntry("Edit Level Completion", nullptr, Save::selLevelCompletion);

        MenuFolder *merchant = new MenuFolder("Street Merchant Codes");
        merchantA = new MenuEntry("Set 1st material slot", nullptr, Save::selMerchantSlot);
        merchantB = new MenuEntry("Set 2nd material slot", nullptr, Save::selMerchantSlot);
        merchantC = new MenuEntry("Set 3rd material slot", nullptr, Save::selMerchantSlot);
        merchantD = new MenuEntry("Set 4th material slot", nullptr, Save::selMerchantSlot);
        merchantE = new MenuEntry("Set 5th material slot", nullptr, Save::selMerchantSlot);
        *merchant += new MenuEntry("Force re-stock Street Merchant stall", nullptr, Save::resetMerchant);

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

        std::string timelessNote = "You will be voiceless in Hytopia, but retain a normal voice in the Drablands.";

        // incomplete -- messy for now...
        masterVol = new MenuEntry("Master volume: 100%", nullptr, BGM_SFX::volSet);
        BGMVol = new MenuEntry("BGM volume: 100%", nullptr, BGM_SFX::volSet);
        //*sound += new MenuEntry("Override current BGM", nullptr, BGM_SFX::bgmSet);
        voiceVol = new MenuEntry("(TODO) Link Voice volume: 100%", nullptr, BGM_SFX::volSet);
        lowHPVol = new MenuEntry("(TODO) Low Health Alert volume: 100%", nullptr, BGM_SFX::volSet);
        // emoteVol += new MenuEntry("(TODO) Emote volume: 100%", nullptr, BGM_SFX::?);
        //*sound += new MenuEntry("Set Lobby Ball volume", BGM_SFX::?);
        //*sound += new MenuEntry("Set Lobby Ball counter", BGM_SFX::?);
        *sound += new MenuEntry("Choose Lobby Ball song", nullptr, BGM_SFX::lobbyBallSong);

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

        *sound += new MenuEntry("Force normal or 8-bit BGM", nullptr, BGM_SFX::forceNormal8bit);
        *sound += new MenuEntry("Disable Timeless Tunic voice filter", nullptr, BGM_SFX::disable8bitVoice, timelessNote);
        *sound += new MenuEntry("Force 8-bit sword SFX", nullptr, BGM_SFX::force8bitSword);

        // auto-managed by plugin; hidden from users...
        lobbyBallAuto = new MenuEntry("Write lobby ball edits (auto)", BGM_SFX::writeLobbyBallSel);
    }
}