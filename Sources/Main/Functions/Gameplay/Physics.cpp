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
         * Hotkeys: North, South, East, West, Ascend
         * Note: South and East are positive, North and West are negative
         */

        u32 offset = GeneralHelpers::getCurrLink() * PLAYER_OFFSET;
        u32 addrX = AddressList::getAddress("SpeedX") + offset;
        u32 addrY = AddressList::getAddress("SpeedY") + offset;
        u32 addrZ = AddressList::getAddress("SpeedZ") + offset;

        u16 currColl;
        bool isAirborne, canMoveInMoonJump;

        // disable X-button screenshot functionality to avoid hotkey conflicts...
        Process::Write8(AddressList::getAddress("CameraXButtonToggle"), cameraDisabled);

        // get current collision type...
        Process::Read16(AddressList::getAddress("CollisionCurrent") + offset, currColl);

        // verify moonjump status...
        isAirborne = (currColl == Collision::colIDFromName("Air"));
        canMoveInMoonJump = isAirborne && (entry->Name() == "Enable Moon Jump");

        // allow lateral movement during active moon jumps OR if currently flying...
        if (canMoveInMoonJump || entry->Name() == "Enable Flight")
        {
            if (entry->Hotkeys[0].IsDown())                             // move player north
                Process::WriteFloat(addrZ, (lateralSpeed * -1.0));
            else if (entry->Hotkeys[1].IsDown())                        // move player south
                Process::WriteFloat(addrZ, lateralSpeed);
            else                                                        // no input = no movement along Z-axis
                Process::WriteFloat(addrZ, 0);

            if (entry->Hotkeys[2].IsDown())                             // move player east
                Process::WriteFloat(addrX, lateralSpeed);
            else if (entry->Hotkeys[3].IsDown())                        // move player west
                Process::WriteFloat(addrX, (lateralSpeed * -1.0));
            else                                                        // no input = no movement along X-axis
                Process::WriteFloat(addrX, 0);
        }

        // TODO: check logic here
        // when NOT moon jumping...
        if (!isAirborne && (entry->Name() == "Enable Moon Jump"))
        {
            if (entry->Hotkeys[4].IsDown())                 // actively try to ascend/moon jump
                Process::WriteFloat(addrY, ascentSpeed);
            else                                            // auto-descend
                Process::WriteFloat(addrY, descentSpeed);
        }

        // when currently flying...
        if (entry->Name() == "Enable Flight")
        {
            if (entry->Hotkeys[4].IsDown())                         // ascend
                Process::WriteFloat(addrY, ascentSpeed);
            else if (entry->Hotkeys[5].IsDown())                    // descend
                Process::WriteFloat(addrY, descentSpeed);
            else                                                    // hover in place
                Process::WriteFloat(addrY, speedToMaintainHover);
        }
    }

    // Keep the two players you aren't currently controlling hovering in place
    void Gameplay::forceHover(MenuEntry *entry)
    {
        int link = GeneralHelpers::getCurrLink();
        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
        {
            if (iterateThruPlayers != link)
            {
                Process::WriteFloat(AddressList::getAddress("SpeedX") + iterateThruPlayers * PLAYER_OFFSET, 0.0);
                Process::WriteFloat(AddressList::getAddress("SpeedY") + iterateThruPlayers * PLAYER_OFFSET, speedToMaintainHover);
                Process::WriteFloat(AddressList::getAddress("SpeedZ") + iterateThruPlayers * PLAYER_OFFSET, 0.0);
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
                lateralSpeed = 0.04;
                entry->SetName("Adjust lateral speed: Weak");
                break;
            case 1:
                lateralSpeed = 0.09;
                entry->SetName("Adjust lateral speed: Medium");
                break;
            case 2:
                lateralSpeed = 0.2;
                entry->SetName("Adjust lateral speed: Strong");
                break;
        }
    }
}