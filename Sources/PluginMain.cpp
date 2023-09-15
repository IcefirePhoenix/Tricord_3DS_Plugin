#include <3ds.h>
#include "cheats.hpp"
#include "csvc.h"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "CTRPluginFramework/Menu/MenuEntry.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryTools.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuTools.hpp"

#include <CTRPluginFramework.hpp>

#include <vector>

namespace CTRPluginFramework

{
     // typically menu entries are not given a name; they are simply initialized with required parameters, but
     // there's no way to reference them later, which is troublesome for things like configuring the menu to edit
     // the visibility of menu entries. currently the only implementation of this is used for the costume slots...
     // there are no other menu entries that appear/disappear in the menu but this may change in the future

     // actually we should init everything here since some entries do disable others...
     
     // to get around this, we declare the menu entries with a name OUTSIDE of the InitMenu() function...
     // this allows us to do two things:

     // 1) access these menu entries in other cpp files
     // 2) lets us reference these entries in the future

     // TODO: probably revamp init menu... bring it to the top here, maybe even just init everything outside the function 
     // and place them here for consistency

     MenuEntry* menuCostumeSlotA;
     MenuEntry* menuCostumeSlotB;
     MenuEntry* menuCostumeSlotC;
     MenuEntry* menuCostumeSlotD;

     MenuEntry* manageCatalogSize;
     MenuEntry* restoreGreatFairy;

     MenuEntry* initCustomCostumes;
     MenuEntry* writeCostumeIDToSlot;

