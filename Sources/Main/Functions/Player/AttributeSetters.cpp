#include "Helpers.hpp"
#include "Main/Player.hpp"

namespace CTRPluginFramework
{
    /* The following are all helper/driver functions: */
    void Player::setSizeChanges(MenuEntry *entry)
    {
        if (entry->Name() == "Set custom player model size")
        {
            openToggleMenu("Player Size", "Enter a custom player size.\n\nNote: This value gets applied to all players.", PLAYER_SIZE);
            entry->SetName("Disable custom player size edits");

            if (Player::PlayerSizes > 0.0)
                Process::WriteFloat(AddressList::getAddress("PlayerModelSize"), Player::PlayerSizes);
        }
        else
        {
            entry->SetName("Set custom player model size");
            Process::WriteFloat(AddressList::getAddress("PlayerModelSize"), 1.0);
        }
    }

    void Player::setJinxChanges(MenuEntry *entry)
    {
        std::string desc = "Use the toggles to change the jinxed status for each player.\n\n";
        desc += ENABLED_SLIDER << Color::White << " Enable jinx\n";
        desc += DISABLED_SLIDER << Color::White << " Disable jinx";

        if (entry->Name() == "Toggle jinxed model edits")
        {
            openToggleMenu("Jinxed Model Status Menu", desc, NORMAL, &Player::jinxStatus);
            entry->SetName("Disable jinxed model edits");
            jinxEditAuto->Enable();
        }
        else
        {
            entry->SetName("Toggle jinxed model edits");
            jinxEditAuto->Disable();
        }
    }

    void Player::setSpawnChanges(MenuEntry *entry)
    {
        std::string desc = "Use the toggles to change the spawned status for each player.\n\n";
        desc += ENABLED_SLIDER << Color::White << " Spawned\n";
        desc += DISABLED_SLIDER << Color::White << " Not spawned";

        if (entry->Name() == "Toggle player spawn edits")
        {
            openToggleMenu("Player Spawn Status Menu", desc, NORMAL, &Player::spawnStatus);
            entry->SetName("Disable player spawn edits");
            spawnEditAuto->Enable();
        }
        else
        {
            entry->SetName("Toggle player spawn edits");
            spawnEditAuto->Disable();
        }
    }

    void Player::setVisibilityChanges(MenuEntry *entry)
    {
        std::string desc = "Use the toggles to change the visibility status for each player.\n\n";
        desc += ENABLED_SLIDER << Color::White << " Visible\n";
        desc += DISABLED_SLIDER << Color::White << " Not visible";

        if (entry->Name() == "Toggle player visibility edits")
        {
            openToggleMenu("Player Model Visibility Menu", desc, NORMAL, &Player::visibleStatus);
            entry->SetName("Disable player visibility edits");
            visibleEditAuto->Enable();
        }
        else
        {
            entry->SetName("Toggle player visibility edits");
            visibleEditAuto->Disable();
        }
    }

    void Player::setInvincibilityChanges(MenuEntry *entry)
    {
        std::string desc = "Use the toggles to change the invincibility status for each player.\n\n";
        desc += ENABLED_SLIDER << Color::White << " Enable invincibility\n";
        desc += DISABLED_SLIDER << Color::White << " Disable invincibility";

        if (entry->Name() == "Toggle player invincibility edits")
        {
            openToggleMenu("Player Invincibility Status Menu", desc, NORMAL, &Player::invinciStatus);
            entry->SetName("Disable player invincibility edits");
            invinciEditAuto->Enable();
        }
        else
        {
            entry->SetName("Toggle player invincibility edits");
            invinciEditAuto->Disable();
        }
    }

    void Player::setAnimStorageChanges(MenuEntry *entry)
    {
        std::string desc = "Use the toggles to change the animation storage status for each player.\n\n";
        desc += ENABLED_SLIDER << Color::White << " Enable animation storage\n";
        desc += DISABLED_SLIDER << Color::White << " Disable animation storage";

        if (entry->Name() == "Toggle animation storage edits")
        {
            openToggleMenu("Animation Storage Status Menu", desc, NORMAL, &Player::animStoreStatus);
            entry->SetName("Disable animation storage edits");
            animStoreEditAuto->Enable();
        }
        else
        {
            entry->SetName("Toggle animation storage edits");
            animStoreEditAuto->Disable();
        }
    }

    void Player::setCollisionChanges(MenuEntry *entry)
    {
        std::string desc = "Use the toggles to change the collision status for each player.\n\n";
        desc += ENABLED_SLIDER << Color::White << " Default collision\n";
        desc += DISABLED_SLIDER << Color::White << " No collision";

        if (entry->Name() == "Toggle collision edits (experimental!)")
        {
            openToggleMenu("Player Collision Status Menu", desc, NORMAL, &Player::collisionStatus);
            entry->SetName("Disable collision edits");
            colEditAuto->Enable();
        }
        else
        {
            entry->SetName("Toggle collision edits (experimental!)");
            colEditAuto->Disable();
        }
    }

    void Player::setPVPChanges(MenuEntry *entry)
    {
        std::string desc = "Use the toggles to change the PvP status for each player.\n\n";
        desc += ENABLED_SLIDER << Color::White << " Enable PvP\n";
        desc += DISABLED_SLIDER << Color::White << " Disable PvP";

        if (entry->Name() == "Toggle PvP damage edits")
        {
            openToggleMenu("Player PvP Status Menu", desc, NORMAL, &Player::pvpStatus);
            entry->SetName("Disable PvP damage edits");
            pvpEditAuto->Enable();
        }
        else
        {
            entry->SetName("Toggle PvP damage edits");
            pvpEditAuto->Disable();
        }
    }

    float setPlayerSize(void)
    {
        float result = 1.0;

        Keyboard sizeKB("Player Size", "Input a player size. The default value is 1.0.\n\nNote: Size values cannot be negative.");
        sizeKB.IsHexadecimal(false);
        sizeKB.Open(result, 1.0);

        if (result < 0.0)
            MessageBox("Error", "Player size cannot be negative.")();
        else
            return result;

        return 1.0;
    }
}