#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    MenuEntryLabel *colorLabel1;
    MenuEntryLabel *colorLabel2;
    MenuEntryLabel *colorLabel3;

    MenuEntryLabel *colorEntries[3] = {0};

    bool showChestContents, isScrollTextDisabled = false;

    /* ------------------ */

    // Wrapper for MenuEntry compatiblity; toggles HUD status
    void Rendering::triggerHideHUD(MenuEntry *entry)
    {
        Rendering::toggleHUD(true);

        if (!entry->IsActivated())
            Rendering::toggleHUD(false);
    }

    // Forces HUD to remain in its dormant animation frame, forcing it off-screen
    void Rendering::toggleHUD(bool hideHUD)
    {
        Process::Write8(AddressList::getAddress("HUDFlag"), hideHUD);
    }

    // Forces status message text to be transparent
    void Rendering::disableScrollingText(MenuEntry *entry)
    {
        isScrollTextDisabled = !isScrollTextDisabled;

        if (isScrollTextDisabled)
        {
            Process::WriteFloat(AddressList::getAddress("ScrollingTextOpacity"), 0.0);
            entry->SetName("Enable status messages");
        }
        else
        {
            Process::WriteFloat(AddressList::getAddress("ScrollingTextOpacity"), 1.0);
            entry->SetName("Disable status messages");
        }
    }

    // Allows player names' colors to be edited within status message text
    void Rendering::editLiveMsgColor(MenuEntry *entry)
    {
        u32 targetaddr, result, newColor;
        int playerChoice = GeneralHelpers::chooseLink();
        MenuEntryLabel *label = nullptr;

        switch (playerChoice)
        {
            case 0:
                targetaddr = AddressList::getAddress("LiveNameColorG");
                label = colorLabel1;
                break;
            case 1:
                targetaddr = AddressList::getAddress("LiveNameColorB");
                label = colorLabel2;
                break;
            case 2:
                targetaddr = AddressList::getAddress("LiveNameColorR");
                label = colorLabel3;
                break;
            default:
                return;
        }

        Keyboard HexColor("Custom Scrolling Text Name Color", "Enter a 6-digit RGB hex code.");
        HexColor.IsHexadecimal(true);
        HexColor.SetMaxLength(6);

        if (HexColor.Open(result) == 0)
        {
            u8 r = result >> 16;
            u8 g = result >> 8;
            u8 b = result;

            newColor = r + (g << 8) + (b << 16) + (0xFF << 24); // little-endian
            Process::Write32(targetaddr, newColor);

            // save to Settings
            Preferences::CustomNameColors[playerChoice] = newColor;
            label->SetName("Player " + std::to_string(playerChoice + 1) + " color: #" + Hex(r) + Hex(g) + Hex(b));
        }
    }

    // Places name color addresses in array for easier access
    void Rendering::initNameColorAddresses(void)
    {
        colorEntries[0] = colorLabel1;
        colorEntries[1] = colorLabel2;
        colorEntries[2] = colorLabel3;
    }

    // Refreshes the menu labels to display the current name color hexcode
    void updateNameColorLabels(void)
    {
        for (int color = 0; color < 3; color++)
        {
            u8 r = Preferences::CustomNameColors[color] & 0xFF;
            u8 g = (Preferences::CustomNameColors[color] >> 8) & 0xFF;
            u8 b = (Preferences::CustomNameColors[color] >> 16) & 0xFF;
            colorEntries[color]->SetName("Player " + std::to_string(color + 1) +" color: #" + Hex(r) + Hex(g) + Hex(b));
        }
    }

    // Updates status message name colors on boot
    void Rendering::loadCustomNameColors(void)
    {
        Rendering::initNameColorAddresses();

        Process::Write32(AddressList::getAddress("LiveNameColorG"), Preferences::CustomNameColors[0]);
        Process::Write32(AddressList::getAddress("LiveNameColorB"), Preferences::CustomNameColors[1]);
        Process::Write32(AddressList::getAddress("LiveNameColorR"), Preferences::CustomNameColors[2]);

        updateNameColorLabels();
    }

    // Resets custom name colors back to default -- bit shifting as done in-game is too extra here, using predefined color values instead
    void Rendering::restoreNameColors(MenuEntry *entry)
    {
        u32 defaultColors[3] = {0xFF40FF40, 0xFFFF4040, 0xFF4040FF}; // little-endian RGBA
        std::memcpy(Preferences::CustomNameColors, defaultColors, sizeof(defaultColors));

        Process::Write32(AddressList::getAddress("LiveNameColorG"), defaultColors[0]);
        Process::Write32(AddressList::getAddress("LiveNameColorB"), defaultColors[1]);
        Process::Write32(AddressList::getAddress("LiveNameColorR"), defaultColors[2]);

        updateNameColorLabels();

        MessageBox("Success", "Name colors have been reset!")();
    }


    // Display custom player respawn indicator arrows
    void Rendering::respawnIndicator(MenuEntry *entry)
    {
        float respawnCoords[3][3] =
        {
            {0.0, 0.0, 0.0}, // green
            {0.0, 0.0, 0.0}, // blue
            {0.0, 0.0, 0.0}  // red
        };

        u32 arrowAddresses[3] =
        {
            AddressList::getAddress("ArrowIndicatorColorTop"),
            AddressList::getAddress("ArrowIndicatorColorMid"),
            AddressList::getAddress("ArrowIndicatorColorBot")
        };

        u8 visibilityOffset = 0x4, indivCoordinateOffset = 0x4;
        u8 coordinateOffsetStart = 0x3C;
        u32 forceVisibility = 0x10FFFF00;

        // grab respawn coords...
        for (int iterateColor = 0; iterateColor < 3; iterateColor++)
        {
            u32 offset = iterateColor * PLAYER_OFFSET;
            Process::ReadFloat((AddressList::getAddress("RespawnPositionX") + offset), respawnCoords[iterateColor][0]);
            Process::ReadFloat((AddressList::getAddress("RespawnPositionY") + offset), respawnCoords[iterateColor][1]);
            Process::ReadFloat((AddressList::getAddress("RespawnPositionZ") + offset), respawnCoords[iterateColor][2]);
        }

        for (int iterateEdits = 0; iterateEdits < 3; iterateEdits++)
        {
            Process::Write8(arrowAddresses[iterateEdits], GameData::generalPlayerIDs[iterateEdits]); // set arrow color
            Process::Write32(arrowAddresses[iterateEdits] + visibilityOffset, forceVisibility);      // force arrow visibility
        }

        for (int iterateArray = 0; iterateArray < 3; iterateArray++)
        {
            Process::WriteFloat((arrowAddresses[0] + (iterateArray * 0x4) - coordinateOffsetStart), respawnCoords[0][iterateArray]);
            Process::WriteFloat((arrowAddresses[1] + (iterateArray * 0x4) - coordinateOffsetStart), respawnCoords[1][iterateArray]);
            Process::WriteFloat((arrowAddresses[2] + (iterateArray * 0x4) - coordinateOffsetStart), respawnCoords[2][iterateArray]);
        }
    }

    // Force visibility of chest contents -> Linebeck's Uniform costume ability
    void Rendering::seeChestContents(MenuEntry *entry)
    {
        showChestContents = !showChestContents;

        if (showChestContents)
        {
            Process::Patch(AddressList::getAddress("SeeChestContents"), 0xEA000017);
            entry->SetName("Disable forced-visibility of Treasure Chest contents");
        }
        else
        {
            Process::Patch(AddressList::getAddress("SeeChestContents"), 0x0A000017);
            entry->SetName("Force visibility of Treasure Chest contents");
        }
    }
}