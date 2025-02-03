#include "Helpers.hpp"
#include "Main/Player.hpp"

namespace CTRPluginFramework
{
    /* The following are all helper/driver functions: */

    void Player::setSwordChanges(MenuEntry *entry)
    {
        if (entry->Name() == "Set custom sword model(s)")
        {
            openToggleMenu("Sword Model Type Menu", SWORD);
            entry->SetName("Disable custom sword model edits");
            swordEditAuto->Enable();
        }
        else
        {
            entry->SetName("Set custom sword model(s)");
            swordEditAuto->Disable();
        }
    }

    void Player::setSizeChanges(MenuEntry *entry)
    {
        if (entry->Name() == "Set custom player model size")
        {
            openToggleMenu("Player Size Menu", PLAYER_SIZE);
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
        std::string intro = "Jinx Model Status Menu\n\nEnable Jinx " << ENABLED_SLIDER << Color::White << "\nDisable Jinx " << DISABLED_SLIDER;
        if (entry->Name() == "Toggle jinxed model edits")
        {
            openToggleMenu(intro, NORMAL, &Player::jinxStatus);
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
        std::string intro = "Player Spawn Status Menu\n\nSpawned " << ENABLED_SLIDER << Color::White << "\nNot spawned " << DISABLED_SLIDER;
        if (entry->Name() == "Toggle player spawn edits")
        {
            openToggleMenu(intro, NORMAL, &Player::spawnStatus);
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
        std::string intro = "Player Model Visibility Menu\n\nVisible " << ENABLED_SLIDER << Color::White << "\nNot visible " << DISABLED_SLIDER;
        if (entry->Name() == "Toggle player visibility edits")
        {
            openToggleMenu(intro, NORMAL, &Player::visibleStatus);
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
        std::string intro = "Player Invincibility Status Menu\n\nEnable invincibility " << ENABLED_SLIDER << Color::White << "\nDisable invincibility " << DISABLED_SLIDER;
        if (entry->Name() == "Toggle player invincibility edits")
        {
            openToggleMenu(intro, NORMAL, &Player::invinciStatus);
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
        std::string intro = "Animation Storage Status Menu\n\nEnable animation storage " << ENABLED_SLIDER << Color::White << "\nDisable animation storage " << DISABLED_SLIDER;
        if (entry->Name() == "Toggle animation storage edits")
        {
            openToggleMenu(intro, NORMAL, &Player::animStoreStatus);
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
        std::string intro = "Player Collision Status Menu\n\nDefault collision " << ENABLED_SLIDER << Color::White << "\nNo collision " << DISABLED_SLIDER;
        if (entry->Name() == "Toggle collision edits (experimental!)")
        {
            openToggleMenu(intro, NORMAL, &Player::collisionStatus);
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
        std::string intro = "Player PvP Status Menu\n\nEnable PvP " << ENABLED_SLIDER << Color::White << "\nDisable PvP " << DISABLED_SLIDER;
        if (entry->Name() == "Toggle PvP damage edits")
        {
            openToggleMenu(intro, NORMAL, &Player::pvpStatus);
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
        float result;

        Keyboard sizeKB("Set player size:");
        sizeKB.IsHexadecimal(false);
        sizeKB.Open(result);

        if (result < 0.0)
            MessageBox(Color::Gainsboro << "Error", "Player Sizes cannot be negative.")();
        else
            return result;

        return 1.0;
    }
}