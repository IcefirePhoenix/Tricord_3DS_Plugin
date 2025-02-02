#ifndef FOLDERENTRIES_HPP
#define FOLDERENTRIES_HPP

#include "CTRPF.hpp"

namespace CTRPluginFramework
{
    extern MenuFolder *emotes;
    extern MenuFolder *chaos;
    extern MenuFolder *turbo;
    extern MenuFolder *effects;
    extern MenuFolder *gameplay;
    extern MenuFolder *warp;
    extern MenuFolder *healthFairies;
    extern MenuFolder *energy;
    extern MenuFolder *enemies;
    extern MenuFolder *physics;
    extern MenuFolder *moonJumpFlight;
    extern MenuFolder *costume;
    extern MenuFolder *miscellaneous;
    extern MenuFolder *player;
    extern MenuFolder *items;
    extern MenuFolder *render;
    extern MenuFolder *save;
    extern MenuFolder *sound;

    void InitFreecamCodes(PluginMenu &menu);
    void InitGameModes(PluginMenu &menu);
    void InitGameplayFolder(PluginMenu &menu);
    void InitEmoteFolder(PluginMenu &menu);
    void InitCostumeFolder(PluginMenu &menu);
    void InitMiscFolder(PluginMenu &menu);
    void InitPlayerFolder(PluginMenu &menu);
    void InitEnergyFolder(PluginMenu &menu);
    void InitItemsFolder(PluginMenu &menu);
    void InitRenderFolder(PluginMenu &menu);
    void InitSaveFolder(PluginMenu &menu);
    void InitSoundFolder(PluginMenu &menu);
    void InitTFH_Settings(PluginMenu &menu);
    void AddAllFolders(PluginMenu &menu);

    void ToggleMenuChange(Time time);
    void CreateMenu(PluginMenu &menu);

    static MenuEntry *EntryWithHotkey(MenuEntry *entry, const std::vector<Hotkey> &hotkeys)
    {
        if (entry != nullptr)
        {
            for (const Hotkey &hotkey : hotkeys)
                entry->Hotkeys += hotkey;
        }
        return entry;
    }
}

#endif
