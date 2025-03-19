#include "Helpers.hpp"
#include "Main/Costume.hpp"

namespace CTRPluginFramework
{
    MenuEntry *swordEditAuto;
    u8 Costume::swordType[3] = {cosmeticNotInUse, cosmeticNotInUse, cosmeticNotInUse};
    bool Costume::customSwordsActive = false;

    /* ------------------ */

    // Helper function to select custom sword model
    void openSwordSelMenu(void)
    {
        int swordChoice = -1;
        Keyboard menu("Sword Model Selection", "Select a player to begin editing the current sword model.");
        StringVector bottomScreenOptions;

        bool isMenuOpen = true;
        menu.CanAbort(false);

        while (isMenuOpen)
        {
            bottomScreenOptions.clear(); // clear bottom screen per frame...
            bottomScreenOptions.push_back(std::string("Player 1: ") << GameData::getSwordNameFromID(Costume::swordType[0]));
            bottomScreenOptions.push_back(std::string("Player 2: ") << GameData::getSwordNameFromID(Costume::swordType[1]));
            bottomScreenOptions.push_back(std::string("Player 3: ") << GameData::getSwordNameFromID(Costume::swordType[2]));
            bottomScreenOptions.push_back("Save and exit");

            // update top screen with real-time current statuses...
            menu.Populate(bottomScreenOptions);

            int playerChoice = menu.Open();
            switch (playerChoice)
            {
                case 0:
                case 1:
                case 2:
                    swordChoice = GeneralHelpers::chooseSword();
                    if (swordChoice >= 0)
                        Costume::swordType[playerChoice] = swordChoice;
                    break;

                // "Save and Exit"
                case 3:
                    isMenuOpen = false;
                    break;
            }
        }
    }

    // Helper function to write sword model edits to memory
    void Costume::writeSwordChanges(MenuEntry *entry)
    {
        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
        {
            u32 finalAddress = AddressList::getAddress("SwordType") + (iterateThruPlayers * PLAYER_OFFSET);
            u32 statusBitAddress = AddressList::getAddress("StatusBitB") + (iterateThruPlayers * PLAYER_OFFSET);
            u8 sword = Costume::swordType[iterateThruPlayers];

            u8 statusBitsB;
            Process::Read8(statusBitAddress, statusBitsB);

            if (sword == 0x0B)
            {
                // No sword - Hide and disable sword usage
                statusBitsB |= 0x01;
                Process::Write8(statusBitAddress, statusBitsB);
            }
            else if (sword != cosmeticNotInUse)
            {
                // Ensure sword is enabled
                // Note: This cannot bypass "no sword" challenges, but it can let you use your sword inside Hytopia Shops
                statusBitsB &= 0xFE;
                Process::Write8(statusBitAddress, statusBitsB);
                Process::Write8(finalAddress, sword);
            }
        }
    }

    // Driver code for setting sword model edits
    void Costume::setSwordChanges(MenuEntry *entry)
    {
        if (entry->Name() == "Set custom sword model(s)")
        {
            openSwordSelMenu();
            entry->SetName("Disable custom sword model edits");
            swordEditAuto->Enable();
            Costume::customSwordsActive = true;
        }
        else
        {
            entry->SetName("Set custom sword model(s)");
            swordEditAuto->Disable();
            Costume::customSwordsActive = false;
        }
    }
}