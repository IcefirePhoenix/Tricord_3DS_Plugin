A multipurpose cheat utility plugin for The Legend of Zelda: Tri Force Heroes. Based on the CTRPluginFramework written by Nanquitas and managed by ThePixellizer group. The original framework library can be found [here](https://gitlab.com/thepixellizeross/ctrpluginframework/-/tree/develop/Library?ref_type=heads) for reference.

Compatible with most major regional releases of TFH on version 2.1.0:
- North America
- Europe/Australia
- Japan (Note: Taiwan/Chinese releases are repackaged JP copies)

Runnable on:
- Nintendo 2/3DS hardware running Luma3DS, preferably v13.0 or higher.
- Citra emulator 
  - Canary build: version 2765 or higher
  - Nightly build: version 2087 or higher

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

In addition, it has been found that having mods installed on Citra will cause the majority of Tricord's codes to have no effect. It is recommended to disable mods before using Tricord. To do this, right-click on Tri Force Heroes in the Citra title list. Click "Open Mods Location". From there, you can backup and delete the `romfs` folder. Or, alternatively, rename the `romfs` folder to something else, for example, `romfs-disabled`. This will prevent the mods from being applied, making Tricord fully usable, while keeping the mod's files in the same location. This way, the mods can be easily reinstalled whenever you want by renaming the folder back to `romfs`.

## Bug-catching 
If you come across any bugs, please open up an Issue in this repository and provide the following information in your post:
- Hardware model OR Citra emulator build
- Description of the issue
- Exact steps to replicate the issue

If you have come across a consistent crash, please be sure to upload an image of the crash screen/log.

## HID-related ErrDisp
CTRPF plugins, including Tricord, may conflict with existing installations of wumiibo depending on your console model. If you are consistently running into ErrDisp errors related to the HID module and are NOT a wumiibo user, please contact Wintenix with a screenshot of your ErrDisp details. 

Before troubleshooting, it is recommended to try loading the default plugin found [here](https://github.com/PabloMK7/CTRPluginFramework-BlankTemplate/releases/tag/v0.7.4) to verify that the issue is not Tricord-specific. If the problem persists, it is likely due to wumiibo. From here, you can either:

- Disable wumiibo or uninstall it completely.
- OR if you do not want to disable/uninstall wumiibo: try toggling **Disable HID memory allocation** in the Settings menu *and* restarting your console.

If the problem still persists, please contact Wintenix with a screenshot of your ErrDisp details. See the Contact section below.

## Build Instructions
The plugin has already been compiled and can be found in the Releases page. However, if you would like to build the plugin yourself, there are Makefiles provided in this repository. One is for building the library, and the other is for building the plugin itself. 

The library only needs to be re-built if it has been edited. Otherwise, any pre-existing .a files will be used for plugin compilation.

## Contact
For inquiries and assistance, please contact Wintenix on Discord @wintenix, via the [TFH Modding Discord server](https://discord.gg/P6csr4jc5U). You may also join this server to see updates on the Tricord plugin and chat with the TFH modding community.

### Credits
Wintenix - Lead, developer of most major features.

Peanut - Developer of a few major features and many secondary features and game modes. Creator of the Tricord Wiki page.

Glem - Documentor of many memory addresses across all regions.
