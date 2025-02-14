#include "Helpers.hpp"
#include "Main/Player.hpp"

namespace CTRPluginFramework
{
    // TODO: create new icons to replace these placeholders...
    FloatingButton loadBtn(IntRect(100, 210, 15, 15), Icon::DrawCentreOfGravity);
    FloatingButton saveBtn(IntRect(125, 210, 15, 15), Icon::DrawUnsplash);

    bool isPositionSaved[3] = {false, false, false};
    float positions[3][3];

    /* ------------------ */

    // Driver code for managing player coordinates
    void Player::posEditor(MenuEntry *entry)
    {
        // init behavior...
        showPosEditorIntroMsg(entry->WasJustActivated());
        resetPositionEditorSaves(entry->WasJustActivated());

        // custom touchscreen button display behavior...
        if (GeneralHelpers::isSinglePlayer() && !GeneralHelpers::isLoadingScreen(false) && Level::getCurrLevel() != Level::levelIDFromName("Hytopia Castle"))
        {
            loadBtn.Draw();
            saveBtn.Draw();

            if (!GeneralHelpers::isPauseScreen())
            {
                loadBtn.Update(Touch::IsDown(), IntVector(Touch::GetPosition()));
                saveBtn.Update(Touch::IsDown(), IntVector(Touch::GetPosition()));
            }
        }

        resetPositionEditorSaves(GeneralHelpers::isLoadingScreen(true));

        int currLink = GeneralHelpers::getCurrLink();
        std::string color = GeneralHelpers::getLinkColorAsStr(currLink);

        // on-press button behavior...
        if (loadBtn())
        {
            if (isPositionSaved[currLink])
            {
                loadPlayerPos(currLink);
                OSD::Notify("[POSITION LOAD/SAVE] Loaded " + color + "'s last saved position.");
            }
            else
                OSD::Notify("[POSITION LOAD/SAVE] No saved position found for " + color + ".");
        }

        if (saveBtn())
        {
            savePlayerPos(currLink);
            OSD::Notify("[POSITION LOAD/SAVE] Saved " + color + "'s current position.");
        }
    }

    // Helper function to reload saved player coordinates
    void loadPlayerPos(int playerID)
    {
        u32 offset = playerID * PLAYER_OFFSET;

        Process::WriteFloat(AddressList::getAddress("PositionX") + offset, positions[playerID][0]);
        Process::WriteFloat(AddressList::getAddress("PositionY") + offset, positions[playerID][1]);
        Process::WriteFloat(AddressList::getAddress("PositionZ") + offset, positions[playerID][2]);
    }

    // Helper function to back up current player coordinates
    void savePlayerPos(int playerID)
    {
        u32 offset = playerID * PLAYER_OFFSET;

        Process::ReadFloat(AddressList::getAddress("PositionX") + offset, positions[playerID][0]);
        Process::ReadFloat(AddressList::getAddress("PositionY") + offset, positions[playerID][1]);
        Process::ReadFloat(AddressList::getAddress("PositionZ") + offset, positions[playerID][2]);

        isPositionSaved[playerID] = true;
    }

    // Reset all "saved" statuses
    void resetPositionEditorSaves(bool reset)
    {
        if (reset)
            std::fill(isPositionSaved, isPositionSaved + 3, false);
    }

    // Displays intro message upon activation
    void showPosEditorIntroMsg(bool showMsg)
    {
        if (showMsg)
        {
            OSD::Notify("[POSITION LOAD/SAVE] Use the touchscreen buttons to save/load");
            OSD::Notify("the current Link's position.");
        }
    }
}