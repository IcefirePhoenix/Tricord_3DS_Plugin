#include "Main/Item.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    MenuEntry *forceShadowSwordOnly;
    u32 defaultJumpTblDestinations[8];
    u8 randomItem = 0x9;

    /* ------------------ */

    // Force-sets current items for Shadow Links
    void Item::shadowItemOpt(MenuEntry *entry)
    {
        int shadowLinkChoice = GeneralHelpers::chooseShadowLink();

        if (shadowLinkChoice >= 0)
        {
            u32 indivShadowOffset = shadowLinkChoice * 0x4;
            u32 shadowItemPointerOffset = 0x20;
            u32 dynamicShadowItemAddress;

            initShadowItemList();

            // get dynamic address via pointer...
            Process::Read32(AddressList::getAddress("ShadowLinkItemPointer"), dynamicShadowItemAddress);
            u32 shadowDataAddress = dynamicShadowItemAddress + shadowItemPointerOffset;

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
                    Process::Write8(shadowDataAddress + indivShadowOffset, choice - 1);
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
        u32 shadowLinkItemOffset = 0x4;
        u32 shadowItemPointerOffset = 0x20;
        u32 dynamicShadowItemAddress;

        // get dynamic address via pointer...
        Process::Read32(AddressList::getAddress("ShadowLinkItemPointer"), dynamicShadowItemAddress);
        u32 shadowDataAddress = dynamicShadowItemAddress + shadowItemPointerOffset;

        // Shadow Links aren't loaded in-game outside of Baneful Zone...
        bool isInBaneful = (Level::getCurrLevel() == Level::levelIDFromName("Baneful Zone")) && (Level::getCurrStage() == 4);
        std::string msg = isInBaneful ? "Select an item." : "Error\n\nThe current level is not Baneful Zone - 4. Please\nenter Baneful Zone and try again.";

        if (isInBaneful)
        {
            // disable per-use item randomization and assign new items...
            for (int iterateThroughPlayers = 0; iterateThroughPlayers < 3; iterateThroughPlayers++)
            {
                toggleRandomizedShadowItems(false, iterateThroughPlayers);
                Process::Write8(shadowDataAddress + iterateThroughPlayers * shadowLinkItemOffset, Utils::Random(0x1, 0x8));
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

    // Redirects all item usage calls to sword usage function
    void Item::toggleShadowForceSwordUse(MenuEntry* entry)
    {
        u32 jumpTblOffset;

        // reset to default destinations...
        if (!entry->IsActivated())
        {
            for (int index = 0; index < 8; index++)
            {
                jumpTblOffset = (index + 1) * sizeof(u32);
                Process::Write32(AddressList::getAddress("ShadowLinkItemJumptbl") + jumpTblOffset, defaultJumpTblDestinations[index]);
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