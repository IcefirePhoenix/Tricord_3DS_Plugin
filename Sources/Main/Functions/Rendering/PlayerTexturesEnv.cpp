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
        Keyboard chooseLink("Player Texture Swapper", "Choose which Link's texture to edit.");
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
        Keyboard texture("Player Texture Swapper", "Choose a texture to load for " + linkName + " Link.");
        texture.Populate(linkList);
        tex = texture.Open();

        if (tex >= 0)
        {
            u32 targetaddr = AddressList::getAddress("TextureName") + link * texNameOffset;
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
            "Coliseum"
        };

        Keyboard lsType("Loading Screen Layout Modifier", "Choose an appearance for the single player No Challenge loading screen.");
        lsType.Populate(lsTypeList);

        switch (lsType.Open())
        {
            case 0:
                // Default single player - Green Link, Blue and Red Doppels
                Process::Patch(AddressList::getAddress("LoadingScreenSPForcedLink"), 0xE3510000);
                Process::WriteFloat(AddressList::getAddress("LoadingScreenSPNoChal"), 3);
                Process::WriteFloat(AddressList::getAddress("LoadingScreenSPIcons"), 3);
                break;
            case 1:
                // Multiplayer - All three Links; Forced Link does not matter
                Process::WriteFloat(AddressList::getAddress("LoadingScreenSPNoChal"), 3);
                Process::WriteFloat(AddressList::getAddress("LoadingScreenSPIcons"), 0);
                break;
            case 2:
                // Coliseum 3P - All three panels
                Process::Patch(AddressList::getAddress("LoadingScreenSPForcedLink"), 0xE3510003);
                Process::WriteFloat(AddressList::getAddress("LoadingScreenSPNoChal"), 6);
                Process::WriteFloat(AddressList::getAddress("LoadingScreenSPIcons"), 6);
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

        Keyboard auraType("Costume Aura Setter", "Select a costume aura or reset to restore default costume checks.");
        auraType.Populate(auras);

        switch (auraType.Open())
        {
            case 0:
                Process::Patch(AddressList::getAddress("Aura"), 0x0A000018);
                break;
            case 1:
                Process::Patch(AddressList::getAddress("Aura"), 0xEA00001B);
                break;
            case 2:
                Process::Patch(AddressList::getAddress("Aura"), 0xEA000004);
                break;
            case 3:
                Process::Patch(AddressList::getAddress("Aura"), 0xEA00000E);
                break;
        }
    }

    // Force-toggles display of Cheer Outfit pompom accessory model
    void Rendering::forcePomPom(MenuEntry *entry)
    {
        if (entry->Name() == "Force visibility of Cheer Outfit pom poms")
        {
            Process::Patch(AddressList::getAddress("CheerPomPom"), 0x1A000009);
            entry->SetName("Hide Cheer Outfit pom poms outside Couture's");
        }
        else
        {
            Process::Patch(AddressList::getAddress("CheerPomPom"), 0x0A000009);
            entry->SetName("Force visibility of Cheer Outfit pom poms");
        }
    }

    // Force-toggles Sword Suit env particles
    void Rendering::forceBlueSwordParticles(MenuEntry *entry)
    {
        if (entry->Name() == "Force Sword Suit blue sword particles")
        {
            Process::Patch(AddressList::getAddress("SwordParticle"), 0xEA000038);
            entry->SetName("Reset sword particles");
        }
        else
        {
            Process::Patch(AddressList::getAddress("SwordParticle"), 0x0A000062);
            entry->SetName("Force Sword Suit blue sword particles");
        }
    }

    // Force-toggles player light emittance
    void Rendering::disablePlayerLight(MenuEntry *entry)
    {
        if (entry->Name() == "Disable player light sources in dark stages")
        {
            Process::WriteFloat(AddressList::getAddress("PlayerLuminanceNormalA"), 0);
            Process::WriteFloat(AddressList::getAddress("PlayerLuminanceNormalB"), 0);
            Process::WriteFloat(AddressList::getAddress("PlayerLuminanceLightArmor"), 0);
            entry->SetName("Enable player light sources in dark stages");
        }
        else
        {
            Process::WriteFloat(AddressList::getAddress("PlayerLuminanceNormalA"), 1);
            Process::WriteFloat(AddressList::getAddress("PlayerLuminanceNormalB"), 0.5);
            Process::WriteFloat(AddressList::getAddress("PlayerLuminanceLightArmor"), 0.89);
            entry->SetName("Disable player light sources in dark stages");
        }
    }

    // Custom menu interface for changing player facial expressions
    void Rendering::editFaceExpr(MenuEntry *entry)
    {
        Keyboard chooseFrame("Facial Expression Editor", "Select a facial expression to edit.\n\nIdle: This is Link's normal facial expression.");
        StringVector frameList =
        {
            "Idle",
            "Shocked",
            "Death / DMG",
            "Triforce Warp",
            "Low HP / Failed Challenge",
            "Fall / Drown / Capture"
        };

        chooseFrame.DisplayTopScreen = true;
        chooseFrame.Populate(frameList);

        chooseFrame.OnKeyboardEvent([](Keyboard &kb, KeyboardEvent &event)
        {
            if (event.type == KeyboardEvent::SelectionChanged)
            {
                std::string &msg = kb.GetMessage();
                msg.clear();
                msg.append("Select a facial expression to edit.\n\n");

                switch (kb.GetLastSelectedEntry())
                {
                    case 0:
                        msg.append("Idle: This is Link's normal facial expression.");
                        break;
                    case 1:
                        msg.append("Shocked: This is Link's expression when colliding into objects, getting caught in Boomerangs, or being surprised by totem-related actions.");
                        break;
                    case 2:
                        msg.append("Death / DMG: Link makes this expression when taking damage or when all HP has been depleted.");
                        break;
                    case 3:
                        msg.append("Triforce Warp: Link makes this expression during the Triforce Warp sequence.");
                        break;
                    case 4:
                        msg.append("Low HP / Failed Challenge: This is Link's expression when running low on HP or when failing a challenge.");
                        break;
                    case 5:
                        msg.append("Fall / Drown / Capture: Link makes this expression when captured by a Life-Like or when entering a fall plane. This includes waterfalls, lava, and quicksand.");
                        break;
                }
            }
        });

        int frame = chooseFrame.Open();
        if (frame >= 0)
            FaceExprEditor(frame, frameList[frame])();
    }

    void Rendering::manualTriggerResetExprs(MenuEntry* entry)
    {
        FaceExprEditor::resetExprs();
    }


    void Rendering::hideDoppelMasks(MenuEntry *entry)
    {
        if (entry->Name() == "Hide Doppel masks")
        {
            for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
                Process::Write8(AddressList::getAddress("DoppelMaskVisibility") + iterateThruPlayers * PLAYER_OFFSET, 0x00);
            entry->SetName("Show Doppel masks");
        }
        else
        {
            for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
                Process::Write8(AddressList::getAddress("DoppelMaskVisibility") + iterateThruPlayers * PLAYER_OFFSET, 0x01);
            entry->SetName("Hide Doppel masks");
        }
    }
}