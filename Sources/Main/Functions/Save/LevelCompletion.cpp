#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // Unlocks all DoT stages
    void Save::unlockDoT(MenuEntry* entry)
    {
        u32 DoTProgress;

        Process::Read32(AddressList::getAddress("NormalNCCompletion") + 0x4, DoTProgress);
        DoTProgress |= 0x1FE; // sets bits 33-40 within level completion bitstring A

        if (Process::Write32(AddressList::getAddress("NormalNCCompletion") + 0x4, DoTProgress))
            MessageBox("Success", "All DoT zones are now unlocked.")();
        else
            MessageBox("Error", "DoT zones could not be unlocked. Please try again.")();
    }

    // Toggles level completion per world
    void Save::selLevelCompletion(MenuEntry* entry)
    {
        std::string msg, modeStr;
        int world = Level::selDrablandsWorld(false);

        if (world >= 0)
        {
            int mode = selPlayMode();
            if (mode >= 0)
            {
                std::string outro = "\n\nPress " + std::string(FONT_B) + " to save and exit this menu.";

                if (mode < 2)
                    modeStr = mode ? "Multiplayer Completion." : "Single-player Completion.";
                else
                    modeStr = "Multiplayer and Single-player\nCompletion.";

                msg = "Currently editing Level Completion statuses\nfor " << Level::worldIDToStr(world) << ", " << modeStr << outro;
                LevelStatusEditor(msg, Level::getLevelNamesFromWorld(world), world, mode)();
            }
        }
    }

    // Helper function to determine which set of level progress data to overwrite
    int selPlayMode(void)
    {
        Keyboard mode("Level Completion Editor", "Choose a mode.");
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