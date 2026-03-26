#include "Cheats.hpp"
#include "Main/Gameplay.hpp"

namespace CTRPluginFramework
{
    MenuEntry *physicsSelG;
    MenuEntry *physicsSelB;
    MenuEntry *physicsSelR;
    MenuEntry *physicsEditAutoG;
    MenuEntry *physicsEditAutoB;
    MenuEntry *physicsEditAutoR;
    MenuEntry *moonJumpEntry;
    MenuEntry *flightEntry;
    MenuEntry *ascentSpeedSet;
    MenuEntry *descentSpeedSet;

    float ascentSpeed = 0.5f, descentSpeed = -0.5f, lateralSpeed = 0.1f, speedToMaintainHover = 0.025f;
    u16 physicsStatus[3];

    const StringVector speedOptions =
    {
        "Weak",
        "Medium",
        "Strong"
    };

    /* ------------------ */

    // Driver code for setting custom collision physics types + displays edits in plugin menu
    void Gameplay::changePhysics(MenuEntry *entry)
    {
        MenuEntry *physicsEntries[3] =
        {
            physicsEditAutoG,
            physicsEditAutoB,
            physicsEditAutoR
        };

        // multiple MenuEntries call this function... to differentiate between them, use MenuEntry Arg value...
        int player = reinterpret_cast<int>(entry->GetArg());
        std::string baseEntryName, result = physicsSelectMenu(player);

        std::string longName = entry->Name();
        std::size_t colonPosition = longName.find(':');

        // get the entry's base name -- everything before the colon...
        if (colonPosition != std::string::npos)
            baseEntryName = longName.substr(0, colonPosition);

        // don't do anything if nothing was selected...
        if (!result.empty())
        {
            if (result == "Not edited")
                physicsEntries[player]->Disable();
            else
                physicsEntries[player]->Enable();

            entry->SetName(baseEntryName + ": " + result);
        }
    }

    // Sets the Collision ID for selected player and returns the collision type as string
    std::string physicsSelectMenu(int player)
    {
        const StringVector physicsList =
        {
            "Reset",
            "Water",
            "Lava",
            "Ice",
            "Quicksand"
        };

        Keyboard physics("Physics Selection Menu", "Choose a type of physics.");
        physics.Populate(physicsList);

        switch (physics.Open())
        {
            case 0:
                return "Not edited";
            case 1:
                physicsStatus[player] = Collision::colIDFromName("Water");
                return "Water";
            case 2:
                physicsStatus[player] = Collision::colIDFromName("Lava");
                return "Lava";
            case 3:
                physicsStatus[player] = Collision::colIDFromName("Ice");
                return "Ice";
            case 4:
                physicsStatus[player] = Collision::colIDFromName("Quicksand_plane");
                return "Quicksand";
            default:
                return "";
        }
    }

    // Sets custom collision edits
    void Gameplay::writePhysicsChanges(MenuEntry *entry)
    {
        // multiple MenuEntries call this function... to differentiate between them, use MenuEntry Arg value...
        int player = reinterpret_cast<int>(entry->GetArg());

        u8 sinkingStatus;
        u16 currentColID, targetColID = physicsStatus[player];
        u32 addressOffset = player * PLAYER_OFFSET;

        /**
         * Note: do not freeze collision if:
         * 1) if Link is sinking in quicksand -- prevents infinite sinking if quicksand is active
         * 2) Link is airborne -- ensures fall zones works properly
         * 3) if Link is swimming -- ensures water/lava acts properly
         */

        Process::Read8(AddressList::getAddress("StatusBitE") + addressOffset, sinkingStatus);  // check 1...
        bool isSinking = (sinkingStatus & 0x80) == 0x80;

        currentColID = Collision::getCurrCol(player);                                   // check 2 + 3...
        bool checkValidColWrite =
            currentColID != Collision::colIDFromName("Water") &&
            currentColID != Collision::colIDFromName("Lava") &&
            currentColID != Collision::colIDFromName("Air") &&
            currentColID != Collision::colIDFromName("Fall_plane");

        // set custom collision edits if checks pass...
        if (!isSinking && checkValidColWrite)
            Collision::setCurrCol(player, targetColID);
    }

