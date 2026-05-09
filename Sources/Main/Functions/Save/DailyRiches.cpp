#include "Cheats.hpp"
#include "Helpers.hpp"

namespace CTRPluginFramework
{
    // Helper function to overwrite prize item with selected material ID
    void selectPrizeItem(void)
    {
        int material = -1;
        int world = Material::selectMaterialWorld();

        if (world >= 0)
            material = Material::selectMaterialIndiv(world);

        if (material >= 0)
        {
            u32 instructionAddr = AddressList::getAddress("DefaultDR_PrizeSet");

            Process::Write32(AddressList::getAddress("DailyRichesPrizeSetCheck"), 0xE1A00000); // NOP out has-prize-been-set check, forcing default item to be used
            Process::Write8(instructionAddr, material);     // overwrite default item MOV instruction immediate
            Process::Write8(instructionAddr + 0x3, 0xE3);   // change instruction opcode from conditional -> unconditional

            MessageBox("Success", Utils::Format("Daily Riches prize item set to %s. Be sure to reload the area for changes to take effect.", Material::getMaterialName(world, material).c_str()))();
        }
    }

    // Restores normal set-prize behavior
    void restoreRandomLogic(void)
    {
        Process::Write32(AddressList::getAddress("DailyRichesPrizeSetCheck"), 0xE3500000); // restore to CMP
        Process::Write32(AddressList::getAddress("DefaultDR_PrizeSet"), 0x03A00014);       // restore to MOVEQ, with imm set to Lady's Glasses
    }

    // Allows current Daily Riches prize item to be overwritten
    void Save::selPrizeSetAction(MenuEntry *entry)
    {
        StringVector options =
        {
            "Force prize material",
            "Restore normal RNG behavior"
        };

        Keyboard action("Daily Riches Prize Item", "Select one of the options below to either set a new prize item or revert any previous edits made.");
        action.Populate(options);

        int selection = action.Open();
        switch (selection)
        {
            case 0:
                selectPrizeItem();
                return;
            case 1:
                restoreRandomLogic();
                return;
            default:
                return;
        }
    }
}
