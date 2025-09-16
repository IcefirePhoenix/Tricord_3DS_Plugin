#include "Helpers.hpp"
#include "Main/Costume.hpp"

namespace CTRPluginFramework
{
    MenuEntry* costumeRandomizerAuto;

    bool randomizers[2][3] = { { false, false, false }, { false, false, false } };
    bool canRandomize = true; // Used to ensure only one write during loading screens
    bool canRandomizeOnDefeat = true; // Used to preserve generated costumes upon defeat if desired

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
            "Effective costume",
            "Cosmetic costume",
            "Disable entry"
        };

        Keyboard costumeType("Costume Randomizers", "Select which type of costume to randomize or disable this entry.");
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
        switch (selection)
        {
            case 2:
                costumeRandomizerAuto->Disable();
            case -1:
                return;
        }

        int status = PlayerMask::PLAYER_NONE;

        // read bool arrays...
        for (int i = 0; i < 3; i++)
        {
            if (randomizers[selection][i])
                status = static_cast<PlayerMask>(status | (1 << i));
        }

        status = PlayerSelector(true, status, "Costume Randomizer Toggle Menu", "randomizer")();

        // set new values for bool arrays...
        for (int i = 0; i < 3; i++)
        {
            randomizers[selection][i] = (status & (1 << i)) != 0;
        }

        // auto-enable or disable depending on values...
        if (randomizers[0][0] || randomizers[0][1] || randomizers[0][2] || randomizers[1][0] || randomizers[1][1] || randomizers[1][2])
            costumeRandomizerAuto->Enable();
        else
            costumeRandomizerAuto->Disable();
    }

    // Sets the current cosmetic/effective costume ID
    void Costume::writeRandomCostume(MenuEntry* entry)
    {
        if (GeneralHelpers::isLoadingScreen(true) && canRandomize && !(!canRandomizeOnDefeat && GeneralHelpers::getHP() == 0))
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

    // Toggles whether new random costumes are generated upon defeat (losing a fairy) or not
    void Costume::toggleRandomizeOnDefeat(MenuEntry* entry)
    {
        if (entry->Name() == "Preserve costumes upon defeat")
        {
            canRandomizeOnDefeat = false;
            entry->SetName("Randomize costumes upon defeat");
        }
        else
        {
            canRandomizeOnDefeat = true;
            entry->SetName("Preserve costumes upon defeat");
        }
    }
}