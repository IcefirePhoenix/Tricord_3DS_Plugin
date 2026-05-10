#include "Cheats.hpp"
#include "Helpers/MenuHelpers/FolderEntries.hpp"

namespace CTRPluginFramework
{
    MenuFolder *autoFolder = new MenuFolder("Auto");
    MenuFolder *emotes = nullptr;
    MenuFolder *chaos = nullptr;
    MenuFolder *turbo = nullptr;
    MenuFolder *effects = nullptr;
    MenuFolder *gameplay = nullptr;
    MenuFolder *warp = nullptr;
    MenuFolder *bookmarks = nullptr;
    MenuFolder *healthFairies = nullptr;
    MenuFolder *energy = nullptr;
    MenuFolder *enemies = nullptr;
    MenuFolder *physics = nullptr;
    MenuFolder *moonJumpFlight = nullptr;
    MenuFolder *stage = nullptr;
    MenuFolder *costume = nullptr;
    MenuFolder *miscellaneous = nullptr;
    MenuFolder *player = nullptr;
    MenuFolder *items = nullptr;
    MenuFolder *render = nullptr;
    MenuFolder *save = nullptr;
    MenuFolder *sound = nullptr;

    void CreateMenu(PluginMenu &menu)
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
        InitTFH_Settings(menu);
        InitStarMarkSettings(menu);

        AddAllFolders(menu);
    }

    void AddAllFolders(PluginMenu &menu)
    {
        menu += costume;
        menu += emotes;
        menu += player;
        menu += gameplay;
        menu += items;
        menu += render;
        menu += save;
        menu += sound;
        menu += miscellaneous;

        // menu.Append(chaos, 2);
        menu.Append(turbo, 2);

        // auto entries added to menu for logging purposes but hidden from users
        menu += autoFolder;
        autoFolder->HideWithoutDisable();
        menu.AddToHidden(autoFolder);
    }
}