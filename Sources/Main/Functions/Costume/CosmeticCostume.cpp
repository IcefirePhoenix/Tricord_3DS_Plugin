#include "Helpers.hpp"
#include "Main/Costume.hpp"

namespace CTRPluginFramework
{
    MenuEntry* cosmeticCostumeAuto;

    u8 currEffectCostumeID;   // true costume currently worn -> this costume's effects will continue to be used...
    u8 currCosmeticCostumeID; // costume MODEL desired...
    u8 Costume::cosmeticIDs[3] = { cosmeticNotInUse, cosmeticNotInUse, cosmeticNotInUse };

    /* ------------------ */

    // Force a specific costume MODEL using an alternate, unused (?) costume ID -> however, true current costume's effects should persist as usual
    void Costume::enableCosmeticCostume(MenuEntry *entry)
    {
        if (entry->Name() == "Enable Cosmetic Costumes")
        {
            u32 custFuncStart = AddressList::getAddress("TextToRodata") + 0x64;
            u32 blOffset = (custFuncStart - 0x8 - AddressList::getAddress("CostumeIDFunBLModel")) / 4; // division by 4 as all instructions are 32-bit...

            // patch LDRB instructions to force graphic/visual costume effects to reference the alternate costume address...
            Process::Patch(AddressList::getAddress("CostumeIDOffsetAuraA"), 0xE5D00D61);
            Process::Patch(AddressList::getAddress("CostumeIDOffsetAuraB"), 0xE5D01D61);
            Process::Patch(AddressList::getAddress("CostumeIDOffsetAuraC"), 0xE5D01D61);
            Process::Patch(AddressList::getAddress("CostumeIDOffsetAuraD"), 0xE5D01D61);
            Process::Patch(AddressList::getAddress("CostumeIDOffsetCheetah"), 0xE5D01D61); // TODO: add more to clarify address name -> env? ptcl?
            Process::Patch(AddressList::getAddress("CostumeIDOffsetDune"), 0xE5D00D61);
            Process::Patch(AddressList::getAddress("CostumeIDOffsetCheer"), 0xE5D00D61);
            Process::Patch(AddressList::getAddress("CostumeIDOffsetSwordPtcl"), 0xE5D00D61);

            // create custom function in text->rodata padding...
            Process::Patch(custFuncStart, 0xE5900008);
            Process::Patch(custFuncStart + 0x4, 0xE5D00D61);
            Process::Patch(custFuncStart + 0x8, 0xE12FFF1E);

            // redirect model loader to custom function via BL instruction...
            Process::Patch(AddressList::getAddress("CostumeIDFunBLModel"), 0xEB000000 + blOffset);

            cosmeticCostumeAuto->Enable();
            entry->SetName("Disable Cosmetic Costumes");
        }
        else
        {
            // revert LDRB instructions to restore graphic/visual costume effect references to the original costume address...
            Process::Patch(AddressList::getAddress("CostumeIDOffsetAuraA"), 0xE5D00064);
            Process::Patch(AddressList::getAddress("CostumeIDOffsetAuraB"), 0xE5D01064);
            Process::Patch(AddressList::getAddress("CostumeIDOffsetAuraC"), 0xE5D01064);
            Process::Patch(AddressList::getAddress("CostumeIDOffsetAuraD"), 0xE5D01064);
            Process::Patch(AddressList::getAddress("CostumeIDOffsetCheetah"), 0xE5D01064);
            Process::Patch(AddressList::getAddress("CostumeIDOffsetDune"), 0xE5D00064);
            Process::Patch(AddressList::getAddress("CostumeIDOffsetCheer"), 0xE5D00064);
            Process::Patch(AddressList::getAddress("CostumeIDOffsetSwordPtcl"), 0xE5D00064);

            // revert model loader BL instruction...
            // TODO: Need to know: Does this hold the same offset value in all regions, or not?
            Process::Patch(AddressList::getAddress("CostumeIDFunBLModel"), 0xEB0FDCF8);

            cosmeticCostumeAuto->Disable();
            entry->SetName("Enable Cosmetic Costumes");
        }
    }

    // Driver code for setting the costume MODEL | TODO: add support for custom costumes IF they are enabled...
    void Costume::setCosmeticCostume(MenuEntry *entry)
    {
        int linkChoice = GeneralHelpers::chooseLink();

        if (linkChoice >= 0)
        {
            u32 playerID = static_cast<u32>(linkChoice);
            u32 memoryOffset = playerID * PLAYER_OFFSET;

            // get current effect (true) costume...
            Process::Read8(AddressList::getAddress("CurrCostume") + memoryOffset, currEffectCostumeID);

            // get existing/current cosmetic costume ID, if any... else just fallback to true costume...
            if (cosmeticIDs[linkChoice] != cosmeticNotInUse)
                currCosmeticCostumeID = cosmeticIDs[linkChoice];
            else
                currCosmeticCostumeID = currEffectCostumeID;

            // choose a new cosmetic costume...
            selCosmeticCostume(linkChoice);
        }
    }

    // Menu interface for selecting cosmetic costume
    void selCosmeticCostume(int player)
    {
        std::string currEffectCostumeName = GameData::getCostumeNameFromID(currEffectCostumeID);
        std::string currCosmeticCostumeName = GameData::getCostumeNameFromID(currCosmeticCostumeID);
        std::string selectedPlayer, topscreenMessage;

        const StringVector setResetOptions =
        {
            "Set new",
            "Reset"
        };

        selectedPlayer = GeneralHelpers::getPlayerAsStr(player);

        topscreenMessage = "Set new cosmetic costume?\nOr reset to the effective costume?\n\nSelected: " + selectedPlayer + "\n\nCurrent effective costume: " + currEffectCostumeName + "\nCurrent cosmetic costume: " + currCosmeticCostumeName;

        Keyboard setReset(topscreenMessage);
        setReset.Populate(setResetOptions);

        int setResetResult = setReset.Open();
        if (setResetResult == 0)
        {
            // TODO: just get new costume using helper method? // remove redundant code
            Keyboard costumeList("Choose a costume:\n\nBe sure to load into a new area for changes to fully\ntake effect.");
            costumeList.Populate(GameData::universalCostumeList);

            int result = costumeList.Open();
            if (result >= 0)
                Costume::cosmeticIDs[player] = result;
        }
        else if (setResetResult == 1)
        {
            Costume::cosmeticIDs[player] = cosmeticNotInUse;
            Costume::setPlayerCostume(player, currEffectCostumeID, true);
        }
    }

    // Reflect cosmetic costume selection to alternate costume address
    void Costume::writeCosmeticCostume(MenuEntry *entry)
    {
        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
        {
            if (cosmeticIDs[iterateThruPlayers] != cosmeticNotInUse)
                Costume::setPlayerCostume(iterateThruPlayers, cosmeticIDs[iterateThruPlayers], true);
        }
    }
}
