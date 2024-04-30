#include "Helpers.hpp"
#include "Main/Rendering.hpp"

namespace CTRPluginFramework
{
    bool showChestContents, isScrollTextDisabled = false;

    void Rendering::triggerHideHUD(MenuEntry* entry) 
    {
        hideHUD();
    }

    void hideHUD(void) 
    {
        u32 HUDPointerCheck;
        Process::Read32(AddressList::HUDPointer.addr, HUDPointerCheck);

        u8 disableOffScrArrow = 0x10;
        u32 mainHUDoffset = 0x18;
        u32 ActionButtonBGOffset = 0x75D0;
        u32 ActionButtonTextOffset = 0x78B0;

        if (!GeneralHelpers::isNullPointer(HUDPointerCheck)) 
        {
            Process::Write32((HUDPointerCheck + mainHUDoffset), 0x7F000004);
            Process::Write32((HUDPointerCheck + ActionButtonBGOffset), 0x00000010);
            Process::Write32((HUDPointerCheck + ActionButtonTextOffset), 0x00FFFF00);
        }

        Process::Write8(AddressList::OffScreenLocation.addr, disableOffScrArrow);
    }

    // does this reset itself? 
    void Rendering::disableFog(MenuEntry* entry) 
    {
        // eventually it would be nice to have proper labels for these addresses + values
        // once enough info is known about these addresses
         
        Process::Write32((AddressList::FogA.addr), 0xFF700FFF);
        Process::Write32((AddressList::FogB.addr), 0x453B8000);
        Process::Write32((AddressList::FogC.addr), 0x459C4000);
    }
    
    void Rendering::disableScrollingText(MenuEntry* entry) 
    {
        isScrollTextDisabled = !isScrollTextDisabled;

        if (isScrollTextDisabled) 
        {
            Process::Write32(AddressList::ScrollingTextOpacity.addr, 0x00000000);
            entry->SetName("Enable top screen scrolling text");
        }
        else 
        {
            Process::Write32(AddressList::ScrollingTextOpacity.addr, 0x3F800000);
            entry->SetName("Disable top screen scrolling text");
        }
    }

    void Rendering::respawnIndicator(MenuEntry* entry) 
    {
        // TODO: add checks? 
        float respawnCoords[3][3] = {
            {0.0, 0.0, 0.0}, // green
            {0.0, 0.0, 0.0}, // blue
            {0.0, 0.0, 0.0}  // red
        };

        u32 arrowAddresses[3] = {
            AddressList::ArrowIndicatorColorTop.addr,
            AddressList::ArrowIndicatorColorMid.addr,
            AddressList::ArrowIndicatorColorBot.addr
        };

        // grab respawn coords
        for (int iterateColor = 0; iterateColor < 3; iterateColor++) 
        {
            u32 offset = iterateColor * GameData::playerAddressOffset;
            Process::ReadFloat((AddressList::RespawnPositionX.addr + offset), respawnCoords[iterateColor][0]);
            Process::ReadFloat((AddressList::RespawnPositionY.addr + offset), respawnCoords[iterateColor][1]);
            Process::ReadFloat((AddressList::RespawnPositionZ.addr + offset), respawnCoords[iterateColor][2]);
        }

        u32 forceVisibility = 0x10FFFF00;
        u8 visibilityOffset = 0x4, indivCoordinateOffset = 0x4;
        u8 coordinateOffsetStart = 0x3C;

        for (int iterateEdits = 0; iterateEdits < 3; iterateEdits++) 
        {
            Process::Write8(arrowAddresses[iterateEdits], GameData::generalPlayerIDs[iterateEdits]); // set arrow color
            Process::Write32(arrowAddresses[iterateEdits] + visibilityOffset, forceVisibility);      // force arrow visibility
        }

        for (int iterateArray = 0; iterateArray < 3; iterateArray++) {
            Process::WriteFloat((arrowAddresses[0] + (iterateArray * 0x4) - coordinateOffsetStart), respawnCoords[0][iterateArray]);
            Process::WriteFloat((arrowAddresses[1] + (iterateArray * 0x4) - coordinateOffsetStart), respawnCoords[1][iterateArray]);
            Process::WriteFloat((arrowAddresses[2] + (iterateArray * 0x4) - coordinateOffsetStart), respawnCoords[2][iterateArray]);
        }
    }

