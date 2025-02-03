#include "Main/Item.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
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

                // adjust for first list item = randomized items = last ID (0x9)
                if (choice == 0)
                    Process::Write8(shadowDataAddress + indivShadowOffset, 0x9);
                else if (choice > 0)
                    Process::Write8(shadowDataAddress + indivShadowOffset, choice);
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

    // Helper function that creates a version of the normal item list modified for Shadow Links
    void initShadowItemList(void)
    {
        Item::shadowItemList = GameData::itemList;

        Item::shadowItemList.pop_back();
        Item::shadowItemList.insert(Item::shadowItemList.begin(), "Randomly switch items :)");
    }
}