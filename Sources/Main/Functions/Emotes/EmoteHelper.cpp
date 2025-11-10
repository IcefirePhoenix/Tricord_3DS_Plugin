#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    u32 Emotes::graphicsAddresses[8] = {0};
    std::vector<u8> currentEmoteLayout;
    bool useBlankEmote;

    // (Indexes taken from Data Documentation Spreadsheet)

    // Note: Because language-specific emotes are stored separately from universal emotes, multiple files need to be accessed and thus some indexes will naturally overlap...
    // To prevent confusion between which file to access (ex: if given index 1, does that mean the first emote from the language set or the universal set?), the indexes
    // are stacked into one continuous range. To determine which file to access, simply check what range the index falls under and subtract an offset if needed.

    // Ranges: 0 -> 20 = SealUSen.ctpk | 21 - 35 = SealCommon.ctpk

    // Layout A: over here, bye, lets go, hello, cheer, frown, no, thumbs up
    // Layout B: item, throw, totem, yawn, cheer, frown, nooo, blank

    // Column order: G, B, R | Row order: emote IDs | Values: indexes used to calculate texture pointer
    const u32 Emotes::pointerIndexes[12][3] =
    {
        {7, 0, 14}, // over here = 0x0
        {8, 1, 15}, // item = 0x1
        {9, 2, 16}, // throw = 0x2
        {10, 3, 17}, // shrug/bow = 0x3
        {11, 4, 18}, // no = 0x4
        {12, 5, 19}, // hello = 0x5
        {13, 6, 20}, // lets go = 0x6
        {26, 21, 31}, // totem = 0x7
        {27, 22, 32}, // cheer = 0x8
        {28, 23, 33}, // thumbs up = 0x9
        {29, 24, 34}, // yawn = 0xA
        {30, 25, 35}  // bye = 0xB
    };

    /* ------------------ */

    // Places relevant addresses into array for easier access
    void Emotes::initEmoteAddresses(void)
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
    void refreshEmoteGraphics(bool useBlank)
    {
        int color = GeneralHelpers::getCurrLink();
        int texPtr = 0x0;

        for (int i = 0; i < 8; i++)
        {
            int ptrIndex = Emotes::pointerIndexes[currentEmoteLayout[i]][color];

            // access correct CTPK file, adjust index if needed
            if (ptrIndex > 20)
                texPtr = IconMngr::retrieveTexPtr(IconPtrSet::SealCommonCTPK, ptrIndex - 21, 0x1000);
            else
                texPtr = IconMngr::retrieveTexPtr(IconPtrSet::SealRegionalCTPK, ptrIndex, 0x1000);

            // if blank emote should be used, assign it to the last emote slot...
            if (useBlank && i == 7)
                Process::Write32(Emotes::graphicsAddresses[i], Emotes::blankEmotePtr);
            else
                Process::Write32(Emotes::graphicsAddresses[i], texPtr);
        }
    }

    // Update emote button IDs and manually update bottom-screen emote menu graphics
    void initEmoteValueLayout(u32 address, u32 buttonID_A, u32 buttonID_B, bool isBlankEmote, std::vector<u8> pointerVector)
    {
        Process::Patch(address, buttonID_A);       // edit button IDs (1st half)
        Process::Patch(address + 0x4, buttonID_B); // edit button IDs (2nd half)

        currentEmoteLayout = pointerVector; // config pointers to emote graphics

        refreshEmoteGraphics(isBlankEmote); // update emote graphics
    }

    // Disables custom emote edits by force-switching to Drablands emote set
    // Note: custom emote layouts are applied ONLY onto Den of Trials emote set
    void toggleDefaultEmotes(bool shouldDisableEdits)
    {
        u32 forceDotLayoutEdit = shouldDisableEdits ? 0xEA00000D : 0x0A00000D; // edit : default
        Process::Patch(AddressList::getAddress("UseDoTLayoutAlways"), forceDotLayoutEdit);
    }

    // The previous location seems to influence which emote set is currently in-use
    // To avoid complicated checks determining which set to use, simply copy over edits from Gameplay -> Lobby sets
    void replicateEditsForLobby(void)
    {
        u64 currGameplayLayout;
        Process::Read64(AddressList::getAddress("GameplayEmotes"), currGameplayLayout);
        Process::Write64(AddressList::getAddress("LobbyEmotes"), currGameplayLayout);
    }

    // Restore default emote set to avoid button mix-ups upon entering new area (as bottom-screen emote menu graphics can't be overridden on load-in)
    void restoreDefaultEmotes(void)
    {
        if (GeneralHelpers::isLoadingScreen(true))
        {
            if (Level::levelIDFromName("Hytopia Castle") == Level::getTargetLevel())
            {
                initEmoteValueLayout(AddressList::getAddress("GameplayEmotes"), 0x05080A0B, 0x00000706, true, {11, 6, 5, 3, 4, 9, 0});
                replicateEditsForLobby();
            }
            else
                initEmoteValueLayout(AddressList::getAddress("GameplayEmotes"), 0x03020100, 0x07060504, false, {0, 1, 2, 7, 8, 3, 4, 9});

            toggleDefaultEmotes(false);
            return;
        }
    }
}