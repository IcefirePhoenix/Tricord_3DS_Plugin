#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // Forces a specific player instance to use a specific colored texture set
    void Rendering::swapLinkTexture(MenuEntry *entry)
    {
        StringVector linkList =
        {
            "Hytopia Link",
            "Green Link",
            "Blue Link",
            "Red Link"
        };

        int link, tex;
        u8 texNameOffset = 0x7;
        u8 texNameDataStartIndex = 0x30;
        std::string linkName;

        // select player instance...
        Keyboard chooseLink("Choose which Link's texture to edit:");
        chooseLink.Populate(linkList);

        switch (chooseLink.Open())
        {
            case 0:
                link = 0;
                linkName = "Hytopia";
                break;
            case 1:
                link = 1;
                linkName = "Green";
                break;
            case 2:
                link = 2;
                linkName = "Blue";
                break;
            case 3:
                link = 3;
                linkName = "Red";
                break;
            default:
                return;
        }

        // select target color texture...
        Keyboard texture("Choose a texture to load for " + linkName + " link:");
        texture.Populate(linkList);
        tex = texture.Open();

        if (tex >= 0)
        {
            u32 targetaddr = AddressList::TextureName.addr + link * texNameOffset;
            u8 towrite = texNameDataStartIndex + tex;

            Process::Write8(targetaddr, towrite);
        }
    }

    // Forces the single-player loading screen to use a specific layout
    void Rendering::swapSPLoadingScreen(MenuEntry *entry)
    {
        StringVector lsTypeList =
        {
            "Default",
            "Multiplayer",
            "Coliseum V1",
            "Coliseum V2"
        };

        Keyboard lsType("Choose an appearance for the single player\nno-challenge loading screen:");
        lsType.Populate(lsTypeList);

        switch (lsType.Open())
        {
            case 0:
                Process::WriteFloat(AddressList::LoadingScreenSPNoChal.addr, 3);
                Process::WriteFloat(AddressList::LoadingScreenSPIcons.addr, 3);
                break;
            case 1:
                Process::WriteFloat(AddressList::LoadingScreenSPNoChal.addr, 3);
                Process::WriteFloat(AddressList::LoadingScreenSPIcons.addr, 0);
                break;
            case 2:
                // Blue and Green panels
                Process::WriteFloat(AddressList::LoadingScreenSPNoChal.addr, 6);
                Process::WriteFloat(AddressList::LoadingScreenSPIcons.addr, 5);
                break;
            case 3:
                // Red and Blue panels
                Process::WriteFloat(AddressList::LoadingScreenSPNoChal.addr, 6);
                Process::WriteFloat(AddressList::LoadingScreenSPIcons.addr, 6);
                break;
        }
    }

    // Force-displays costume-specific aura env effect
    void Rendering::forceAura(MenuEntry *entry)
    {
        /**
         * Assumption: Every region's version of this function is structured identically
         * and changes to branch target addresses are consistent
         */

        StringVector auras =
        {
            "Reset",
            "Bear Maximum",
            "Showstopper",
            "Fierce Deity Armor"
        };

        Keyboard auraType("Select a costume aura, or reset to restore\ncostume checks.");
        auraType.Populate(auras);

        switch (auraType.Open())
        {
            case 0:
                Process::Patch(AddressList::Aura.addr, 0x0A000018);
                break;
            case 1:
                Process::Patch(AddressList::Aura.addr, 0xEA00001B);
                break;
            case 2:
                Process::Patch(AddressList::Aura.addr, 0xEA000004);
                break;
            case 3:
                Process::Patch(AddressList::Aura.addr, 0xEA00000E);
                break;
        }
    }

    // Force-toggles display of Cheer Outfit pompom accessory model
    void Rendering::forcePomPom(MenuEntry *entry)
    {
        if (entry->Name() == "Force visibility of Cheer Outfit pom poms")
        {
            Process::Patch(AddressList::CheerPomPom.addr, 0x1A000009);
            entry->SetName("Hide Cheer Outfit pom poms outside Couture's");
        }
        else
        {
            Process::Patch(AddressList::CheerPomPom.addr, 0x0A000009);
            entry->SetName("Force visibility of Cheer Outfit pom poms");
        }
    }

    // Force-toggles Sword Suit env particles
    void Rendering::forceBlueSwordParticles(MenuEntry *entry)
    {
        if (entry->Name() == "Force Sword Suit blue sword particles")
        {
            Process::Patch(AddressList::SwordParticle.addr, 0xEA000038);
            entry->SetName("Reset sword particles");
        }
        else
        {
            Process::Patch(AddressList::SwordParticle.addr, 0x0A000062);
            entry->SetName("Force Sword Suit blue sword particles");
        }
    }

    // Force-toggles player light emittance
    void Rendering::disablePlayerLight(MenuEntry *entry)
    {
        if (entry->Name() == "Disable player light sources in dark stages")
        {
            Process::WriteFloat(AddressList::PlayerLuminanceNormalA.addr, 0);
            Process::WriteFloat(AddressList::PlayerLuminanceNormalB.addr, 0);
            Process::WriteFloat(AddressList::PlayerLuminanceLightArmor.addr, 0);
            entry->SetName("Enable player light sources in dark stages");
        }
        else
        {
            Process::WriteFloat(AddressList::PlayerLuminanceNormalA.addr, 1);
            Process::WriteFloat(AddressList::PlayerLuminanceNormalB.addr, 0.5);
            Process::WriteFloat(AddressList::PlayerLuminanceLightArmor.addr, 0.89);
            entry->SetName("Disable player light sources in dark stages");
        }
    }

    // Custom menu interface for changing player facial expressions
    void Rendering::editFaceExpr(MenuEntry *entry)
    {
        // 11 expressions total; unclear where they are all used... for now, only use Idle
        StringVector expressionList =
        {
            "Idle",
            "?",
            "?",
            "?",
            "?",
            "?",
            "?",
            "?",
            "?",
            "?",
            "?"
        };

        // TODO: use when multiple facial expressions are supported:
        // Keyboard expr("Select a facial expression to edit.");
        // expr.Populate(expressionList);

        // int selection = expr.Open();
        // if (selection >= 0)
        //     FaceSelMenu(selection, expressionList[selection])();

        FaceSelMenu(0, expressionList[0])(); // Idle expression
    }
}