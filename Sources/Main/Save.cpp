#include "Helpers.hpp"
#include "Main/Save.hpp"

namespace CTRPluginFramework
{
    MenuEntry* merchantA;
    MenuEntry* merchantB;
    MenuEntry* merchantC;
    MenuEntry* merchantD;
    MenuEntry* merchantE;

    void Save::useVoiceless(MenuEntry* entry) 
    {
        Process::Write8(AddressList::MainVoice.addr, 0x4);
    }

    void Save::mainVoice(MenuEntry* entry) 
    {
        Keyboard voiceMenu("Choose a voice:\n\nThese changes can be observable online. Keep in\nmind your selection will be overwritten by Voiceless,\nif used.");
        voiceMenu.Populate(GameData::voiceList);

        int result = voiceMenu.Open();

        if (result >= 0) 
        {
            Process::Write8(AddressList::MainVoice.addr, static_cast<u8>(result));
            entry->SetName("Set Main Voice: " + GameData::getVoiceAsStr(result));
        }
    }

    void Save::heroPointCountSet(MenuEntry* entry) 
    {
        u32 result;
        Keyboard heroPointInput("Set Hero Point count:");
        heroPointInput.IsHexadecimal(false);

        heroPointInput.OnKeyboardEvent([](Keyboard &kb, KeyboardEvent &event)
        {
            if (event.type == KeyboardEvent::CharacterAdded)
            {
                std::string &input = kb.GetInput();
                int value = std::stoi(input);

                if (value > 999)
                    input = "999";
                else if (value < 0)
                    input = "0";
            }
        });

        if (heroPointInput.Open(result) == 0) 
        {
            Process::Write32(AddressList::HeroPointCount.addr, result);
            entry->SetName("Edit Hero Point count: " + std::to_string(result));
        }
    }

    void Save::coliseumWinCountSet(MenuEntry* entry) 
    {
        u32 result;
        Keyboard coliseumWinInput("Set Coliseum Win count:");
        coliseumWinInput.IsHexadecimal(false);

        coliseumWinInput.OnKeyboardEvent([](Keyboard &kb, KeyboardEvent &event)
        {
            if (event.type == KeyboardEvent::CharacterAdded)
            {
                std::string &input = kb.GetInput();
                int value = std::stoi(input);

                if (value > 999)
                    input = "999";
                else if (value < 0)
                    input = "0";
            }
        });

        if (coliseumWinInput.Open(result) == 0) 
        {
            Process::Write32(AddressList::ColiseumWinCount.addr, result);
            entry->SetName("Edit Coliseum Win count: " + std::to_string(result));
        }
    }

    void Save::merchantSlotA(MenuEntry* entry) 
    {
        std::string material = openMerchantMatMenu(0);
        entry->SetName("Set 1st material slot: " << material);
    }

    void Save::merchantSlotB(MenuEntry* entry) 
    {
        std::string material = openMerchantMatMenu(2);
        entry->SetName("Set 2nd material slot: " << material);
    }

    void Save::merchantSlotC(MenuEntry* entry) 
    {
        std::string material = openMerchantMatMenu(4);
        entry->SetName("Set 3rd material slot: " << material);
    }

    void Save::merchantSlotD(MenuEntry* entry) 
    {
        std::string material = openMerchantMatMenu(6);
        entry->SetName("Set 4th material slot: " << material);
    }

    void Save::merchantSlotE(MenuEntry* entry) 
    {
        std::string material = openMerchantMatMenu(8);
        entry->SetName("Set 5th material slot: " << material);
    }
    
    // since each slot has a 8-bit spacer between them, slot number is just an offset
    // used to navigate between them without having to define 5 separate addresses
    std::string openMerchantMatMenu(u8 slotNumber) 
    {
        int world = Material::selectMaterialWorld();
        int material = Material::selectMaterialIndiv(world);
        
        Process::Write8(AddressList::EditMerchantStock.addr + slotNumber, static_cast<unsigned char>(material));
        return Material::getMaterialName(world, material);
    }

    void Save::resetMerchant(MenuEntry* entry) 
    {
        Process::Write8(AddressList::ResetMerchantStock.addr, 0x0);
        MessageBox(Color::Gainsboro << "Street Merchant Stall has been re-stocked.")();
    }

    void Save::selLevelCompletion(MenuEntry* entry) 
    {
        std::string msg, modeStr;
        int world = GameData::selWorld(false);

        if (world >= 0) 
        {
            int mode = selPlayMode();
            if (mode >= 0) 
            {
                std::string outro = "\n\nPress " + std::string(FONT_B) + " to save and exit this menu.";

                if (mode < 2)
                    modeStr = mode ? "Multiplayer Completion." : "Single-player Completion.";
                else
                    modeStr = "Multiplayer and Single-player\nCompletion";

                msg = "Currently editing Level Completion statuses\nfor " << GameData::worldIDToStr(world) << ", " << modeStr << outro;
                LevelStatusEditor(msg, GameData::getWorldNamesfromID(world), world, mode)();
            }
        }
    }

    int selPlayMode(void) 
    {
        Keyboard mode("Choose a mode:");
        static const StringVector modeList =
        {
            "Single-player",
            "Multiplayer",
            "Both"
        };

        mode.Populate(modeList);
        return mode.Open();
    }

    // TODO: owned costumes menu
    // TODO: name changer using custom keyboard
    // TODO: rupee amount changer
}