     void    InitMenu(PluginMenu& menu)
     {
         // This is where the main menu is configured

         // Menu Entries come in many types, though the two main ones are checkbox and gear
         // 
         // - Checkboxes are self-explanatory
         // They are created through this constructor: 
         // MenuEntry [entry internal name] = new MenuEntry("[public entry name displayed in menu]", [function called when enabled]);
         // 
         // - Gear entries do not run continuously -- they run once when selected, which is nice for things 
         // such as toggles. They are created through this constructor:
         // MenuEntry [entry internal name] = new MenuEntry("[public entry name displayed in menu]", nullptr, [function called]);
         // 
         // - Other entry types and their constructors are defined in the lib files

         // Functions don't necessarily have to edit game values right away; just like any code, you can have Menu Entries
         // call related child functions that do other things. In this case, you can have parent functions that
         // do things to the plugin menu rather than the game
         //
         // An example would be in the [Player Codes -> Use Jinxed model] entry. When the "Use Jinxed model" entry is selected,
         // it'll call the openJinxOptions function, which will cause the "Green Link", "Blue Link", and "Red Link"
         // entries to appear in the menu. When either of the Link colors are selected, they will call the g/b/rJinx function,
         // which will take care of loading in the Jinx model. If "Use Jinxed model" is de-selected, any changes made by g/b/rJinx are 
         // reverted and the "Green/Blue/Red Link" options in the menu will disappear

         // Function names are defined in cheats.hpp, however the actual function code is written in .cpp files titled after
         // the function's group. So for example, Emote Codes will have their function names in cheat.hpp, however, the actual Emote
         // function code itself will be found in emotes.cpp

         // Menu Folders are similar to Menu Entries; they are created via this constructor:
         // MenuFolder *[folder internal name] = new MenuFolder("[public name displayed in menu]");
         // 
         // Adding Menu Entries to folders can be done via +=

         // I am planning on having 3 menus: the main menu, freecam menu, and game modes menu
         // 
         // Main Menu - main codes that aren't significant enough to warrant their own button
         // Freecam - self explanatory
         // Game Modes - Chaos mode will be here, and possibly other major game-altering cheats/mods as well. If we figure
         // out Hide and Seek mode, it can go here as well. I might remove PvP from [Player Codes -> Enable PvP damage]
         // and make it its own gamemode, but again, we haven't quite figured out the specifics of this yet, so for now, it'll remain
         // where it currently is...
         // 
         // I have not quite figured out exactly how to create 2 other menus AND be able to swap
         // between the 3 menus without breaking anything, so right now this section only initializes the main menu


         // Create Emote folder
         MenuFolder* emotes = new MenuFolder("Emote Codes");
         {
             *emotes += new MenuEntry("Enable Emote Swapper", emoteSwapper);
             *emotes += new MenuEntry("Use Custom Emotes", customEmotes);
         }
         // Add Emote folder to main menu
         menu += emotes;


         // Create Link Color folder
         MenuFolder* linkcolor = new MenuFolder("Custom Link Colors");
         {
             // these should be presets now....
             // OG: green, blue, red
             // [name]: purple, blue, red
             // [name]: pink, light blue, white
             // [name]: blue, light, white
             // [name]: black, blue, white
             // [name]: pink, yellow, 
             *linkcolor += new MenuEntry("Set Green Link color", nullptr, customColorG);
             *linkcolor += new MenuEntry("Set Blue Link color", nullptr, customColorB);
             *linkcolor += new MenuEntry("Set Red Link color", nullptr, customColorR);
             *linkcolor += new MenuEntry("Use re-colored level textures", recolorLevelTex);
         }
         // Add Link Color folder to main menu
         menu += linkcolor;

         // Create Costume folder
         MenuFolder* costume = new MenuFolder("Costume Codes");

         {
             *costume += new MenuEntry("Trigger Custom Costume Slots", nullptr, openCustomCostumeSlots);

             // init slot entries
             // are these gear or box??
             menuCostumeSlotA = new MenuEntry("Set custom costume slot A", nullptr, selectCostumeID, "This setting is not accessible if Restore Great Fairy Costume is enabled.");
             menuCostumeSlotB = new MenuEntry("Set custom costume slot B", nullptr, selectCostumeID);
             menuCostumeSlotC = new MenuEntry("Set custom costume slot C", nullptr, selectCostumeID);
             menuCostumeSlotD = new MenuEntry("Set custom costume slot D", nullptr, selectCostumeID);
             restoreGreatFairy = new MenuEntry("Restore Great Fairy Costume", greatFairyEnable);

             
             // these is not added to the menu -- this is auto-managed by the plugin
             manageCatalogSize = new MenuEntry("Manage Catalog Size (auto)", manageCatalogSizeAuto);
             initCustomCostumes = new MenuEntry("Init Custom Costume list (auto)", initCustomCostumesAuto);
             writeCostumeIDToSlot = new MenuEntry("Write to costume slots (auto", writeCostumeSlot);

             // add slot entries to menu
             *costume += menuCostumeSlotA;
             *costume += menuCostumeSlotB;
             *costume += menuCostumeSlotC;
             *costume += menuCostumeSlotD;
             *costume += restoreGreatFairy;

             // note: by default these should be hidden 
             menuCostumeSlotA->Hide();
             menuCostumeSlotB->Hide();
             menuCostumeSlotC->Hide();
             menuCostumeSlotD->Hide();

         }

         // handle visibility of costume slots


         // Create Costume Effects sub-folder
         MenuFolder* costumeeffect = new MenuFolder("Custom Costume Effects");

         // Create G/B/R Costume Effects sub-folder
         MenuFolder* gLinkCostumeEffects = new MenuFolder("Green Link");
         {
             /*
             We'll have to add all of these as Menu Entries in the future:

             - Maybe add "Additional Customizations" folder at the top?
                 Such as: new MenuEntry("Set number of Tingle Balloons", nullptr, tingleBalloonNumber);

             - take double damage
             - luck
                 3 presets: bear, cursed, and lucky
                 1 custom: set percentage
             - 2x energy increased bar
             - hearts
                 5 presets: jack, queen, lady, bear min, bear max
                 1 custom:  set number of added/reduced hearts
             - shoot 3 arrows
             - big bombs
             - increased heart drop rate
             - big water pillar
             - zora swim
                 does this include both zora swim damage attack and current resistance?
             - goron lava resistance
             - big boomerang
             - big hammer
             - no slip ice
             - enable balloons
             - cacto
             - increased rupee drop rate
             - sword double damage
             - sword beams - normal
                 4 beams? is this possible for big beams too?
             - sword beams - big
             - cheetah speed
             - quicksand resistance
             - big gust
             - light armor
                 does this include both coliseum fire dmg AND big light radius?
             - ninja dash
                 does this include instant dash AND 3x dmg?
             - serpent toga
             - big fire
             - big robo
             - showstopper increase aggro
             - saa
             - dapper spinner
             - see inside chests
             */
         }
         // the long list of options above will also be added to the following 2 folders:
         MenuFolder* bLinkCostumeEffects = new MenuFolder("Blue Link");
         MenuFolder* rLinkCostumeEffects = new MenuFolder("Red Link");

         // should this just be one option with a secondary window choosing link color?
         // same thing for costume effects...
         *costume += new MenuEntry("Change Player Costume", nullptr, changeLinkCostume);

         // Add G/B/R Costume Effects sub-folder to Costume Effects sub-folder
         *costumeeffect += gLinkCostumeEffects;
         *costumeeffect += bLinkCostumeEffects;
         *costumeeffect += rLinkCostumeEffects;
         // Add Costume Effects sub-folder to Costume folder
         *costume += costumeeffect;
         // Add Costume folder to main menu
         menu += costume;

         // Create Miscellaneous folder
         MenuFolder* miscellaneous = new MenuFolder("Miscellaneous Codes");
         {
             *miscellaneous += new MenuEntry("Enable button spam", buttonSpammer);
             *miscellaneous += new MenuEntry("Force instant text boxes", instantText);
             //*miscellaneous += new MenuEntry("Display photo on top screen", displayPhoto);
                 // should this be part of freecam?
             *miscellaneous += new MenuEntry("Disable sword beam cooldown", beamCooldown);
         }
         // Add Miscellaneous folder to main menu
         menu += miscellaneous;


         // Create Player folder
         MenuFolder* player = new MenuFolder("Player Codes");
         {
             // add hotkeys to posEditor
             *player += new MenuEntry("Use position editor menu", posEditor);
             *player += new MenuEntry("Use Jinxed model", openJinxOptions);
             *player += new MenuEntry("Green Link", gJinx);
             *player += new MenuEntry("Blue Link", bJinx);
             *player += new MenuEntry("Red Link", bJinx);
             *player += new MenuEntry("Spawn/Despawn Links", openSpawnOptions);
             *player += new MenuEntry("Green Link", nullptr, gSpawn);
             *player += new MenuEntry("Blue Link", nullptr, bSpawn);
             *player += new MenuEntry("Red Link", nullptr, rSpawn);
             *player += new MenuEntry("Enable Water Storage", waterStorage);
             *player += new MenuEntry("Toggle Link Visibility", openVisibleOptions);
             *player += new MenuEntry("Green Link", nullptr, gVisible);
             *player += new MenuEntry("Blue Link", nullptr, bVisible);
             *player += new MenuEntry("Red Link", nullptr, rVisible);
             *player += new MenuEntry("Toggle Link Invincibility", openInvincibleOptions);
             *player += new MenuEntry("Green Link", gInvincible);
             *player += new MenuEntry("Blue Link", bInvincible);
             *player += new MenuEntry("Red Link", rInvincible);
             // I don't really know if I wanna add these... they seem a bit unnecessary tbh
             //*player += new MenuEntry("Change Individual Link Voice", openVoiceOptions);
                 //*player += new MenuEntry("Green Link", nullptr, gVoiceClient);
                 //*player += new MenuEntry("Blue Link", nullptr, bVoiceClient);
                 //*player += new MenuEntry("Red Link", nullptr, rVoiceClient);
             *player += new MenuEntry("Disable Collision", removeCollision);
             *player += new MenuEntry("Set Link model size", linkSize);
             *player += new MenuEntry("Enable PvP damage", pvpModeEnable);
             *player += new MenuEntry("Change Sword Types", openSwordOptions);
             *player += new MenuEntry("Green Link", nullptr, gSwordChoose);
             *player += new MenuEntry("Blue Link", nullptr, bSwordChoose);
             *player += new MenuEntry("Red Link", nullptr, rSwordChoose);
         }
         // Add Player folder to main menu
         menu += player;


         // Create Energy folder
         MenuFolder* energy = new MenuFolder("Energy Codes");
         {
             *energy += new MenuEntry("Infinite energy", infEnergy);
             *energy += new MenuEntry("Set max energy amount", nullptr, maxEnergySet);
             *energy += new MenuEntry("Set energy consumption multiplier", nullptr, energyConsumeMultiplier);
         }
         // Add Energy folder to main menu
         menu += energy;


         // Create Item folder
         MenuFolder* items = new MenuFolder("Item Codes");
         {
             *items += new MenuEntry("Set current item", openItemOptions);
             *items += new MenuEntry("Green Link", nullptr, gSetItem);
             *items += new MenuEntry("Blue Link", nullptr, bSetItem);
             *items += new MenuEntry("Red Link", nullptr, rSetItem);
             *items += new MenuEntry("Set Shadow Link item", openShadowItemOptions);
             *items += new MenuEntry("Shadow Green Link", nullptr, gShadowSetItem);
             *items += new MenuEntry("Shadow Blue Link", nullptr, bShadowSetItem);
             *items += new MenuEntry("Shadow Red Link", nullptr, rShadowSetItem);
             *items += new MenuEntry("Set strafing speeds", nullptr, strafingSpeedSet);
             *items += new MenuEntry("Always use upgraded Items", upgradeItemAlways);
         }
         // Add Item folder to main menu
         menu += items;


         // Create Rendering folder 
         MenuFolder* render = new MenuFolder("Rendering Codes");
         {
             // should these be part of freecam?
             *render += new MenuEntry("Hide HUD", hideHUD);
             *render += new MenuEntry("Disable fog effects", disableFog);
         }
         // Add Rendering folder to main menu
         menu += render;

         // Create Save folder
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
         // Add Save folder to main menu
         menu += save;

         // Create Sound folder
         MenuFolder* sound = new MenuFolder("BGM and SFX Codes");
         {
             *sound += new MenuEntry("Set current BGM", bgmSet);
             *sound += new MenuEntry("Set BGM volume", nullptr, bgmVolSet);
             *sound += new MenuEntry("Choose Lobby Ball song", lobbyBallSong);
             // not sure whether to add this
             // *sound += new MenuEntry("Set Lobby Ball volume", lobbyBallVol);
             *sound += new MenuEntry("Set Link Voice volume", nullptr, voiceVol);
             *sound += new MenuEntry("Set Low Health Alert volume", nullptr, lowHPVol);
             // not sure whether to add this
             //*sound += new MenuEntry("Set Level Completion Fanfare volume", levelDoneVol);
         }
         // Add Sound folder to main menu
         menu += sound;
     }

     int     main(void)
     {
         // title is left blank since the name is already set in the lib files
         PluginMenu* menu = new PluginMenu("", 0, 5, 0,
             "An advanced, region-free cheat plugin made for\nThe Legend of Zelda: Tri Force Heroes.\n\nForked from the original CTRPluginFramework\nblank template repository.");

         // Synchronize the menu with frame event
         menu->SynchronizeWithFrame(true);

         // Init our menu entries & folders
         InitMenu(*menu);

         // Launch menu and mainloop
         menu->Run();

         delete menu;

         // Exit plugin
         return (0);
     }

    // This patch the NFC disabling the touchscreen when scanning an amiibo, which prevents ctrpf to be used
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

        // This basically tells the plugin, "hey, figure out what region we are in, and initialize the correct
        // set of addresses to be used"
        AddressList::InitAddresses();


        ToggleTouchscreenForceOn();
    }

    // This function is called when the process exits
    // Useful to save settings, undo patches or clean up things
    void    OnProcessExit(void)
    {
        ToggleTouchscreenForceOn();
       
    }




















}
