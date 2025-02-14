#include "Helpers.hpp"
#include "Main/Costume.hpp"

namespace CTRPluginFramework
{
    MenuEntry* costumeRandomizerAuto;

    bool randomizers[2][3] = { { false, false, false }, { false, false, false } };
    bool canRandomize = true; // Used to ensure only one write during loading screens

    /* ------------------ */

    // Driver code for costume randomizers
    void Costume::costumeRandomizer(MenuEntry* entry)
    {
        openRandomizerToggles(selCostumeType());
    }

    // Helper method to select the costume type
    int selCostumeType(void)
    {
        StringVector costumeTypeOptions =
        {
            "Effective",
            "Cosmetic",
            "Disable entry"
        };

        Keyboard costumeType("Select which type of costume to randomize,\nor disable this entry.");
        costumeType.Populate(costumeTypeOptions);

        int selection = costumeType.Open();
        if (selection == 2)
        {
            costumeRandomizerAuto->Disable();
            for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
            {
                // Reset cosmetic costume
                if (randomizers[1][iterateThruPlayers])
                    Costume::cosmeticIDs[iterateThruPlayers] = cosmeticNotInUse;
            }
        }
        return selection;
    }

    // Menu interface for toggling costume randomizers per player
    void openRandomizerToggles(int selection)
    {
        StringVector bottomScreenOptions;
        std::string title;
        bool isMenuOpen = true;

        Keyboard kbd("Menu");
        kbd.CanAbort(false);

        if (selection == 0 || selection == 1)
        {
            while (isMenuOpen)
            {
                title = "Use the toggles to enable the\n";
                title += (selection == 0) ? "Effective Costume Randomizers:\n\n" : "Cosmetic Costume Randomizers:\n\n";

                bottomScreenOptions.clear();
                bottomScreenOptions.push_back(std::string("Player 1 ") << (randomizers[selection][0] ? ENABLED_SLIDER : DISABLED_SLIDER));
                bottomScreenOptions.push_back(std::string("Player 2 ") << (randomizers[selection][1] ? ENABLED_SLIDER : DISABLED_SLIDER));
                bottomScreenOptions.push_back(std::string("Player 3 ") << (randomizers[selection][2] ? ENABLED_SLIDER : DISABLED_SLIDER));
                bottomScreenOptions.push_back("Save changes");

                kbd.GetMessage() = title;
                kbd.Populate(bottomScreenOptions);
                int choice = kbd.Open();

                if (choice >= 0 && choice <= 2)
                    randomizers[selection][choice] = !randomizers[selection][choice];
                else
                {
                    if (choice == 3)
                    {
                        if (randomizers[0][0] || randomizers[0][1] || randomizers[0][2] || randomizers[1][0] || randomizers[1][1] || randomizers[1][2])
                            costumeRandomizerAuto->Enable();
                        else
                            costumeRandomizerAuto->Disable();
                    }
                    isMenuOpen = false;
                }
            }
        }
    }

    // Sets the current cosmetic/effective costume ID
    void Costume::writeRandomCostume(MenuEntry* entry)
    {
        if (GeneralHelpers::isLoadingScreen(true) && canRandomize)
        {
            canRandomize = false;

            // Effective costume
            for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
            {
                if (randomizers[0][iterateThruPlayers])
                    Costume::setPlayerCostume(iterateThruPlayers, Utils::Random(0, GameData::maxCostumeCount)); // primary + alternate costume IDs used...
            }

            // Cosmetic costume
            for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
            {
                if (randomizers[1][iterateThruPlayers])
                    cosmeticIDs[iterateThruPlayers] = Utils::Random(0, GameData::maxCostumeCount); // let Cosmetic Costume function handle mem writes...
            }
        }

        if (Level::getElapsedTime() == 100)
            canRandomize = true;
    }
}