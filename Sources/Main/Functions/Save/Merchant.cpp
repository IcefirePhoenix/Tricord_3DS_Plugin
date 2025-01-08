#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    MenuEntry *merchantA;
    MenuEntry *merchantB;
    MenuEntry *merchantC;
    MenuEntry *merchantD;
    MenuEntry *merchantE;

    /* ------------------ */

    // Driver code for force-setting merchant data -> also responsible for displaying edits in menu
    void Save::selMerchantSlot(MenuEntry *entry)
    {
        /**
         * Multiple MenuEntries call this function... use pre-defined Arg value to differentiate between them.
         * This value is also used to determine which merchant slot to write to... see below.
         */
        int slotID = reinterpret_cast<int>(entry->GetArg());
        std::string material = openMerchantMatMenu(slotID);
        std::string newName;

        // Append chosen material to MenuEntry's title string...
        switch (slotID)
        {
            case 0:
                newName = "Set 1st material slot: ";
                break;
            case 2:
                newName = "Set 2nd material slot: ";
                break;
            case 4:
                newName = "Set 3rd material slot: ";
                break;
            case 6:
                newName = "Set 4th material slot: ";
                break;
            case 8:
                newName = "Set 5th material slot: ";
                break;
            default:
                break;
        }

        if (!material.empty())
            entry->SetName(newName << material);
    }


    // Writes custom material selections to memory -> returns choice as string to display in menu
    std::string openMerchantMatMenu(u8 slotNumber)
    {
        int material = -1;
        int world = Material::selectMaterialWorld();

        if (world >= 0)
            material = Material::selectMaterialIndiv(world);

        if (material >= 0)
        {
            /**
             * Since each slot's data has a 8-bit spacer between them, the entry's Arg value (even int 0-8)
             * is used as an offset to navigate between slots without having to define 5 separate addresses.
             */
            Process::Write8(AddressList::EditMerchantStock.addr + slotNumber, static_cast<u8>(material));
            return Material::getMaterialName(world, material);
        }
        return "";
    }

    // Force-triggers refresh of merchant selection
    void Save::resetMerchant(MenuEntry *entry)
    {
        Process::Write8(AddressList::ResetMerchantStock.addr, 0x0);
        MessageBox(Color::Gainsboro << "Street Merchant Stall has been re-stocked.")();
    }
}