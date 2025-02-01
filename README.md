A multipurpose cheat utility plugin for The Legend of Zelda: Tri Force Heroes. Based on the CTRPluginFramework written by Nanquitas and managed by ThePixellizer group. The original framework library can be found [here](https://gitlab.com/thepixellizeross/ctrpluginframework/-/tree/develop/Library?ref_type=heads) for reference.

Compatible with most major regional releases of TFH on version 2.1.0:
- North America
- Europe/Australia
- Japan (Note: Taiwan/Chinese releases are repackaged JP copies)

Runnable on:
- Nintendo 2/3DS hardware running Luma3DS, v13.3 or higher.
- Citra emulator, release r608383e.

Older versions of Luma3DS / Citra may or may not be compatible. If the plugin causes a crash or freezing/stalling on game boot, update to the latest Luma3DS / Citra version.

Online Play is currently supported on the Pretendo Network, and will continue to be supported for as long as the Pretendo servers are maintained.

Major features include:
- Freecam with button-based controls
- Custom gameplay modes, some of which are also playable via online multiplayer
- Visual and UI improvement option(s) for vanilla gameplay
- Ability to edit the default emote selector(s)
- Live/real-time respawn location trackers
- Savefile editor

A full list of features can be found in the [Wiki](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/wiki).

## Data Sources
All data used in the development of this plugin has been manually datamined by the Tricord team and can be found here:
- [Memory address documentation spreadsheet](https://docs.google.com/spreadsheets/d/1gg5JByPV6ilyAlF44R_v7AukZ8DQncjPCiPDLiAWxiI/edit?usp=sharing)
- [Data values documentation spreadsheet](https://docs.google.com/spreadsheets/d/1GXdPX1AcOjoOjx7bS8-YmCEngs5gXZHqlwXEymGrk80/edit?usp=sharing)

If you would like to contribute to either of these sources, please contact Wintenix on Discord. See the Contact section below.

## Citra Compatibility Notes
Some menu entries may not work as expected on Citra. Your mileage may vary depending on the specific Citra build you are using, though the following entries are known to have issues running on the following builds of Citra. 

As of our last tests on Canary 2765 | Nightly 2087:
- Force Instant Text Boxes
- Show Treasure Chest Contents
- Lobby Emote Swapper
- Drablands Emote Swapper

This plugin manipulates RAM data, and since Citra's memory layout may not be 1:1 with original hardware memory, we can't always guarantee compatibility.

In addition, it has been found that having mods installed on Citra will cause the majority of Tricord's codes to have no effect. It is recommended to disable mods before using Tricord. To do this, right-click on Tri Force Heroes in the Citra title list. Click "Open Mods Location". From there, you can backup and delete the `romfs` folder. Or, alternatively, rename the `romfs` folder to something else, for example, `romfs-disabled`. This will prevent the mods from being applied, making Tricord more usable, while keeping the mod's files in the same location. This way, the mods can be easily reinstalled whenever you want by renaming the folder back to `romfs`.

## Bug-catching 
If you come across any bugs, please open up an Issue in this repository and provide the following information in your post:
- Hardware model OR Citra emulator build
- Description of the issue
- Exact steps to replicate the issue

If you have come across a consistent crash, please be sure to upload an image of the crash screen/log.

## HID-related ErrDisp
CTRPF plugins, including Tricord, may conflict with existing installations of wumiibo depending on your console model. If you are consistently running into ErrDisp errors related to the HID module and are NOT a wumiibo user, please contact Wintenix with a screenshot of your ErrDisp details. 

Before troubleshooting, it is recommended to try loading the default plugin found [here](https://github.com/PabloMK7/CTRPluginFramework-BlankTemplate/releases) to verify that the issue is not Tricord-specific. If the problem persists, it is likely due to wumiibo. From here, you can either:

- Disable wumiibo or uninstall it completely.
- OR if you do not want to disable/uninstall wumiibo: try toggling **Disable HID memory allocation** in the Settings menu *and* restarting your console.

If the problem still persists, please contact Wintenix with a screenshot of your ErrDisp details. See the Contact section below.

## Installation Instructions

### 2/3DS Console

1. Make sure you are running the latest version of [Luma3DS](https://github.com/LumaTeam/Luma3DS/releases).
2. Download the latest `Tricord.zip` and `Tricord.3gx` files from the [releases page](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/releases).
3. Take your SD card out of your console and insert it into your computer (or connect to it via FTP). Unzip `Tricord.zip` and place the `Tricord` folder onto the root of the SD card.
4. Navigate to `luma/plugins/` on the SD card (if this folder doesn't exist, create it). Create folders for each title ID of Tri Force Heroes that you intend to use the plugin on (list of title IDs below). Place a copy of `Tricord.3gx` into each folder. This ensures the plugin only runs for Tri Force Heroes titles.
5. Put the SD card back in your console and power it on (or close the FTP connection). Go to the Rosalina menu (L+Down+Select), make sure "Enable plugin loader" is checked on.
6. Boot your Tri Force Heroes title and you should see the top screen flash blue for a moment and then the message "Plugin ready! Tricord can now be started." You can access the plugin by pressing Select while the game is running (close the menu with the same button). For details on how to use the various Tricord features, peruse the [Wiki](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/wiki).

### Citra

1. Make sure you are running the latest version of Citra, found on the [archived repository](https://github.com/PabloMK7/citra/releases/tag/r608383e) or the [official Citra website](https://citra-emulator.com/download).
2. Download the latest `Tricord.zip` and `Tricord.3gx` files from the [releases page](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/releases).
3. On Citra, go to File > Open Citra Folder. Navigate to `sdmc`. Unzip `Tricord.zip` and place the `Tricord` folder inside `sdmc`.
4. Navigate to `sdmc/luma/plugins/` (if this folder doesn't exist, create it). Create folders for each title ID of Tri Force Heroes that you intend to use the plugin on (list of title IDs below). Place a copy of `Tricord.3gx` into each folder. This ensures the plugin only runs for Tri Force Heroes titles. You can now close the file explorer window.
5. Go to Emulation > Configure > System. Check the box that says "Enable 3GX plugin loader".
6. Boot your Tri Force Heroes title. Wait for the message "Plugin ready! Tricord can now be started." You can access the plugin by pressing Select while the game is running (close the menu with the same button). For details on how to use the various Tricord features, peruse the [Wiki](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/wiki).

Title IDs:
* 0004000000176F00 - North America
* 0004000000177000 - Europe
* 0004000000176E00 - Japan

### Updating
Download the latest files from the [releases page](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/releases) and reinstall them to the SD card (or emulated SD card) in the same places as described above, overwriting previous files. Do not worry about other newly generated files that you may find in the plugins folder (such as `CTRPFData.bin` or .txt files); they do not need to be removed or replaced unless explicitly mentioned in a release.

## Build Instructions
The plugin has already been compiled and can be found in the Releases page. However, if you would like to build the plugin yourself, there are Makefiles provided in this repository. One is for building the library, and the other is for building the plugin itself. 

The library only needs to be re-built if it has been edited. Otherwise, any pre-existing .a files will be used for plugin compilation.

## Contact
For inquiries and assistance, please contact Wintenix on Discord @wintenix, via the [TFH Modding Discord server](https://discord.gg/P6csr4jc5U). You may also join this server to see updates on the Tricord plugin and chat with the TFH modding community.

### Credits
Wintenix - Lead, developer of most major features.

Peanut - Developer of a few major features and many secondary features and game modes. Creator of the Tricord Wiki page.

Glem - Documentor of many memory addresses across all regions.
