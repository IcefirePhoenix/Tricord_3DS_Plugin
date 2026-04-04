#include "Helpers.hpp"
#include "Main/Player.hpp"

namespace CTRPluginFramework
{
    /* The following are all helper/driver functions: */
    void Player::setSizeChanges(MenuEntry *entry)
    {
        if (entry->Name() == "Set custom player model size")
        {
            Player::PlayerSizes = setPlayerSize();

            if (Player::PlayerSizes > 0.0)
            {
                Process::WriteFloat(AddressList::getAddress("PlayerModelSize"), Player::PlayerSizes);
                entry->SetName("Disable custom player size edits");
            }
        }
        else
        {
            entry->SetName("Set custom player model size");
            Process::WriteFloat(AddressList::getAddress("PlayerModelSize"), 1.0);
        }
    }

    void Player::setJinxChanges(MenuEntry *entry)
    {
        if (entry->Name() == "Toggle jinxed model edits")
        {
            int result = PlayerSelector(true, Player::jinxStatus, "Jinxed Status Toggle Menu", "jinxed")();
            if (result > PLAYER_INVALID)
            {
                Player::jinxStatus = result;
                entry->SetName("Disable jinxed model edits");
                jinxEditAuto->Enable();
            }
        }
        else
        {
            entry->SetName("Toggle jinxed model edits");
            jinxEditAuto->Disable();
        }
    }

    void Player::setSpawnChanges(MenuEntry *entry)
    {
        if (entry->Name() == "Toggle player spawn edits")
        {
            int result = PlayerSelector(true, Player::spawnStatus, "Spawned Status Toggle Menu", "spawn")();
            if (result > PLAYER_INVALID)
            {
                Player::spawnStatus = result;
                entry->SetName("Disable player spawn edits");
                spawnEditAuto->Enable();
            }
        }
        else
        {
            entry->SetName("Toggle player spawn edits");
            spawnEditAuto->Disable();
        }
    }

    void Player::setVisibilityChanges(MenuEntry *entry)
    {
        if (entry->Name() == "Toggle player visibility edits")
        {
            int result = PlayerSelector(true, Player::visibleStatus, "Visibility Toggle Menu", "visibility")();
            if (result > PLAYER_INVALID)
            {
                Player::visibleStatus = result;
                entry->SetName("Disable player visibility edits");
                visibleEditAuto->Enable();
            }
        }
        else
        {
            entry->SetName("Toggle player visibility edits");
            visibleEditAuto->Disable();
        }
    }

    void Player::setInvincibilityChanges(MenuEntry *entry)
    {
        if (entry->Name() == "Toggle player invincibility edits")
        {
            int result = PlayerSelector(true, Player::invinciStatus, "Invincibility Toggle Menu", "invincibility")();
            if (result > PLAYER_INVALID)
            {
                Player::invinciStatus = result;
                entry->SetName("Disable player invincibility edits");
                invinciEditAuto->Enable();
            }
        }
        else
        {
            entry->SetName("Toggle player invincibility edits");
            invinciEditAuto->Disable();
        }
    }

    void Player::setAnimStorageChanges(MenuEntry *entry)
    {
        if (entry->Name() == "Toggle animation storage edits")
        {
            int result = PlayerSelector(true, Player::animStoreStatus, "Animation Storage Toggle Menu", "animation storage")();
            if (result > PLAYER_INVALID)
            {
                Player::animStoreStatus = result;
                entry->SetName("Disable animation storage edits");
                animStoreEditAuto->Enable();
            }
        }
        else
        {
            entry->SetName("Toggle animation storage edits");
            animStoreEditAuto->Disable();
        }
    }

    void Player::setCollisionChanges(MenuEntry *entry)
    {
        if (entry->Name() == "Toggle collision edits (experimental!)")
        {
            int result = PlayerSelector(true, Player::collisionStatus, "Collision Status Toggle Menu", "collision")();
            if (result > PLAYER_INVALID)
            {
                Player::collisionStatus = result;
                entry->SetName("Disable collision edits");
                colEditAuto->Enable();
            }
        }
        else
        {
            entry->SetName("Toggle collision edits (experimental!)");
            colEditAuto->Disable();
        }
    }

    void Player::setPVPChanges(MenuEntry *entry)
    {
        if (entry->Name() == "Toggle PvP damage edits")
        {
            int result = PlayerSelector(true, Player::pvpStatus, "PvP Status Toggle Menu", "PvP")();
            if (result > PLAYER_INVALID)
            {
                Player::pvpStatus = result;
                entry->SetName("Disable PvP damage edits");
                pvpEditAuto->Enable();
            }
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
        Keyboard sizeKB("Player Size", "Input a player size. The default value is 1.0. This value gets applied to all players.\n\nNote: Size values cannot be negative.");

        sizeKB.IsHexadecimal(false);
        sizeKB.DisableSignKey();
        sizeKB.Open(result, 1.0);

        return result;
    }
}