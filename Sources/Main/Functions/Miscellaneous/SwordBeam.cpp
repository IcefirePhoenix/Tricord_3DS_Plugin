#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    MenuEntry *autoBeamCooldown = nullptr;

    bool beamStatuses[3] = {false, false, false};
    bool useBeamCooldown;

    /* ------------------ */

    // Menu interface for force-toggling beam cooldowns for individual players
    void Miscellaneous::selectLinkBeam(MenuEntry *entry)
    {
        std::string title;
        StringVector bottomScreenOptions;
        Keyboard kbd("Menu");

        kbd.CanAbort(false);
        bool loop = true;

        while (loop)
        {
            title = "Use the toggles to disable the Sword Beam cooldown period:\n\n";

            bottomScreenOptions.clear();
            bottomScreenOptions.push_back(std::string("Player 1 ") << (beamStatuses[0] ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string("Player 2 ") << (beamStatuses[1] ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string("Player 3 ") << (beamStatuses[2] ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back("Save changes");
            bottomScreenOptions.push_back("Disable entry");

            kbd.GetMessage() = title;
            kbd.Populate(bottomScreenOptions);

            switch (kbd.Open())
            {
                case 0:
                    beamStatuses[0] = !beamStatuses[0];
                    break;
                case 1:
                    beamStatuses[1] = !beamStatuses[1];
                    break;
                case 2:
                    beamStatuses[2] = !beamStatuses[2];
                    break;
                case 3:
                    autoBeamCooldown->Enable();
                    loop = false;
                    break;
                default:
                    autoBeamCooldown->Disable();
                    loop = false;
                    break;
            }
        }
    }

    // Helper function to write cooldown edits to memory
    void Miscellaneous::setBeamCooldown(MenuEntry *entry)
    {
        u8 minBeamCooldownTimer = 0x1E;

        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
        {
            if (beamStatuses[iterateThruPlayers])
                Process::Write8(AddressList::SwordBeamCD.addr + (PLAYER_OFFSET * iterateThruPlayers), minBeamCooldownTimer);
        }
    }
}