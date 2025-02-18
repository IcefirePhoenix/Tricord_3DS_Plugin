#include "Main/Item.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    MenuEntry *forceShadowSwordOnly;
    u32 defaultJumpTblDestinations[8];

    u32 shadowLinkItemOffset = 0x4;
    u32 shadowItemPointerOffset = 0x20;
    u32 dynamicShadowItemAddress;
    u8 randomItem = 0x9;

    /* ------------------ */

    // Force-sets current items for Shadow Links
    void Item::shadowItemOpt(MenuEntry *entry)
    {
        int shadowLinkChoice = GeneralHelpers::chooseShadowLink();

        if (shadowLinkChoice >= 0)
        {
            u32 shadowDataAddress = getShadowItemAddress();
            u32 indivShadowOffset = shadowLinkChoice * shadowLinkItemOffset;

            initShadowItemList();

            // Shadow Links aren't loaded in-game outside of Baneful Zone...
            bool isInBaneful = (Level::getCurrLevel() == Level::levelIDFromName("Baneful Zone")) && (Level::getCurrStage() == 4);
            std::string msg = isInBaneful ? "Select an item." : "Error\n\nThe current level is not Baneful Zone - 4. Please\nenter Baneful Zone and try again.";

            if (isInBaneful)
            {
                msg.append("\n\nCurrent Shadow Link items:\n");
                msg.append(readCurrItems(shadowDataAddress, true));
                msg.append("\n\nBe sure to re-load the stage for changes to\ntake effect.");

                Keyboard item(msg);
                item.Populate(Item::shadowItemList);

                int choice = item.Open();
                if (choice == 0)
                {
                    toggleRandomizedShadowItems(true, shadowLinkChoice);
                    Process::Write8(shadowDataAddress + indivShadowOffset, randomItem);
                }
                else if (choice > 0)
                {
                    toggleRandomizedShadowItems(false, shadowLinkChoice);
                    Process::Write8(shadowDataAddress + indivShadowOffset, choice);
                }
            }
            else
            {
                // if not in Baneful Zone -> no item options displayed...
                const StringVector temp = {"Exit"};
                Keyboard item(msg);

                item.Populate(temp);
                item.Open();
            }
        }
    }

    // Disables specialty item options and resets Shadow Link items to a new randomized set
    void Item::resetShadowItems(MenuEntry *entry)
    {
        u32 shadowDataAddress = getShadowItemAddress();

        // Shadow Links aren't loaded in-game outside of Baneful Zone...
        bool isInBaneful = (Level::getCurrLevel() == Level::levelIDFromName("Baneful Zone")) && (Level::getCurrStage() == 4);
        std::string msg = isInBaneful ? "Select an item." : "Error\n\nThe current level is not Baneful Zone - 4. Please\nenter Baneful Zone and try again.";

        if (isInBaneful)
        {
            int index, itemPoolCount = 8;
            std::vector<u8> availableItems = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};

            // disable per-use item randomization and assign new items...
            for (int iterateThroughPlayers = 0; iterateThroughPlayers < 3; iterateThroughPlayers++)
            {
                index = Utils::Random(0, itemPoolCount - 1);
                Process::Write8(shadowDataAddress + iterateThroughPlayers * shadowLinkItemOffset, availableItems[index]);
                toggleRandomizedShadowItems(false, iterateThroughPlayers);

                availableItems.erase(availableItems.begin() + index);
                itemPoolCount--;
            }

            // disable forced-sword attacks...
            forceShadowSwordOnly->Disable();
        }
    }

    // Bypasses the item init sequence by forcing the Shadow Link ID checks to fail
    void toggleRandomizedShadowItems(bool useRandom, int shadowLink)
    {
        u8 shadowLinkOffset = 0x8;
        u32 invalidCMP = 0xE3500000; // immediate value set to invalid ID... by default, they range from 1-3
        u32 patchedCMP = useRandom ? invalidCMP : (invalidCMP & 0xFFFFFFF0) | (shadowLink + 1);

        Process::Patch(AddressList::getAddress("ShadowLinkItemInitCheck") + shadowLinkOffset * shadowLink, patchedCMP);
    }

    u32 getShadowItemAddress(void)
    {
        // get dynamic address via pointer...
        Process::Read32(AddressList::getAddress("ShadowLinkItemPointer"), dynamicShadowItemAddress);
        return dynamicShadowItemAddress + shadowItemPointerOffset;
    }

    // Redirects all item usage calls to sword usage function
    void Item::toggleShadowForceSwordUse(MenuEntry* entry)
    {
        u8 currItem;
        u32 jumpTblOffset;
        u32 shadowDataAddress = getShadowItemAddress();

        // reset to default destinations...
        if (!entry->IsActivated())
        {
            for (int index = 0; index < 8; index++)
            {
                jumpTblOffset = (index + 1) * sizeof(u32);
                Process::Write32(AddressList::getAddress("ShadowLinkItemJumptbl") + jumpTblOffset, defaultJumpTblDestinations[index]);
            }

            for (int iterateThroughPlayers = 0; iterateThroughPlayers < 3; iterateThroughPlayers++)
            {
                Process::Read8(shadowDataAddress + iterateThroughPlayers * shadowLinkItemOffset, currItem);

                // restore randomization if necessary...
                if (currItem == 0x9)
                    toggleRandomizedShadowItems(true, iterateThroughPlayers);
            }
        }

        // patch all item function jumptable destinations -> point to default case (sword usage)
        if (entry->WasJustActivated())
        {
            // store then patch default addresses...
            for (int index = 0; index < 8; index++)
            {
                jumpTblOffset = (index + 1) * sizeof(u32);
                Process::Read32(AddressList::getAddress("ShadowLinkItemJumptbl") + jumpTblOffset, defaultJumpTblDestinations[index]);
                Process::Write32(AddressList::getAddress("ShadowLinkItemJumptbl") + jumpTblOffset, AddressList::getAddress("ShadowLinkSwordUsageFunc"));
            }

            // item disabling overrides randomization...
            for (int iterateThroughPlayers = 0; iterateThroughPlayers < 3; iterateThroughPlayers++)
                toggleRandomizedShadowItems(false, iterateThroughPlayers);
        }
    }

    // Helper function that creates a version of the normal item list modified for Shadow Links
    void initShadowItemList(void)
    {
        Item::shadowItemList = GameData::itemList;

        Item::shadowItemList.pop_back();
        Item::shadowItemList.insert(Item::shadowItemList.begin(), "Randomly switch items :)");
    }
}