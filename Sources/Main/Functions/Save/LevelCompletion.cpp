#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // Toggles level completion per world
    void Save::selLevelCompletion(MenuEntry *entry)
    {
        std::string msg, modeStr;
        int world = Level::selWorld(false, false);

        if (world >= 0)
        {
            int mode = selPlayMode();
            if (mode >= 0)
            {
                std::string outro = "\n\nPress " + std::string(FONT_B) + " to save and exit this menu.";

                if (mode < 2)
                    modeStr = mode ? "Multiplayer Completion." : "Single-player Completion.";
                else
                    modeStr = "Multiplayer and Single-player\nCompletion";

                msg = "Currently editing Level Completion statuses\nfor " << Level::worldIDToStr(world) << ", " << modeStr << outro;
                LevelStatusEditor(msg, Level::getWorldNamesfromID(world, false), world, mode)();
            }
        }
    }

    // Helper function to determine which set of level progress data to overwrite
    int selPlayMode(void)
    {
        Keyboard mode("Choose a mode:");
        StringVector modeList =
        {
            "Single-player",
            "Multiplayer",
            "Both"
        };

        mode.Populate(modeList);
        return mode.Open();
    }
}