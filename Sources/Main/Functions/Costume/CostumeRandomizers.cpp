#include "Helpers.hpp"
#include "Main/Costume.hpp"
#include "Helpers/GeneralHelpers.hpp"

namespace CTRPluginFramework
{

    MenuEntry* costumeRandomizerAuto;

    bool randomizers[2][3] = { { false, false, false }, { false, false, false } };
    bool canRandomize = true; // Used to ensure only one write during loading screens

    void Costume::costumeRandomizer(MenuEntry* entry)
    {
        Keyboard costumeType("Select which type of costume to randomize,\nor disable this entry.");
        StringVector costumeTypeOptions =
        {
            "Effective",
            "Cosmetic",
            "Disable entry"
        };
        costumeType.Populate(costumeTypeOptions);
        int i = costumeType.Open();
        if (i < 0)
            return;
        if (i == 2)
        {
            costumeRandomizerAuto->Disable();
            for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
            {
                if (randomizers[1][iterateThruPlayers])
                {
                    // Reset cosmetic costume
                    cosmeticIDs[iterateThruPlayers] = 0xFF;
                }
            }
            return;
        }

        std::string enSlid = Color::LimeGreen << "\u2282\u25CF";
        std::string disSlid = Color::Red << "\u25CF\u2283";
        std::string title;

        StringVector bottomScreenOptions;

        Keyboard kbd("Menu");
        kbd.CanAbort(false);

        bool loop = true;
        while (loop)
        {
            if (i == 0)
                title = "Use the toggles to enable the\nEffective Costume Randomizers:\n\n";
            else
                title = "Use the toggles to enable the\nCosmetic Costume Randomizers:\n\n";

            bottomScreenOptions.clear();
            bottomScreenOptions.push_back(std::string("Player 1 ") << (randomizers[i][0] ? enSlid : disSlid));
            bottomScreenOptions.push_back(std::string("Player 2 ") << (randomizers[i][1] ? enSlid : disSlid));
            bottomScreenOptions.push_back(std::string("Player 3 ") << (randomizers[i][2] ? enSlid : disSlid));
            bottomScreenOptions.push_back("Save changes");

            kbd.GetMessage() = title;
            kbd.Populate(bottomScreenOptions);

            switch (kbd.Open())
            {
            case 0:
                randomizers[i][0] = !randomizers[i][0];
                break;
            case 1:
                randomizers[i][1] = !randomizers[i][1];
                break;
            case 2:
                randomizers[i][2] = !randomizers[i][2];
                break;
            case 3:
                if (randomizers[0][0] || randomizers[0][1] || randomizers[0][2] || randomizers[1][0] || randomizers[1][1] || randomizers[1][2])
                    costumeRandomizerAuto->Enable();
                else
                    costumeRandomizerAuto->Disable();
                loop = false;
                break;
            default:
                loop = false;
                break;
            }
        }
    }

    // Sets the current cosmetic/effective costume ID
    void Costume::writeRandomCostume(MenuEntry* entry)
    {
        if (GeneralHelpers::isLoadingScreen(true) && canRandomize)
        {
            canRandomize = false;
            // Effective costume - Write directly to address
            for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
            {
                if (randomizers[0][iterateThruPlayers])
                {
                    // Generate new random costume
                    // Write to both primary and alternate costume ID addresses to ensure the model updates at the
                    // loading zone if cosmetic costumes are enabled
                    int randomCostumeID = rand() % 38;
                    Process::Write8(AddressList::getAddress("CurrCostume") + iterateThruPlayers*PLAYER_OFFSET, randomCostumeID);
                    Process::Write8(AddressList::getAddress("CurrCostumeAlt") + iterateThruPlayers*PLAYER_OFFSET, randomCostumeID);
                }
            }

            // Cosmetic costume - Let it be managed by the Enable Cosmetic Costumes entry
            for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
            {
                if (randomizers[1][iterateThruPlayers])
                {
                    // Generate new random costume
                    cosmeticIDs[iterateThruPlayers] = rand() % 38;
                }
            }
        }
        if (Level::getElapsedTime() == 100)
        {
            canRandomize = true;
        }
    }

}