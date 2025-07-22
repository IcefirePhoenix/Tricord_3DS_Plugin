[![GitHub Stars](https://img.shields.io/github/stars/IcefirePhoenix/Tricord_3DS_Plugin?style=flat)](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin)
[![GitHub release](https://img.shields.io/github/v/release/IcefirePhoenix/Tricord_3DS_Plugin?include_prereleases&sort=semver)](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/releases)
[![Last Commit](https://img.shields.io/github/last-commit/IcefirePhoenix/Tricord_3DS_Plugin)](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/commits)

# Tricord
A multipurpose utility plugin for The Legend of Zelda: Tri Force Heroes. Built upon CTRPluginFramework, which can be found [here](https://gitlab.com/thepixellizeross/ctrpluginframework) for reference.

### Supported Platforms
Compatible with most major regional releases of TFH on version 2.1.0. Due to a dwindling player base, support for the Korean version was dropped in July 2025.
- North America
- Europe/Australia
- Japan (Note: Taiwanese/Chinese releases are repackaged JP copies.)

Runnable on:
- Nintendo 2/3DS hardware running Luma3DS, v13.3 or higher.
- Citra emulator, preferably version r608383e. (See the [archived repository](https://github.com/PabloMK7/citra/releases/tag/r608383e) or  [official Citra website](https://citra-emulator.com/download)).

Older versions of Luma3DS / Citra may or may not be compatible. If Tricord causes a crash, freezes, or stalls on game boot, try updating to the latest Luma3DS / Citra version.

###  Major features
- Freecam with button-based controls
- Custom gameplay modes, some of which are also playable via online multiplayer
- Visual and UI improvement options
- Warp from anywhere, to anywhere
- Ability to edit the default emote selector menus
- Real-time respawn location trackers
- Savefile editor

A full list of features can be found in the [Wiki](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/wiki). Online Play is currently supported on Pretendo Network, and will continue to be supported for as long as the Pretendo servers are maintained.

## 📖 Data Sources
All data used to create this plugin has been manually datamined by the Development Team and can be found here:
- [Memory Addresses - documentation spreadsheet](https://docs.google.com/spreadsheets/d/1gg5JByPV6ilyAlF44R_v7AukZ8DQncjPCiPDLiAWxiI/edit?usp=sharing)
- [Data Values - documentation spreadsheet](https://docs.google.com/spreadsheets/d/1GXdPX1AcOjoOjx7bS8-YmCEngs5gXZHqlwXEymGrk80/edit?usp=sharing)

If you would like to contribute to either of these sources, please contact someone on the Development Team. See the Contact section below.

## 🎮 Emulator Compatibility Notes

### Azahar Compatibility
We currently do not support Azahar. While Tricord does run on the [latest release](https://azahar-emu.org/pages/download/), many entries are broken.

### Citra Compatibility
Some menu entries may not work as expected on Citra. Your mileage may vary depending on the specific Citra build, though the following entries are known to have issues running on the latest release.

As of our last tests on build r608383e:
- Force Instant Text Boxes
- Show Treasure Chest Contents
- Lobby Emote Swapper
- Drablands Emote Swapper

This plugin works by manipulating RAM data, and since Citra's memory layout may not be 1:1 with original hardware memory, we can't always guarantee compatibility.

In addition, it has been found that having mods installed on Citra will cause the majority of Tricord's entries to have no effect. It is recommended to disable mods before using Tricord. To do this, right-click on Tri Force Heroes in the Citra title list. Click "Open Mods Location". From there, you can backup and delete the `romfs` folder. Or, alternatively, rename the `romfs` folder to something else, for example, `romfs-disabled`. This will prevent the mods from being applied -- making Tricord more usable -- while keeping the mod's files in the same location. This way, the mods can be easily reinstalled whenever you want by renaming the folder back to `romfs`.

## 🐛 Bug-catching

Tricord is a passion project developed by our team during our own personal free time. Between real-life commitments like work, school, and everything else, mistakes can (and do!) happen. While we do our best to squash as many bugs as we can during testing, some are sneaky enough to slip by!

If you encounter a bug, please let us know! You can do this by opening an Issue in this repository and providing the following information in your post:
- Hardware model OR Citra emulator build
- A copy of the error log as an attachment
- Description of the issue
- Exact steps to replicate the issue

To save an error log to your SD card, be sure to press X while in the crash screen. The log can be found under `Tricord/Logs/[NA/EU/JP]`.

If you do not have a GitHub account, you can also reach out via the [TFH Modding Discord server](https://discord.gg/P6csr4jc5U). Your feedback is greatly appreciated and helps make this project better for everyone!

## ⚠️ HID-related ErrDisp and wumiibo
CTRPF plugins, including Tricord, may conflict with existing installations of wumiibo depending on your console model. Before troubleshooting, it is recommended to try loading the default plugin found [here](https://github.com/PabloMK7/CTRPluginFramework-BlankTemplate/releases) to verify that the issue is not Tricord-specific. If the problem persists, it is likely due to wumiibo. From here, you can either:

- Disable wumiibo or uninstall it completely.
- OR if you do not want to remove wumiibo: try toggling **Disable HID memory allocation** in the Settings menu then restarting your console.

If the problem still persists OR if you are not a wumiibo user, please contact someone on the Development Team with a screenshot of your ErrDisp details. See the Contact section below.

## 🛠️ Installation Instructions

### 2/3DS Console

1. Make sure you are running the latest version of [Luma3DS](https://github.com/LumaTeam/Luma3DS/releases).
2. Download the latest `Tricord.zip` and `Tricord.3gx` files from the [releases page](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/releases).
3. Take your SD card out of your console and insert it into your computer (or connect to it via FTP). Unzip `Tricord.zip` and place the `Tricord` folder onto the root of the SD card.
4. Navigate to `luma/plugins/` on the SD card (if this folder doesn't exist, create it). Create folders for each title ID of Tri Force Heroes that you intend to use the plugin on (list of title IDs below). Place a copy of `Tricord.3gx` into each folder. This ensures the plugin **only** runs for Tri Force Heroes titles.
5. Put the SD card back in your console and power it on (or close the FTP connection). Go to the Rosalina menu (L+Down+Select), make sure "Enable plugin loader" is checked on.
6. Boot your Tri Force Heroes title and you should see the top screen flash blue for a moment and then the message "Plugin ready! Tricord can now be started." You can access the plugin by pressing Select while the game is running (close the menu with the same button). For details on how to use the various Tricord features, peruse the [Wiki](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/wiki).

### Citra

1. Make sure you are running the latest version of Citra, found on the [archived repository](https://github.com/PabloMK7/citra/releases/tag/r608383e) or the [official Citra website](https://citra-emulator.com/download).
2. Download the latest `Tricord.zip` and `Tricord.3gx` files from the [releases page](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/releases).
3. On Citra, go to File > Open Citra Folder. Navigate to `sdmc`. Unzip `Tricord.zip` and place the `Tricord` folder inside `sdmc`.
4. Navigate to `sdmc/luma/plugins/` (if this folder doesn't exist, create it). Create folders for each title ID of Tri Force Heroes that you intend to use the plugin on (list of title IDs below). Place a copy of `Tricord.3gx` into each folder. This ensures the plugin only runs for Tri Force Heroes titles. You can now close the file explorer window.
5. Go to Emulation > Configure > System. Check the box that says "Enable 3GX plugin loader".
6. Boot your Tri Force Heroes title. Wait for the message "Plugin ready! Tricord can now be started." You can access the plugin by pressing Select while the game is running (close the menu with the same button). For details on how to use the various Tricord features, peruse the [Wiki](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/wiki).

#### Title IDs:
* 0004000000176F00 - North America
* 0004000000177000 - Europe
* 004000000176E00 - Japan

### Updating
Download the latest files from the [releases page](https://github.com/IcefirePhoenix/Tricord_3DS_Plugin/releases) and reinstall them to the SD card (or emulated SD card) in the same places as described above, overwriting previous files. Don't worry about other newly generated files that you may find in the plugins folder (such as `CTRPFData.bin` or .txt files); they usually do not need to be removed or replaced **unless explicitly mentioned in a release**.

## ⚙️ Build Policy
The Tricord developer team is partnered with the TFH Matchmaking Discord Server. As part of that collaboration, we are committed to promoting  safe and responsible use of Tricord.
To deter users from modifying Tricord and using it to intentionally disrupt online gameplay, we do not provide instructions nor the means to build the plugin from source.

Please find a ready-to-use build in the Releases page. **Attempting to build the plugin using the makefile from the original CTRPF repository will fail.**

## 📫 Contact
For inquiries and assistance, please contact someone on the Developer Team via the [TFH Modding Discord server](https://discord.gg/P6csr4jc5U). You may also join this server to see updates on the Tricord plugin and chat with the TFH modding community.

#### Developer Team

Our team has done extensive work in reverse engineering various components of TFH, including RAM layouts, the RomFS (file assets), and the game's compiled executable. This research involves identifying variables, flags, and other key data stored in memory, analyzing how in-game data is updated and accessed in real time, and interpreting assembly-level code to better understand gameplay logic / behavior. By identifying memory addresses that manage this data, we’ve been able to develop various quality-of-life features and extra gameplay content found in Tricord!

Wintenix - Lead developer.

Peanut - Developer. Created several major features, numerous secondary features, and game modes. Creator of the Tricord Wiki page.

Glem - Contributed extensively to the documentation of internal game mechanics through memory address research and testing.

#### Credits
The Pixellizer Group (notably Nanquitas and PabloMK7) - [CTRPluginFramework library](https://gitlab.com/thepixellizeross/ctrpluginframework)

Neils Lohmann - [JSON for Modern C++ library](https://github.com/nlohmann/json)

Nayuki - [QR Code generator library](https://www.nayuki.io/page/qr-code-generator-library)
