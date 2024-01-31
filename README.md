A multipurpose cheat plugin for The Legend of Zelda: Tri Force Heroes. Based on the CTRPluginFramework written by Nanquitas and managed by ThePixellizer group. The original framework library can be found [here](https://gitlab.com/thepixellizeross/ctrpluginframework/-/tree/develop/Library?ref_type=heads) for reference.

Compatible with all major regional releases of TFH on version 2.1.0:
- North America
- Europe/Australia
- Japan (Note: Taiwan/Chinese releases are repackaged JP copies)
- Korea

Runnable on:
- Nintendo 2/3DS hardware running Luma3DS, preferably v13.0 or higher.
- Citra emulator 
  - Canary build: version 2765 or higher
  -  Nightly build: version 2087 or higher

Online Play is currently supported on Nintendo Network, and will continue to be supported until official server shutdown on April 8, 2024. Pretendo Network support is TBD.

Major features include:
- Custom level map loader (requires additional configuration detailed in the wiki)
- Freecam, with touchscreen or button-based controls
- Custom gameplay modes, which are also playable via online multiplayer
- Visual and UI improvement option(s) for vanilla gameplay
- Ability to edit the default emote selector(s)
- Live/real-time respawn location trackers
- Savefile editor

A full list of features can be found in the wiki.

## Data Sources
All data used in the development of this plugin has been manually datamined by the Tricord team and can be found here:
- Memory address documentation spreadsheet: [link]
- RomFS documentation: [link]

If you would like to contribute to either of these sources, please contact WinterPhoenix on Discord @snowfirebirb.

## Citra Compatibility Notes
Some menu entries may not work as expected on Citra. Your mileage may vary depending on the specific Citra build you are using, though the following entries are known to have issues running on the latest builds of Citra. 

As of our last tests on Canary 2765 | Nightly 2087:
- Force Instant Text Boxes
- Show Treasure Chest Contents
- Lobby Emote Swapper
- Drablands Emote Swapper

This plugin manipulates RAM data, and since Citra's memory layout may not be 1:1 with original hardware memory, we can't always guarantee compatibility.

## Bug-catching 
If you come across any bugs, please open up an Issue in this repository and provide the following information in your post:
- hardware model OR Citra emulator build
- description of the issue
- exact steps to replicate the issue

If you have come across a consistent crash, please be sure to upload an image of the generated QR code displayed on the crash screen, as shown below:

[screenshot] 

## Build instructions:
The plugin has already been compiled and can be found in the Releases page. However, if you would like to build the plugin yourself, there are Makefiles provided in this repository. One is for building the library, and the other is for building the plugin itself. 

The library only needs to be re-built if it has been edited. Otherwise, any pre-existing .a files will be used for plugin compilation.

### Credits
WinterPhoenix - 

Peanut/Sil - 

Glem - 