    // Driver code for flight and/or moon jumping
    void Gameplay::moonJumpAndFlight(MenuEntry *entry)
    {
        /**
         * Hotkeys: Ascend, Descend
         * Note: South and East are positive, North and West are negative
         */

        // Disable X-button screenshot functionality to avoid hotkey conflicts
        // Also disables "disabled action" beep SFX on the B, Y and L/R buttons
        // (B is a hotkey for flight, but all those buttons are tied together)
        if (entry->WasJustActivated())
        {
            Process::Patch(AddressList::getAddress("CameraXButtonDisable"), 0xEA000028);
            Process::Patch(AddressList::getAddress("DisabledActionSE"), 0xEA000008);
        }
        if (!entry->IsActivated())
        {
            Process::Patch(AddressList::getAddress("CameraXButtonDisable"), 0x0A000028);
            Process::Patch(AddressList::getAddress("DisabledActionSE"), 0x0A000008);
        }

        u16 currColl;
        bool isOnTriforceGate;
        int player = GeneralHelpers::getCurrLink();

        currColl = Collision::getCurrCol(player);
        isOnTriforceGate = (currColl == Collision::colIDFromName("Triforce_gate"));

        // Only apply edits if NOT standing on a Triforce gate, to avoid crashes on warp
        if (!isOnTriforceGate)
        {
            u32 offset = player * PLAYER_OFFSET;
            u32 addrX = AddressList::getAddress("SpeedX") + offset;
            u32 addrY = AddressList::getAddress("SpeedY") + offset;
            u32 addrZ = AddressList::getAddress("SpeedZ") + offset;

            bool isAirborne;

            isAirborne = (currColl == Collision::colIDFromName("Air"));

            // Allow lateral movement while airborne
            if (isAirborne)
            {
                // Translate Circle Pad input into lateral movement
                // As the game normally does not let you change trajectory in midair
                // This implementation offers 360 degree movement, unlike 8 directions from just taking Left/Right/Up/Down input
                float CirclePadXCoord, CirclePadYCoord;
                Process::ReadFloat(AddressList::getAddress("CPadXCoord"), CirclePadXCoord);
                Process::ReadFloat(AddressList::getAddress("CPadYCoord"), CirclePadYCoord);
                Process::WriteFloat(addrX, lateralSpeed * CirclePadXCoord);
                Process::WriteFloat(addrZ, lateralSpeed * CirclePadYCoord * -1.0f); //Circle Pad Y is reverse to Z-axis
            }

            // Ascent / moon jump
            if (entry->Name() == "Enable Moon Jump" && entry->Hotkeys[0].IsDown() && canApplyYSpeed(player))
                Process::WriteFloat(addrY, ascentSpeed);

            // Flight ascent, descent or hovering
            if (entry->Name() == "Enable Flight" && canApplyYSpeed(player))
            {
                if (entry->Hotkeys[0].IsDown())                         // ascend
                    Process::WriteFloat(addrY, ascentSpeed);
                else if (entry->Hotkeys[1].IsDown())                    // descend
                    Process::WriteFloat(addrY, descentSpeed);
                else if (isAirborne)                                    // hover in place
                    Process::WriteFloat(addrY, speedToMaintainHover);
            }
        }
    }

