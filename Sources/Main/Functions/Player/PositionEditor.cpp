#include "Helpers.hpp"
#include "Main/Player.hpp"

namespace CTRPluginFramework
{
    Button loadBtn(Button::Icon | Button::Toggle, IntRect(32, 107, 15, 15), Icon::DrawLoadPosition);
    Button saveBtn(Button::Icon | Button::Toggle, IntRect(32, 85, 15, 15), Icon::DrawSavePosition);

    bool isPositionSaved[3] = {false, false, false};
    float positions[3][3];

    Clock writeTimer;

    /* ------------------ */

    // Driver code for managing player coordinates
    void Player::posEditor(MenuEntry *entry)
    {
        resetPositionEditorSaves(entry->WasJustActivated());

        // custom touchscreen button display behavior...
        if (GeneralHelpers::isSinglePlayer() && !GeneralHelpers::isLoadingScreen(false) && Level::getCurrLevel() != Level::levelIDFromName("Hytopia Castle"))
        {
            OSDImpl::AddToPriorityDrawQueue(loadBtn);
            OSDImpl::AddToPriorityDrawQueue(saveBtn);

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
                writeTimer.Restart();
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

        while (!writeTimer.HasTimePassed(Milliseconds(20)))
        {
            Process::WriteFloat(AddressList::getAddress("PositionX") + offset, positions[playerID][0]);
            Process::WriteFloat(AddressList::getAddress("PositionY") + offset, positions[playerID][1]);
            Process::WriteFloat(AddressList::getAddress("PositionZ") + offset, positions[playerID][2]);
        }
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
}