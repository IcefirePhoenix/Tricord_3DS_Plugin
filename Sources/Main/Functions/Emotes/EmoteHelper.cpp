#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // layout A: over here, bye, lets go, hello, cheer, frown, no, thumbs up
    // layout B: item, throw, totem, yawn, cheer, frown, nooo, blank

    u32 Emotes::graphicsAddresses[8] = {0};
    std::vector<u8> currentEmoteLayout;
    bool useBlankEmote;

    // column order: NA, EU, JP
    const u32 Emotes::greenEmotePointers[12][3] =
    {
        {0x20307780, 0x208E9080, 0x20330C80}, // over here
        {0x20308780, 0x208EA080, 0x20331C80}, // item
        {0x20309780, 0x208EB080, 0x20332C80}, // throw
        {0x2030A780, 0x208EC080, 0x20333C80}, // shrug/bow
        {0x2030B780, 0x208ED080, 0x20334C80}, // no
        {0x2030C780, 0x208EE080, 0x20335C80}, // hello
        {0x2030D780, 0x208EF080, 0x20336C80}, // lets go
        {0x20C86080, 0x20D1B480, 0x20C6A680}, // totem
        {0x20C87080, 0x20D1C480, 0x20C37680}, // cheer
        {0x20C88080, 0x20D1D480, 0x20C38680}, // thumbs up
        {0x20C89080, 0x20D1E480, 0x20C39680}, // yawn
        {0x20C8A080, 0x20D1F480, 0x20C3A680}  // bye
    };

    const u32 Emotes::blueEmotePointers[12][3] =
    {
        {0x20300780, 0x208E2080, 0x20329C80}, // over here
        {0x20301780, 0x208E3080, 0x2032AC80}, // item
        {0x20302780, 0x208E4080, 0x2032BC80}, // throw
        {0x20303780, 0x208E5080, 0x2032CC80}, // shrug/bow
        {0x20304780, 0x208E6080, 0x2032DC80}, // no
        {0x20305780, 0x208E7080, 0x2032EC80}, // hello
        {0x20306780, 0x208E8080, 0x2032FC80}, // lets go
        {0x20C81080, 0x20D16480, 0x20C31680}, // totem
        {0x20C82080, 0x20D17480, 0x20C32680}, // cheer
        {0x20C83080, 0x20D18480, 0x20C33680}, // thumbs up
        {0x20C84080, 0x20D19480, 0x20C34680}, // yawn
        {0x20C85080, 0x20D1A480, 0x20C35680}  // bye
    };

    const u32 Emotes::redEmotePointers[12][3] =
    {
        {0x2030E780, 0x208F0080, 0x20337C80}, // over here
        {0x2030F780, 0x208F1080, 0x20338C80}, // item
        {0x20310780, 0x208F2080, 0x20339C80}, // throw
        {0x20311780, 0x208F3080, 0x2033AC80}, // frown/bow
        {0x20312780, 0x208F4080, 0x2033BC80}, // no
        {0x20313780, 0x208F5080, 0x2033CC80}, // hello
        {0x20314780, 0x208F6080, 0x2033DC80}, // lets go
        {0x20C8B080, 0x20D20480, 0x20C3B680}, // totem
        {0x20C8C080, 0x20D21480, 0x20C3C680}, // cheer
        {0x20C8D080, 0x20D22480, 0x20C3D680}, // thumbs up
        {0x20C8E080, 0x20D23480, 0x20C3E680}, // yawn
        {0x20C8F080, 0x20D24480, 0x20C3F680}  // bye
    };

    /* ------------------ */

    // Places relevant addresses into array for easier access
    void initEmoteAddresses(void)
    {
        Emotes::graphicsAddresses[0] = AddressList::getAddress("EmoteAGraphic");
        Emotes::graphicsAddresses[1] = AddressList::getAddress("EmoteBGraphic");
        Emotes::graphicsAddresses[2] = AddressList::getAddress("EmoteCGraphic");
        Emotes::graphicsAddresses[3] = AddressList::getAddress("EmoteDGraphic");
        Emotes::graphicsAddresses[4] = AddressList::getAddress("EmoteEGraphic");
        Emotes::graphicsAddresses[5] = AddressList::getAddress("EmoteFGraphic");
        Emotes::graphicsAddresses[6] = AddressList::getAddress("EmoteGGraphic");
        Emotes::graphicsAddresses[7] = AddressList::getAddress("EmoteHGraphic");
    }

    // Force-refreshes bottom-screen emote menu graphics
    void refreshEmoteGraphics(const u32 color[][3], std::vector<u8> &layout, bool useBlank)
    {
        int region;
        switch (Process::GetTitleID())
        {
            case TID_USA:
                region = 0;
                break;

            case TID_EUR:
                region = 1;
                break;

            case TID_JPN:
                region = 2;
                break;

            default:
                OSD::Notify("Emote Swapper: Cannot determine region.", Color::Red);
                break;
        }

        // update graphic pointers using hardcoded array...
        for (int i = 0; i < 8; i++)
        {
            // if blank emote should be used, assign it to the last emote slot...
            if (useBlank && i == 7)
                Process::Write32(Emotes::graphicsAddresses[i], Emotes::blankEmotePointer);
            else
                Process::Write32(Emotes::graphicsAddresses[i], color[layout[i]][region]);
        }
    }

    // Determines which set of emote graphics to refresh
    void initGraphicsRefresh(bool useBlankEmote)
    {
        switch (GeneralHelpers::getCurrLink())
        {
            case 0:
                refreshEmoteGraphics(Emotes::greenEmotePointers, currentEmoteLayout, useBlankEmote);
                break;
            case 1:
                refreshEmoteGraphics(Emotes::blueEmotePointers, currentEmoteLayout, useBlankEmote);
                break;
            case 2:
                refreshEmoteGraphics(Emotes::redEmotePointers, currentEmoteLayout, useBlankEmote);
                break;
        }
    }

    // Update emote button IDs and manually update bottom-screen emote menu graphics
    void initEmoteValueLayout(u32 address, u32 buttonID_A, u32 buttonID_B, bool isBlankEmote, std::vector<u8> pointerVector)
    {
        Process::Patch(address, buttonID_A);       // edit button IDs (1st half)
        Process::Patch(address + 0x4, buttonID_B); // edit button IDs (2nd half)

        currentEmoteLayout = pointerVector; // config pointers to emote graphics

        initGraphicsRefresh(isBlankEmote); // update emote graphics
    }

    // Disables custom emote edits by force-switching to Drablands emote set
    // Note: custom emote layouts are applied ONLY onto Den of Trials emote set
    void Emotes::forceDefaultEmotes(bool shouldDisable)
    {
        u32 forceDotLayoutEdit = shouldDisable ? 0xEA00000D : 0x0A00000D; // edit : default
        Process::Patch(AddressList::getAddress("UseDoTLayoutAlways"), forceDotLayoutEdit);
    }
}