    void Rendering::seeChestContents(MenuEntry* entry) 
    {
        showChestContents = !showChestContents;

        if (showChestContents) 
        {
            Process::Patch(AddressList::SeeChestContents.addr, 0xEA000017);
            entry->SetName("Disable forced-visibility of Treasure Chest contents");
        } 
        else 
        {
            Process::Patch(AddressList::SeeChestContents.addr, 0x0A000017);
            entry->SetName("Force visibility of Treasure Chest contents");
        }
    }

    void Rendering::swapLinkTexture(MenuEntry* entry)
    {
        int Link, Tex;
        std::string linkName;
        static const StringVector linkList =
        {
            "Hytopia Link",
            "Green Link",
            "Blue Link",
            "Red Link"
        };

        Keyboard ChooseLink("Choose which Link's texture to edit:");
        ChooseLink.Populate(linkList);
        switch(ChooseLink.Open()){
            case 0:
                Link = 0; linkName = "Hytopia";
                break;
            case 1:
                Link = 1; linkName = "Green";
                break;
            case 2:
                Link = 2; linkName = "Blue";
                break;
            case 3:
                Link = 3; linkName = "Red";
                break;
            default:
                return;
        }
        
        Keyboard Texture("Choose a texture to load for "+linkName+" Link:");
        Texture.Populate(linkList);
        Tex = Texture.Open();
        if (Tex >= 0){
            u32 targetaddr = AddressList::TextureName.addr + Link*0x7;
            u8 towrite = 0x30 + Tex;
            Process::Write8(targetaddr, towrite);
        }
    }

    void Rendering::swapSPLoadingScreen(MenuEntry* entry)
    {
        Keyboard lsType("Choose an appearance for the single player\nno-challenge loading screen:");
        static const StringVector lsTypeList = 
        {
            "Default",
            "Multiplayer",
            "Coliseum V1",
            "Coliseum V2"
        };
        lsType.Populate(lsTypeList);

        switch(lsType.Open()){
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
                // Red and Blue panels(
                Process::WriteFloat(AddressList::LoadingScreenSPNoChal.addr, 6);
                Process::WriteFloat(AddressList::LoadingScreenSPIcons.addr, 6);
                break;
        }
    }

    void Rendering::editLiveMsgColor(MenuEntry* entry)
    {
        u32 targetaddr;
        switch (GeneralHelpers::chooseLink()){
            case 0:
                targetaddr = AddressList::LiveNameColorG.addr;
                break;
            case 1:
                targetaddr = AddressList::LiveNameColorB.addr;
                break;
            case 2:
                targetaddr = AddressList::LiveNameColorR.addr;
                break;
            default:
                return;
        }

        Keyboard HexColor("Enter a 6-digit RGB hex code:");
        HexColor.IsHexadecimal(true);
        u32 result, newColor;
        if (HexColor.Open(result) == 0){
            u8 r = result >> 16;
            u8 g = result >> 8;
            u8 b = result;
            newColor = r + (g << 8) + (b << 16) + (0xFF << 24);
            Process::Write32(targetaddr, newColor);
        }
    }

    void Rendering::forcePomPom(MenuEntry* entry)
    {
        if (entry->Name() == "Force visibility of Cheer Outfit pom poms"){
            Process::Patch(AddressList::CheerPomPom.addr, 0x1A000009);
            entry->SetName("Hide Cheer Outfit pom poms outside Couture's");
        } else {
            Process::Patch(AddressList::CheerPomPom.addr, 0x0A000009);
            entry->SetName("Force visibility of Cheer Outfit pom poms");
        }
    }

    void Rendering::forceAura(MenuEntry* entry)
    {
        // Assumption: Every region's version of this function is structured identically
        // and changes to branch target addresses are consistent
        Keyboard AuraType("Select a costume aura, or reset to restore\ncostume checks.");
        static const StringVector Auras =
        {
            "Reset",
            "Bear Maximum",
            "Showstopper",
            "Fierce Deity Armor"
        };
        AuraType.Populate(Auras);
        switch(AuraType.Open()){
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

    void Rendering::forceBlueSwordParticles(MenuEntry* entry)
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

    void Rendering::disablePlayerLight(MenuEntry* entry)
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
}