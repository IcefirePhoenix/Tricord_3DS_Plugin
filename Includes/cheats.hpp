#ifndef CHEATS_HPP
#define CHEATS_HPP

#include "AddressList.hpp"
#include "Main/Costume.hpp"
#include "Main/Emotes.hpp"
#include "Main/Gameplay.hpp"
#include "Main/Item.hpp"
#include "Main/Miscellaneous.hpp"
#include "Main/Player.hpp"
#include "Main/Rendering.hpp"
#include "Main/Save.hpp"
#include "Main/BGM_SFX.hpp"

#include "Main/AltModes/Chaos.hpp"
#include "Main/AltModes/Freecam.hpp"

namespace CTRPluginFramework {
    void InitFreecamCodes(PluginMenu& menu);
    void InitGameModes(PluginMenu& menu);
    void InitGameplayFolder(PluginMenu& menu);
    void InitEmoteFolder(PluginMenu& menu);
    void InitCostumeFolder(PluginMenu& menu);
    void InitMiscFolder(PluginMenu& menu);
    void InitPlayerFolder(PluginMenu& menu);
    void InitEnergyFolder(PluginMenu& menu);
    void InitItemsFolder(PluginMenu& menu);
    void InitRenderFolder(PluginMenu& menu);
    void InitSaveFolder(PluginMenu& menu);
    void InitSoundFolder(PluginMenu& menu);
    void AddAllFolders(PluginMenu& menu);
}

#endif
