#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{

    // NOTE: if an entry edits an address that does NOT automatically fix itself upon disabling the entry, we need
    // to create a piece of code that runs after said entry is disabled that restores default values!!

    void posEditor(MenuEntry* entry) {
        // TODO:
        // good question... not sure how to implement this
        // 
        // we could do something where:
        // - skip button and fairy indicator are re-sized and moved higher
        // - two new graphics are drawn below the skip button: load and save buttons... need to figure out what graphics to draw...
        // - to save position: swap to desired Link, then hit save button -- this will store the X, Y, and Z coords
        // - to load position: swap to desired Link, the hit load button -- this will write stored values to pos addresses
        // 
        // - note 1: if no position was saved for the current Link AND load button was pressed, display OSD notif on 
        // top screen saying that there's no saved position to load
        // - note 2: the code should check the current Link first

    }

    void openJinxOptions(MenuEntry* entry) {
        // TODO: when enabled, cause the 3 link color options to appear in the menu
    }

    void gJinx(MenuEntry* entry) {
        // TODO: simple 32-bit write to jinx address -> 0x00000000
        
        // I believe this address automatically resets itself, so there's no need to 
        // implement a "restore default value" piece of code
    }

    void bJinx(MenuEntry* entry) {
        // TODO: see gJinx()
    }

    void rJinx(MenuEntry* entry) {
        // TODO: see gJinx()
    }

    void openSpawnOptions(MenuEntry* entry) {
        // TODO: when enabled, cause the 3 link color options to appear in the menu
    }

    void gSpawn(MenuEntry* entry) {
        // TODO: simple 8-bit write to spawn address -> 0x00 or 0x01
    }

    void bSpawn(MenuEntry* entry) {
        // TODO: see gSpawn()
    }

    void rSpawn(MenuEntry* entry) {
        // TODO: see gSpawn()
    }

    void waterStorage(MenuEntry* entry) {
        // TODO: simple 24-bit write to water storage address -> 0x000000
        // and do the same for all 3 Links
    }

    void openVisibleOptions(MenuEntry* entry) {
        // TODO: when enabled, cause the 3 link color options to appear in the menu
    }

    void gVisible(MenuEntry* entry) {
       // TODO: simple 8-bit write to visibility address -> 0x00 or 0x01
    }
    
    void bVisible(MenuEntry* entry) {
        // TODO: see gVisible()
    }

    void rVisible(MenuEntry* entry) {
        // TODO: see gVisible()
    }

    void openInvincibleOptions(MenuEntry* entry) {
        // TODO: when enabled, cause the 3 link color options to appear in the menu
    }

    void gInvincible(MenuEntry* entry) {
        // TODO: simple 8-bit write to invincibility -> 0x00 or 0x16

    }

    void bInvincible(MenuEntry* entry) {
        // TODO: see gInvincible()
    }

    void rInvincible(MenuEntry* entry) {
        // TODO: see gInvincible()
    }

    /*
    // I don't really know if I wanna add these... they seem a bit unnecessary tbh

    void openVoiceOptions(MenuEntry* entry) {
        // TODO 1: when enabled, cause the 3 link color options to appear in the menu
        // TODO 2: since g/b/rVoiceClient() are all gear entries, we can't use those
        // to maintain/freeze the values... therefore we gotta do things a bit differently here

        // this function will be responsible for freezing the values:
        // - if a voice has not been set (via g/b/rVoiceChosen boolean), don't do anything
        // - if a voice is set, take the voice value from g/b/rVoiceClient() and freeze 
        // that value onto the voice address(es)

        // so basically:
        // if g/b/rVoiceChosen bool = true
        //      read value of g/b/rVoice int
        //      set g/b/r link voice to g/b/rVoice int value
        // else do nothing
    }

    int gVoiceClient(void) {
        // TODO: 
        // 1) open a Combo Box with the Voice options and return the voice option chosen
        // 2) set g/b/rVoiceChosen boolean to true (since this is a static boolean, we'll need to create it elsewhere)
    }

    int bVoiceClient(void) {
        // TODO: see gVoiceClient()
    }

    int rVoiceClient(void) {
        // TODO: see gVoiceClient()
    }

    */
    void removeCollision(MenuEntry* entry) {
        // TODO: simple 8-bit write to no collision address and repeat for all Links
        // Note: should we make this per individual Link?
    }

    void linkSize(MenuEntry* entry) {
        // TODO: open numerical keyboard and set inputted value as Link size
        // Note: be sure to add limits (0 < x < possible max float value) since you can't
        // use negative values
    }

    void pvpModeEnable(MenuEntry* entry) {
        // TODO: simple 8-bit write to pvp address -> 0x01 and repeat for all Links
    }

    void openSwordOptions(MenuEntry* entry) {
        // TODO: when enabled, cause the 3 link color options to appear in the menu
    }

    void gSwordChoose(MenuEntry* entry) {
        // TODO: open Combo Box and set chosen sword type
        // Does this address reset??
    }

    void bSwordChoose(MenuEntry* entry) {
        // TODO: see gSwordChoose()
    }

    void rSwordChoose(MenuEntry* entry) {
        // TODO: see rSwordChoose()
    }

}