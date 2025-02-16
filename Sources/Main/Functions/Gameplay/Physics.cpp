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

    float ascentSpeed = 0.5, descentSpeed = -0.5, lateralSpeed = 0.09, speedToMaintainHover = 0.025;
    u16 physicsStatus[3];
    u8 cameraDisabled = 0x0;

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

        Keyboard physics("Choose a type of physics:");
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

            // disable X-button screenshot functionality to avoid hotkey conflicts...
            Process::Write8(AddressList::getAddress("CameraXButtonToggle"), cameraDisabled);

            isAirborne = (currColl == Collision::colIDFromName("Air"));

            // Allow lateral movement while airborne and not warping on a triforce gate
            if (isAirborne)
            {
                // Translate Circle Pad input into lateral movement
                // As the game normally does not let you change trajectory in midair
                // This implementation offers 360 degree movement, unlike 8 directions from just taking Left/Right/Up/Down input
                float CirclePadXCoord, CirclePadYCoord;
                Process::ReadFloat(AddressList::getAddress("CPadXCoord"), CirclePadXCoord);
                Process::ReadFloat(AddressList::getAddress("CPadYCoord"), CirclePadYCoord);
                Process::WriteFloat(addrX, lateralSpeed * CirclePadXCoord);
                Process::WriteFloat(addrZ, lateralSpeed * CirclePadYCoord * -1.0); //Circle Pad Y is reverse to Z-axis
            }

            // Ascent / moon jump
            if (entry->Name() == "Enable Moon Jump" && entry->Hotkeys[0].IsDown() && canApplyYSpeed(player))
                Process::WriteFloat(addrY, ascentSpeed);

            // Flight descent / hovering
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

        // complete entry title string...
        if (newSpeed == 0.2)
            newEntryName += "Weak";
        else if (newSpeed == 0.5)
            newEntryName += "Medium";
        else if (newSpeed == 0.7)
            newEntryName += "Strong";

        // update height adjustment values...
        if (entryID == 1)
            ascentSpeed = newSpeed;
        else
            descentSpeed = (newSpeed * -1.0);

        // update entry title with edits...
        entry->SetName(newEntryName);
    }

    // Adjust ascension/descension speed
    float setHeightAdjustmentSpeed(float originalSpeed)
    {
        float chosenSpeed = originalSpeed;

        Keyboard speed("Choose a speed level:");
        speed.Populate(speedOptions);

        switch (speed.Open())
        {
            case 0:
                chosenSpeed = 0.2;
                break;
            case 1:
                chosenSpeed = 0.5;
                break;
            case 2:
                chosenSpeed = 0.7;
                break;
        }

        // original speed gets returned if nothing was chosen
        return chosenSpeed;
    }

    // Adjust lateral movement speed
    void Gameplay::adjustLateralSpeed(MenuEntry *entry)
    {
        Keyboard speed("Choose a speed level:");
        speed.Populate(speedOptions);

        switch (speed.Open())
        {
            case 0:
                lateralSpeed = 0.05;
                entry->SetName("Adjust lateral speed: Weak");
                break;
            case 1:
                lateralSpeed = 0.1;
                entry->SetName("Adjust lateral speed: Medium");
                break;
            case 2:
                lateralSpeed = 0.2;
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