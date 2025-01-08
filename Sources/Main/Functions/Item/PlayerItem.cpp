#include "Main/Item.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // Force-sets current player item
    void Item::itemOpt(MenuEntry *entry)
    {
        int player = GeneralHelpers::chooseLink();

        if (player >= 0)
        {
            u32 playerOffset = player * PLAYER_OFFSET;

            std::string intro = "Select an item.\n\nCurrent items:\n";
            intro.append(readCurrItems(AddressList::CurrentItem.addr, false));

            Keyboard item(intro);
            item.Populate(GameData::itemList);

            int choice = item.Open();
            if (choice >= 0)
            {
                choice++; // since the list isn't actually 0-indexed in memory...

                Process::Write8(AddressList::CurrentItem.addr + PLAYER_OFFSET, choice);
                currItems[player] = choice;
            }
        }
    }

    // Removes or freezes current player items
    void Item::manageItems(MenuEntry *entry)
    {
        u8 item;
        std::string successMsg = "All items have been removed.";

        if (entry->WasJustActivated())
            saveCurrItems();

        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
        {
            u32 playerOffset = iterateThruPlayers * PLAYER_OFFSET;

            // multiple MenuEntries call this function... use MenuEntry name to differentiate between them...
            item = (entry->Name() == "Remove current items") ? NOITEM : currItems[iterateThruPlayers];
            Process::Write8(AddressList::CurrentItem.addr + playerOffset, item);
        }

        if (entry->Name() == "Remove current items")
            MessageBox(Color::Gainsboro << "Success!", successMsg)();
    }

    // Helper function that backs up current player items
    void saveCurrItems(void)
    {
        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
        {
            u32 playerOffset = iterateThruPlayers * PLAYER_OFFSET;
            Process::Read8(AddressList::CurrentItem.addr + playerOffset, Item::currItems[iterateThruPlayers]);
        }
    }
}