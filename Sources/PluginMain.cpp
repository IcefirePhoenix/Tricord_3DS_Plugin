#include "Helpers.hpp"
#include "Cheats.hpp"
#include "csvc.h"
#include "pIDindex.h"
#include <3ds.h>

#include "CTRPluginFrameworkImpl/Menu/HotkeysModifier.hpp"

#include "CTRPluginFrameworkImpl/Preferences.hpp"
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
    MenuFolder* turbo;
    MenuFolder* effects;
    MenuFolder* gameplay;
    MenuFolder* warp;
    MenuFolder* healthFairies;
    MenuFolder* energy;
    MenuFolder* enemies;
    MenuFolder* physics;
    MenuFolder* moonJumpFlight;
    MenuFolder* costume;
    MenuFolder* miscellaneous;
    MenuFolder* player;
    MenuFolder* items;
    MenuFolder* render;
    MenuFolder* save;
    MenuFolder* sound;

    static MenuEntry* EntryWithHotkey(MenuEntry* entry, const std::vector<Hotkey>& hotkeys)
    {
        if (entry != nullptr) {
            for (const Hotkey& hotkey : hotkeys)
                entry->Hotkeys += hotkey;
        }
        return entry;
    }

    void    InitMenu(PluginMenu& menu)
    {
        InitFreecamCodes(menu);
        InitGameModes(menu);
        InitGameplayFolder(menu);
        InitEmoteFolder(menu);
        InitCostumeFolder(menu);
        InitMiscFolder(menu);
        InitPlayerFolder(menu);
        InitItemsFolder(menu);
        InitRenderFolder(menu);
        InitSaveFolder(menu);
        InitSoundFolder(menu);

        AddAllFolders(menu);

        HideRegionEntries(menu);
    }

    void AddAllFolders(PluginMenu& menu)
    {
        menu += chaos;
        menu += turbo;
        menu += costume;
        menu += emotes;
        menu += player;
        menu += gameplay;
        menu += items;
        menu += render;
        menu += save;
        menu += sound;
        menu += miscellaneous;
    }

    void InitGameModes(PluginMenu& menu)
    {
        chaos = new MenuFolder("Chaos Mode by Glem");
        effects = new MenuFolder("Chaos Events");

        *effects += new MenuEntry("(TODO) View selected events");
        *effects += new MenuEntry("(TODO) Edit selected events");
        *effects += new MenuEntry("(TODO) Reset event selection");
        *chaos += effects;

        *chaos += new MenuEntry("(TODO) Select event trigger interval");

        turbo = new MenuFolder("Turbo Mode by Peanut");
        *turbo += new MenuEntry("Enable Turbo Mode", nullptr, TurboMode::enableTurboMode);
    }

    void InitGameplayFolder(PluginMenu& menu)
    {
        gameplay = new MenuFolder("Gameplay");
        warp = new MenuFolder("Instant Warp");
        healthFairies = new MenuFolder("Health & Fairies");
        energy = new MenuFolder("Energy");
        enemies = new MenuFolder("Enemies");
        physics = new MenuFolder("Ground Physics");
        moonJumpFlight = new MenuFolder("Moon Jump & Flight");

        *warp += new MenuEntry("Warp to any location", nullptr, Gameplay::instantWarp);
        *warp += new MenuEntry("Warp to any stage in current Drablands level", nullptr, Gameplay::stageWarp);
        reWarp = new MenuEntry("Return to previous warp: None", nullptr, Gameplay::warpAgain);
        *warp += reWarp;
        *warp += (EntryWithHotkey(new MenuEntry("Reset the current area (trigger via hotkey)", Gameplay::resetRoom), {
            Hotkey(Key::L | Key::R, "Reset current area"),
        }));

        doppelEnableAuto = new MenuEntry("Mid-warp Doppel Enable (auto)", Gameplay::midWarpDoppelEnable);
        challengeEditAuto = new MenuEntry("Challenge ID edit (auto)", Gameplay::writeChallengeEdit);

        *healthFairies += new MenuEntry("Set maximum heart containers", nullptr, Gameplay::maxHealthSet,
        "Affects your base heart containers. Costume effects that add or remove heart containers still apply.");
        healthMaxAuto = new MenuEntry("Set max heart containers (auto)", Gameplay::writeMaxHealth);
        *healthFairies += new MenuEntry("Infinite health", Gameplay::infHealth,
        "Freezes your health to match your maximum heart containers. May not be consistent if a costume that adds or removes heart containers is worn.");
        *healthFairies += new MenuEntry("Infinite fairies", Gameplay::infFairy);

        *energy += new MenuEntry("Infinite energy", Gameplay::infEnergy);
        *energy += new MenuEntry("Set maximum energy", nullptr, Gameplay::maxEnergySet);
        *energy += new MenuEntry("Set energy consumption multiplier", nullptr, Gameplay::energyConsumeMultiplier);
        *energy += new MenuEntry("Use large energy gauge graphic", nullptr, Gameplay::useLargeEnergyGauge,
        "Switches the energy gauge in the UI to the Energy Gear / Cheer Outfit / Tri Suit energy gauge. Does not affect your max energy.");

        *enemies += (EntryWithHotkey(new MenuEntry("Insta-kill all spawned enemies", Gameplay::autoKillEnemy), {
            Hotkey(Key::L | Key::B, "Insta-kill all spawned enemies"),
        }));

        *enemies += (EntryWithHotkey(new MenuEntry("Make all spawned enemies invincible", Gameplay::enemyInvinci), {
            Hotkey(Key::R | Key::B, "Make all spawned enemies invincible"),
        }));

        /* ------------------------------ */

        physicsSelG = new MenuEntry("Player 1 (Green): Not edited", nullptr, Gameplay::changePhysics);
        physicsSelB = new MenuEntry("Player 2 (Blue): Not edited", nullptr, Gameplay::changePhysics);
        physicsSelR = new MenuEntry("Player 3 (Red): Not edited", nullptr, Gameplay::changePhysics);

        physicsEditAutoG = new MenuEntry("Write physics edit G (auto)", Gameplay::writePhysicsChanges);
        physicsEditAutoB = new MenuEntry("Write physics edit B (auto)", Gameplay::writePhysicsChanges);
        physicsEditAutoR = new MenuEntry("Write physics edit R (auto)", Gameplay::writePhysicsChanges);

        MenuEntry* physicsSlots[6] = {
            physicsEditAutoG,
            physicsSelG,
            physicsEditAutoB,
            physicsSelB,
            physicsEditAutoR,
            physicsSelR
        };

        for (int iterator = 0; iterator < 6; ++iterator)
        {
            // assign ArgIDs for EditAuto and Sel -> green = 0, blue = 1, red = 2
            // integer division always rounds up, so iterator / 2 should always result in 0-2 values
            physicsSlots[iterator]->SetArg(reinterpret_cast<void*>(iterator / 2));

            // add the physicsSel entries to the menu
            if (iterator % 2 == 1)
                *physics += physicsSlots[iterator];
        }

        /* ------------------------------ */

        // Radio group 1 for mutual exclusivity
        *moonJumpFlight += (EntryWithHotkey(new MenuEntry(1, "Enable Moon Jump", Gameplay::moonJump), {
            Hotkey(Key::CPadUp, "Move North"),
            Hotkey(Key::CPadDown, "Move South"),
            Hotkey(Key::CPadRight, "Move East"),
            Hotkey(Key::CPadLeft, "Move West"),
            Hotkey(Key::X, "Ascend")
        }));
        *moonJumpFlight += (EntryWithHotkey(new MenuEntry(1, "Enable Flight", Gameplay::flight,
        "Avoid holding "+std::string(FONT_B)+" during teleportation sequences to prevent crashes."), {
            Hotkey(Key::CPadUp, "Move North"),
            Hotkey(Key::CPadDown, "Move South"),
            Hotkey(Key::CPadRight, "Move East"),
            Hotkey(Key::CPadLeft, "Move West"),
            Hotkey(Key::X, "Ascend"),
            Hotkey(Key::B, "Descend")
        }));

        *moonJumpFlight += new MenuEntry("Keep uncontrolled players hovering in place", Gameplay::hover);
        *moonJumpFlight += new MenuEntry("Adjust ascent speed: Medium", nullptr, Gameplay::adjustAscentSpeed);
        *moonJumpFlight += new MenuEntry("Adjust descent speed: Medium", nullptr, Gameplay::adjustDescentSpeed,
        "Only affects Flight. Moon Jump uses natural gravity.");
        *moonJumpFlight += new MenuEntry("Adjust lateral speed: Medium", nullptr, Gameplay::adjustLateralSpeed);

        *gameplay += warp;
        *gameplay += healthFairies;
        *gameplay += energy;
        //*gameplay += enemies;
        *gameplay += physics;
        *gameplay += moonJumpFlight;

        *gameplay += new MenuEntry("Control all players", nullptr, Gameplay::controlAllPlayers);
        controlAllAuto = new MenuEntry("Control all players (auto)", Gameplay::writePlayerControl);

        *gameplay += new MenuEntry("Infinite time", Gameplay::infTime);

        *gameplay += new MenuEntry("Set custom movement speed", nullptr, Gameplay::customSpeed,
        "Work in progress. Currently only affects walking speed.");
        }

    void InitEmoteFolder (PluginMenu& menu)
    {
        emotes = new MenuFolder("Emotes");

        *emotes += (EntryWithHotkey(new MenuEntry("Enable Drablands Emote Swapper", Emotes::drablandEmoteSwapper), {
            Hotkey(Key::DPadLeft, "Swap to original emote set"),
            Hotkey(Key::DPadRight, "Swap to alternative emote set")
        }));

        *emotes += (EntryWithHotkey(new MenuEntry("Enable Lobby Emote Swapper", Emotes::lobbyEmoteSwapper), {
            Hotkey(Key::DPadLeft, "Swap to original emote set"),
            Hotkey(Key::DPadRight, "Swap to alternative emote set"),
            Hotkey(Key::DPadUp, "Use blank emote")
        }));
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

    void InitCostumeFolder(PluginMenu& menu)
    {
        costume = new MenuFolder("Costumes");

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

        *costume += new MenuEntry("Preserve Doppel costume edits in single player lobby", nullptr, Costume::preventLobbyReset,
        "Determines whether Doppel costumes reset to the Hero's Tunic when entering or returning to the single player lobby.");

        *costume += new MenuEntry("Enable Cosmetic Costumes", nullptr, Costume::enableCosmeticCostume,
        "Determines whether choices under Set Cosmetic Costume get applied or not.");
        *costume += new MenuEntry("Set Cosmetic Costume", nullptr, Costume::setCosmeticCostume,
        "Select a costume to gain the cosmetics of without changing the effects of the currently worn costume hiding underneath. Any usages of the \"Change Player Costume\" code or normal in-game costume changes after activating this will only change your costume's effect.");
        writeCosmeticCostumeID = new MenuEntry("Write cosmetic costumes (auto)", Costume::writeCosmeticCostume);
        *costume += new MenuEntry("Set Custom Sword Model(s)", nullptr, Player::setSwordChanges);
        swordEditAuto = new MenuEntry("Write Sword edits (auto)", Player::writeSwordChanges);

        // create costume sub-folders
        MenuFolder* costumeEffects = new MenuFolder("Costume Effects",
        "Any combination of these costume effects can be selected and will be applied to the chosen player. This does not overwrite any existing costume effects the player may have.");

        MenuFolder* indCostumeEffects[3] = {
            new MenuFolder("Costume Effects - Player 1 (Green)"),
            new MenuFolder("Costume Effects - Player 2 (Blue)"),
            new MenuFolder("Costume Effects - Player 3 (Red)")
        };

        MenuEntry* subEntries[3][15];

        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++) {
            pIDindex args;
            args.playerID = iterateThruPlayers;

            // Checkbox to apply effects
            subEntries[iterateThruPlayers][0] = new MenuEntry("Apply costume effects", Costume::writeIndCostumeEffects);
            subEntries[iterateThruPlayers][0]->SetArg(reinterpret_cast<void*>(iterateThruPlayers));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][0];

            // Double damage
            args.index = 19;
            subEntries[iterateThruPlayers][1] = new MenuEntry("(  ) Bear / Cursed - Double damage taken", nullptr, Costume::setIndCostumeEffect);
            subEntries[iterateThruPlayers][1]->SetArg(reinterpret_cast<void*>(new pIDindex(args)));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][1];

            // Lucky dodge
            subEntries[iterateThruPlayers][2] = new MenuEntry("(  ) Lucky Dodge", nullptr, Costume::luckyDodge);
            subEntries[iterateThruPlayers][2]->SetArg(reinterpret_cast<void*>(iterateThruPlayers));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][2];

            // Zora
            args.index = 1;
            subEntries[iterateThruPlayers][3] = new MenuEntry("(  ) Zora Costume - Enhanced swimming", nullptr, Costume::setIndCostumeEffect);
            subEntries[iterateThruPlayers][3]->SetArg(reinterpret_cast<void*>(new pIDindex(args)));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][3];

            // Goron
            args.index = 2;
            subEntries[iterateThruPlayers][4] = new MenuEntry("(  ) Goron Garb - Burn immunity & lava swim", nullptr, Costume::setIndCostumeEffect,
            "Does not prevent you from taking damage from lava pillars. Does not allow you to walk through flame walls. Does not allow you to pick up flaming rupees.");
            subEntries[iterateThruPlayers][4]->SetArg(reinterpret_cast<void*>(new pIDindex(args)));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][4];

            // Parka
            args.index = 3;
            subEntries[iterateThruPlayers][5] = new MenuEntry("(  ) Cozy Parka - Freeze & ice slip immunity", nullptr, Costume::setIndCostumeEffect);
            subEntries[iterateThruPlayers][5]->SetArg(reinterpret_cast<void*>(new pIDindex(args)));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][5];

            // Dune
            args.index = 21;
            subEntries[iterateThruPlayers][6] = new MenuEntry("(  ) Dunewalker Duds - Quicksand immunity", nullptr, Costume::setIndCostumeEffect,
            "Does not some with the sand ripple effect.");
            subEntries[iterateThruPlayers][6]->SetArg(reinterpret_cast<void*>(new pIDindex(args)));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][6];

            // Legendary
            args.index = 40;
            subEntries[iterateThruPlayers][7] = new MenuEntry("(  ) Legendary Dress - Increased heart drops", nullptr, Costume::setIndCostumeEffect);
            subEntries[iterateThruPlayers][7]->SetArg(reinterpret_cast<void*>(new pIDindex(args)));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][7];

            // Rupee
            args.index = 11;
            subEntries[iterateThruPlayers][8] = new MenuEntry("(  ) Rupee Regalia - Increased rupee drops", nullptr, Costume::setIndCostumeEffect);
            subEntries[iterateThruPlayers][8]->SetArg(reinterpret_cast<void*>(new pIDindex(args)));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][8];

            // Serpent
            args.index = 29;
            subEntries[iterateThruPlayers][9] = new MenuEntry("(  ) Serpent's Toga - Invincible when still", nullptr, Costume::setIndCostumeEffect);
            subEntries[iterateThruPlayers][9]->SetArg(reinterpret_cast<void*>(new pIDindex(args)));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][9];

            // Cacto
            args.index = 33;
            subEntries[iterateThruPlayers][10] = new MenuEntry("(  ) Cacto Clothes - Damage foes on contact", nullptr, Costume::setIndCostumeEffect);
            subEntries[iterateThruPlayers][10]->SetArg(reinterpret_cast<void*>(new pIDindex(args)));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][10];

            // Dapper
            args.index = 28;
            subEntries[iterateThruPlayers][11] = new MenuEntry("(  ) Dapper Spinner - 3x"+std::string(FONT_B)+" to spin attack", nullptr, Costume::setIndCostumeEffect,
            "This spin attack is not affected by the Spin Attack Attire's effect.");
            subEntries[iterateThruPlayers][11]->SetArg(reinterpret_cast<void*>(new pIDindex(args)));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][11];

            // Ninja
            args.index = 4;
            subEntries[iterateThruPlayers][12] = new MenuEntry("(  ) Ninja Gi - Instant triple damage dash", nullptr, Costume::setIndCostumeEffect);
            subEntries[iterateThruPlayers][12]->SetArg(reinterpret_cast<void*>(new pIDindex(args)));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][12];

            // Spin Attack
            args.index = 5;
            subEntries[iterateThruPlayers][13] = new MenuEntry("(  ) Spin Attack Attire - Great Spin Attack", nullptr, Costume::setIndCostumeEffect);
            subEntries[iterateThruPlayers][13]->SetArg(reinterpret_cast<void*>(new pIDindex(args)));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][13];

            // Master
            args.index = 32;
            subEntries[iterateThruPlayers][14] = new MenuEntry("(  ) Sword Master Suit - Big sword beam", nullptr, Costume::setIndCostumeEffect,
            "Only has an effect if you are wearing another sword beam costume (Sword Suit or Fierce Deity Armor), or if you have enabled a sword beam effect from the All-Player Costume Effects folder.");
            subEntries[iterateThruPlayers][14]->SetArg(reinterpret_cast<void*>(new pIDindex(args)));
            *indCostumeEffects[iterateThruPlayers] += subEntries[iterateThruPlayers][14];

            // Add to superfolder
            *costumeEffects += indCostumeEffects[iterateThruPlayers];
        }

        MenuFolder* allPCostumeEffects = new MenuFolder("All-Player Costume Effects");
        *allPCostumeEffects += new MenuEntry("(  ) Kokiri Clothes - Bow upgrade", nullptr, Costume::kokiri);
        *allPCostumeEffects += new MenuEntry("(  ) Big Bomb Outfit - Bomb upgrade", nullptr,  Costume::bigBomb);
        *allPCostumeEffects += new MenuEntry("(  ) Torrent Robe - Water Rod upgrade", nullptr, Costume::torrent);
        *allPCostumeEffects += new MenuEntry("(  ) Boomeranger - Boomerang upgrade", nullptr, Costume::boomeranger);
        *allPCostumeEffects += new MenuEntry("(  ) Fire Blazer - Fire Gloves upgrade", nullptr, Costume::fireBlazer);
        *allPCostumeEffects += new MenuEntry("(  ) Gust Garb - Gust Jar upgrade", nullptr, Costume::gustGarb);
        *allPCostumeEffects += new MenuEntry("(  ) Robowear - Gripshot upgrade", nullptr, Costume::robowear);
        *allPCostumeEffects += new MenuEntry("(  ) Hammerwear - Hammer upgrade", nullptr, Costume::hammerwear);
        *costumeEffects += allPCostumeEffects;

        MenuFolder* bonusEffects = new MenuFolder("Bonus Effects", "A collection of codes that tweak the effect of some costumes.");
        
        // TODO - Other bonus effects
        *bonusEffects += new MenuEntry("Tingle Tights - Set number of balloons", nullptr, Costume::tingle,
        "You do not need to be wearing Tingle Tights.");
        autoInfBalloons = new MenuEntry("Infinite tingle balloons (auto)", Costume::writeBalloons);
        *bonusEffects += new MenuEntry("Dapper Spinner - Reduce # swings to spin", nullptr, Costume::dapperInstant,
        "Must be wearing Dapper Spinner or have its effect forced on.");
        autoDapper = new MenuEntry("Dapper faster spins (auto)", Costume::writeDapper);
        *bonusEffects += new MenuEntry("Toggle sword beam cooldown", nullptr, Costume::selectLinkBeam);
        autoBeamCooldown = new MenuEntry("Set Beam Cooldown (auto)", Costume::setBeamCooldown);
        *costumeEffects += bonusEffects;

        *costume += costumeEffects;

        *costume += new MenuEntry("Costume Randomizers", nullptr, Costume::costumeRandomizer,
        "Toggle randomizers for either your effective or cosmetic costume. Changes the costume to a new, random one every stage (changes happen during Triforce portal teleportation). If setting a cosmetic randomizer, you must enable cosmetic costumes.");
        autoCostumeRandomizer = new MenuEntry("Write random costume IDs (auto)", Costume::writeRandomCostume);

        // NOT added to main menu -- these are auto-managed by the plugin and don't need to be accessed by the user
        manageCatalogSize = new MenuEntry("Manage Catalog Size (auto)", Costume::manageCatalogSizeAuto);
        initCustomCostumes = new MenuEntry("Init Custom Costume list (auto)", Costume::initCustomCostumesAuto);
        writeCostumeIDToSlot = new MenuEntry("Write to costume slots (auto)", Costume::writeCostumeSlot);

        *costume += new MenuEntry("Force Bear Minimum / Maximum", nullptr, Costume::forceBearMinMax);
    }

    void InitMiscFolder(PluginMenu& menu)
    {
        miscellaneous = new MenuFolder("Miscellaneous");

        // broken...
        *miscellaneous += (EntryWithHotkey(new MenuEntry("(TODO) Enable button spam", Miscellaneous::buttonSpammer, "When any of the selected keys are\npressed down, they will automatically spam.\nGood for in-game manuevers that require\nstrict timing of button input(s).\n\nDefault keys: A, B, X, Y, L, R."), {
            Hotkey(Key::A | Key::B | Key::X | Key::Y | Key::L | Key::R , "Button Spammer")
        }));

        *miscellaneous += new MenuEntry("Use photo viewer touchscreen toggle", Miscellaneous::managePhotoDisp);
        *miscellaneous += new MenuEntry("Force instant text boxes", nullptr, Miscellaneous::manageInstantText);
        *miscellaneous += new MenuEntry("Set Lobby Ball counter (TODO)", nullptr, Miscellaneous::setLobbyBallCounter);
        *miscellaneous += new MenuEntry("Toggle camera on X button: No edits", nullptr, Miscellaneous::toggleCameraButton);
        *miscellaneous += new MenuEntry("Disable camera shutter", nullptr, Miscellaneous::toggleCameraShutter);

        autoWriteCameraStatus = new MenuEntry("Toggle camera status (auto)",  Miscellaneous::writeCameraEdits);
        autoDisableCamShutter = new MenuEntry("Disable camera shutter (auto)", Miscellaneous::writeShutterDisable);
    }

    void InitPlayerFolder(PluginMenu& menu)
    {
        player = new MenuFolder("Player");

        *player += new MenuEntry("Bypass Doppel Master dialogue cutscene", Player::bypassDoppelDemo);
        *player += new MenuEntry("Enable position save/load touchscreen toggle", Player::posEditor);

        *player += (EntryWithHotkey(new MenuEntry("Bring Doppels to your position (with hotkey)", Player::doppelsToPlayer), {
            Hotkey(Key::X | Key::DPadDown, "Bring Doppels to you"),
        }));

        *player += new MenuEntry("Toggle collision edits (experimental!)", nullptr, Player::setCollisionChanges);
        *player += new MenuEntry("Toggle animation storage edits", nullptr, Player::setAnimStorageChanges);
        *player += new MenuEntry("Toggle player spawn edits", nullptr, Player::setSpawnChanges);
        *player += new MenuEntry("Toggle jinxed model edits", nullptr, Player::setJinxChanges);
        *player += new MenuEntry("Toggle player visibility edits", nullptr, Player::setVisibilityChanges);
        *player += new MenuEntry("Toggle player invincibility edits", nullptr, Player::setInvincibilityChanges);
        *player += new MenuEntry("Toggle PvP damage edits", nullptr, Player::setPVPChanges);
        *player += new MenuEntry("Set custom player model size", nullptr, Player::setSizeChanges);

        jinxEditAuto = new MenuEntry("Write Jinx edits (auto)", Player::writeJinxChanges);
        spawnEditAuto = new MenuEntry("Write Spawn edits (auto)", Player::writeSpawnChanges);
        visibleEditAuto = new MenuEntry("Write Visibility edits (auto)", Player::writeVisibilityChanges);
        invinciEditAuto = new MenuEntry("Write Invincibility edits (auto)", Player::writeInvincibilityChanges);
        pvpEditAuto = new MenuEntry("Write PVP edits (auto)", Player::writePVPChanges);
        animStoreEditAuto = new MenuEntry("Write Anim edits (auto)", Player::writeAnimStorageChanges);
        colEditAuto = new MenuEntry("Write Collision edits (auto)", Player::writeCollisionChanges);
    }

    void InitItemsFolder(PluginMenu& menu)
    {
        items = new MenuFolder("Items");

        *items += new MenuEntry("Set current item", nullptr, Item::itemOpt);
        *items += new MenuEntry("Set Shadow Link item", nullptr, Item::shadowItemOpt);
        *items += new MenuEntry("Set strafing speeds", nullptr, Item::strafingSpeedSet);
        *items += new MenuEntry("Remove current items", nullptr, Item::manageItems);
        *items += new MenuEntry("Freeze current items", Item::manageItems);

        // TODO: *items += new MenuEntry("Always use upgraded Items", Item::upgradeItemAlways);
    }

    void InitRenderFolder(PluginMenu& menu)
    {
        render = new MenuFolder("Rendering");

        *render += new MenuEntry("Hide HUD", Rendering::triggerHideHUD);
        *render += new MenuEntry("Disable fog effects", Rendering::disableFog);
        *render += new MenuEntry("Display current respawn location", Rendering::respawnIndicator);
        *render += new MenuEntry("Edit top screen scrolling text colors", nullptr, Rendering::editLiveMsgColor);
        *render += new MenuEntry("Disable top screen scrolling text", nullptr, Rendering::disableScrollingText);
        *render += new MenuEntry("Force visibility of Treasure Chest contents", nullptr, Rendering::seeChestContents);
        *render += new MenuEntry("Swap Link textures", nullptr, Rendering::swapLinkTexture);
        *render += new MenuEntry("Swap single player loading screen", nullptr, Rendering::swapSPLoadingScreen);
        *render += new MenuEntry("Force visibility of Cheer Outfit pom poms", nullptr, Rendering::forcePomPom);
        *render += new MenuEntry("Force visibility of a costume aura", nullptr, Rendering::forceAura);
        *render += new MenuEntry("Force Sword Suit blue sword particles", nullptr, Rendering::forceBlueSwordParticles);
        *render += new MenuEntry("Disable player light sources in dark stages", nullptr, Rendering::disablePlayerLight);
    }

    void InitSaveFolder(PluginMenu& menu)
    {
        save = new MenuFolder("Save Data Editors");

        *save += new MenuEntry("Use Voiceless", Save::useVoiceless);
        *save += new MenuEntry("Set Main Voice", nullptr, Save::mainVoice);
        *save += new MenuEntry("Set Hero Point count", nullptr, Save::heroPointCountSet);
        *save += new MenuEntry("Set Coliseum Win count", nullptr, Save::coliseumWinCountSet);
        //*save += new MenuEntry("Edit Level Completion (BROKEN)", nullptr, Save::selLevelCompletion);

        MenuFolder* merchant = new MenuFolder("Street Merchant Codes");
            merchantA = new MenuEntry("Set 1st material slot", nullptr, Save::selMerchantSlot);
            merchantB = new MenuEntry("Set 2nd material slot", nullptr, Save::selMerchantSlot);
            merchantC = new MenuEntry("Set 3rd material slot", nullptr, Save::selMerchantSlot);
            merchantD = new MenuEntry("Set 4th material slot", nullptr, Save::selMerchantSlot);
            merchantE = new MenuEntry("Set 5th material slot", nullptr, Save::selMerchantSlot);
            *merchant += new MenuEntry("Force re-stock Street Merchant stall", nullptr, Save::resetMerchant);

            MenuEntry* slots[5] = {
                merchantA,
                merchantB,
                merchantC,
                merchantD,
                merchantE
            };

            for (int iterator = 0; iterator < 5; ++iterator)
            {
                slots[iterator]->SetAltIcon(true);      // remove icon
                slots[iterator]->SetArg(reinterpret_cast<void*>(iterator * 2));  // slot IDs are even numbers
                *merchant += slots[iterator];
            }

        *save += merchant;
    }

    void InitSoundFolder(PluginMenu& menu)
    {
        sound = new MenuFolder("BGM & SFX");

        masterVol = new MenuEntry("Master volume: 100%", nullptr, BGM_SFX::volSet);
        BGMVol = new MenuEntry("BGM volume: 100%", nullptr, BGM_SFX::volSet);
        //*sound += new MenuEntry("Override current BGM", nullptr, BGM_SFX::bgmSet);
        voiceVol = new MenuEntry("(TODO) Link Voice volume: 100%", nullptr, BGM_SFX::volSet);
        lowHPVol = new MenuEntry("(TODO) Low Health Alert volume: 100%", nullptr, BGM_SFX::volSet);
        // emoteVol += new MenuEntry("(TODO) Emote volume: 100%", nullptr, BGM_SFX::volSet);
        //*sound += new MenuEntry("Set Lobby Ball volume", BGM_SFX::volSet);
        *sound += new MenuEntry("Choose Lobby Ball song", nullptr, BGM_SFX::lobbyBallSong);

        MenuEntry* soundEntries[4] = {
            masterVol,
            BGMVol,
            voiceVol,
            lowHPVol,
            //emoteVol
        };

        for (int iterator = 0; iterator < 4; ++iterator)
        {
            soundEntries[iterator]->SetArg(reinterpret_cast<void*>(iterator));  // store entry IDs
            *sound += soundEntries[iterator];
        }

        lobbyBallAuto = new MenuEntry("Write lobby ball edits (auto)", BGM_SFX::writeLobbyBallSel);

        *sound += new MenuEntry("Force normal or 8-bit BGM", nullptr, BGM_SFX::forceNormal8bit);
        *sound += new MenuEntry("Disable Timeless Tunic voice filter", nullptr, BGM_SFX::disable8bitVoice,
        "You will be voiceless in Hytopia, but retain a normal voice in the Drablands.");
        *sound += new MenuEntry("Force 8-bit sword SFX", nullptr, BGM_SFX::force8bitSword);
    }

    void HideRegionEntries(PluginMenu& menu)
    {
        switch (Process::GetTitleID())
		{
			case TID_USA:
                break;

            case TID_EUR:
                break;

            case TID_JPN:
                break;

            case TID_KOR:
                restoreGreatFairy->Hide();
                break;
        }
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
            turbo->HideWithoutDisable();
            costume->HideWithoutDisable();
            emotes->HideWithoutDisable();
            player->HideWithoutDisable();
            gameplay->HideWithoutDisable();
            items->HideWithoutDisable();
            render->HideWithoutDisable();
            save->HideWithoutDisable();
            sound->HideWithoutDisable();
            miscellaneous->HideWithoutDisable();
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
                turbo->Hide();
                costume->Show();
                emotes->Show();
                player->Show();
                gameplay->Show();
                items->Show();
                render->Show();
                save->Show();
                sound->Show();
                miscellaneous->Show();
            }
            else
            {
                menuFreecam->HideWithoutDisable();
                editFreecamControls->HideWithoutDisable();
                editFreecamSen->HideWithoutDisable();
                swapZoom->HideWithoutDisable();

                chaos->Show();
                turbo->Show();
                costume->HideWithoutDisable();
                emotes->HideWithoutDisable();
                player->HideWithoutDisable();
                gameplay->HideWithoutDisable();
                items->HideWithoutDisable();
                render->HideWithoutDisable();
                save->HideWithoutDisable();
                sound->HideWithoutDisable();
                miscellaneous->HideWithoutDisable();
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

        Address::InitMemoryRange();
        AddressList::InitAddresses();

        // Patch executable on boot
        // Disable Doppel between-stage costume reset
        Process::Patch(AddressList::DoppelStageResetA.addr, 0xE320F000);
        Process::Patch(AddressList::DoppelStageResetB.addr, 0xE320F000);
        Process::Patch(AddressList::DoppelStageResetC.addr, 0xE320F000);

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
        u32*    found;

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
            found[13] = 0xE1A00000; // NOP operation
        }

        svcUnmapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, textTotalSize);
    exit:
        svcCloseHandle(processHandle);
    }

    void    PatchProcess(FwkSettings& settings)
    {
        if (Preferences::IsEnabled(Preferences::HIDToggle))
            settings.UseGameHidMemory = true;

        //settings.CachedDrawMode = true;
        settings.ThreadPriority = 0x3E;

        ToggleTouchscreenForceOn();
    }

    // Called when the process exits
    void    OnProcessExit(void)
    {
        ToggleTouchscreenForceOn();
    }
}