    // Keep the two players you aren't currently controlling hovering in place while airborne
    void Gameplay::forceHover(MenuEntry *entry)
    {
        int link = GeneralHelpers::getCurrLink();
        u16 currColl, lastColl;
        bool isAirborne, wasOnTriforceGate;
        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
        {
            if (iterateThruPlayers != link && canApplyYSpeed(iterateThruPlayers))
            {
                currColl = Collision::getCurrCol(iterateThruPlayers);
                isAirborne = (currColl == Collision::colIDFromName("Air"));
                lastColl = Collision::getLastCol(iterateThruPlayers);
                wasOnTriforceGate = (lastColl == Collision::colIDFromName("Triforce_gate"));
                if (isAirborne && !wasOnTriforceGate)
                {
                    Process::WriteFloat(AddressList::getAddress("SpeedX") + iterateThruPlayers * PLAYER_OFFSET, 0.0);
                    Process::WriteFloat(AddressList::getAddress("SpeedY") + iterateThruPlayers * PLAYER_OFFSET, speedToMaintainHover);
                    Process::WriteFloat(AddressList::getAddress("SpeedZ") + iterateThruPlayers * PLAYER_OFFSET, 0.0);
                }
            }
        }
    }

    // Select which height adjustment speed to edit: ascension or descension
    void Gameplay::selAscentDescentEdit(MenuEntry *entry)
    {
        std::string newEntryName = "";
        float newSpeed;
        int entryID = reinterpret_cast<int>(entry->GetArg());

        newSpeed = (entryID == 1) ? setHeightAdjustmentSpeed(ascentSpeed) : setHeightAdjustmentSpeed(descentSpeed);
        newEntryName = (entryID == 1) ? "Adjust ascent speed: " : "Adjust descent speed: ";

        // absolute value used in case original descentSpeed is returned...
        newSpeed = fabsf(newSpeed);

        if (newSpeed == 0.2f)
            newEntryName += "Weak";
        else if (newSpeed == 0.5f)
            newEntryName += "Medium";
        else if (newSpeed == 0.7f)
            newEntryName += "Strong";
        else
            newEntryName += "?";

        if (entryID == 1)
            ascentSpeed = newSpeed;
        else
            descentSpeed = (newSpeed * -1.0f);

        entry->SetName(newEntryName);
    }

    // Adjust ascension/descension speed
    float setHeightAdjustmentSpeed(float originalSpeed)
    {
        float chosenSpeed = originalSpeed;

        Keyboard speed("Height Adjustment Speed", "Choose a speed level.");
        speed.Populate(speedOptions);

        switch (speed.Open())
        {
            case 0:
                chosenSpeed = 0.2f;
                break;
            case 1:
                chosenSpeed = 0.5f;
                break;
            case 2:
                chosenSpeed = 0.7f;
                break;
        }

        // original speed gets returned if nothing was chosen
        return chosenSpeed;
    }

    // Adjust lateral movement speed
    void Gameplay::adjustLateralSpeed(MenuEntry *entry)
    {
        Keyboard speed("Lateral Movement Speed", "Choose a speed level.");
        speed.Populate(speedOptions);

        switch (speed.Open())
        {
            case 0:
                lateralSpeed = 0.05f;
                entry->SetName("Adjust lateral speed: Weak");
                break;
            case 1:
                lateralSpeed = 0.1f;
                entry->SetName("Adjust lateral speed: Medium");
                break;
            case 2:
                lateralSpeed = 0.2f;
                entry->SetName("Adjust lateral speed: Strong");
                break;
        }
    }

    // Returns true if the player is not currently spawning into a stage, respawning from a fall
    // For the purpose of not applying y axis speed during these times
    bool canApplyYSpeed(int player)
    {
        u8 respawning, invincible;
        // Link's shadow is invisible while falling out of bounds / respawning, so we can check this indicator
        Process::Read8(AddressList::getAddress("PlayerShadowVisibility") + player * PLAYER_OFFSET, respawning);
        // Extra check for water collision and invincibility frames since these also hide Link's shadow
        Process::Read8(AddressList::getAddress("IsInvincible") + player * PLAYER_OFFSET, invincible);
        return (respawning == 0 && Level::hasCertainTimeElapsed(100)) || Collision::getCurrCol(player) == Collision::colIDFromName("Water") || invincible != 0;
    }
}