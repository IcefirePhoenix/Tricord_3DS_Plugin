#include "cheats.hpp"
#include "AddressList.hpp"

#include "Helpers/Address.hpp"
#include "Helpers/Level.hpp"
#include "Helpers/GeneralHelpers.hpp"

#include "CTRPluginFramework/Menu/MenuEntryHotkeys.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Icon.hpp"
#include "CTRPluginFramework/System/Touch.hpp"
#include "CTRPluginFrameworkImpl/Graphics/FloatingButton.hpp"
#include "CTRPluginFrameworkImpl/Menu/HotkeysModifier.hpp"

#include "3ds.h"

namespace CTRPluginFramework
{
    bool showPhoto, showChestContents, useInstantText, useBeamCooldown = false;
   
    bool beamStatuses[3] = { false, false, false };
    u32 keys;

    void buttonSpammer(MenuEntry* entry) {
        // currently a mess; ignore for now

        // intnButtons = entry->Hotkeys.Count();
        // OSD::Notify(std::to_string(chosenButtons));
        // Controller::InjectKey(Controller::GetKeysDown(true));
    }

    void instantText(MenuEntry* entry) {
        useInstantText = !useInstantText;

        // not using absolute max of 0xFF to avoid graphical bugs... 0x2D is fast enough
        u8 textSpeed = useInstantText ? 0x2D : 0x1;
        std::string entryName = useInstantText ? "Disable instant text boxes" : "Force instant text boxes";
        
        Process::Write8(AddressList::TextBoxSpeed.addr, textSpeed);
        entry->SetName(entryName);
    }
    
    FloatingButton photoBtn(IntRect(120, 50, 32, 32), Icon::DrawUnsplash);
    void managePhotoDisp(MenuEntry* entry) {
        u8 doesPhotoExist;
        Process::Read8(AddressList::CheckPhotoExist.addr, doesPhotoExist);

        showPhotoBtnIntroMsg(entry->WasJustActivated());
        
        if (Level::isInDrablands() && !GeneralHelpers::isLoadingScreen()){
            photoBtn.Draw();
            if (!GeneralHelpers::isPauseScreen())
                photoBtn.Update(Touch::IsDown(), IntVector(Touch::GetPosition()));
        }

        if (photoBtn()) {
            if (!showPhoto) {
                if (doesPhotoExist) {
                    OSD::Notify("[DISPLAY PHOTO TOGGLE]: Currently viewing stored photo.");
                    showPhoto = !showPhoto;
                }
                else OSD::Notify("[DISPLAY PHOTO TOGGLE]: No photo currently stored.");
            } 
            else
                showPhoto = !showPhoto;
        }

        if (showPhoto) 
            displayPhoto(doesPhotoExist);
        else {
            Process::Write8(AddressList::DisplayTopPhoto.addr, 0x0);
            GeneralHelpers::managePlayerLock(false);
        }
    }
    
    void showPhotoBtnIntroMsg(bool showMsg) {
        if (showMsg) {
            OSD::Notify("[DISPLAY PHOTO TOGGLE]: In the Drablands, drag the camera");
            OSD::Notify("button on the touchscreen to change its location.");
        }
    }

    void displayPhoto(bool photoCheck) {
        if (photoCheck) {
            Process::Write8(AddressList::DisplayTopPhoto.addr, 0xFF);
            
            GeneralHelpers::managePlayerLock(true);
            hideHUD();
        }
    }

    void selectLinkBeam(MenuEntry* entry) {
        std::string enSlid = Color::LimeGreen << "\u2282\u25CF";
        std::string disSlid = Color::Red << "\u25CF\u2283";
        std::string title;

        StringVector bottomScreenOptions;

        Keyboard kbd("Menu");
        kbd.CanAbort(false);

        bool loop = true;
        while (loop) {
            title = "Use the toggles to disable the Sword Beam cooldown period:\n\n";

            bottomScreenOptions.clear();
            bottomScreenOptions.push_back(std::string("Player 1 ") << (beamStatuses[0] ? enSlid : disSlid));
            bottomScreenOptions.push_back(std::string("Player 2 ") << (beamStatuses[1] ? enSlid : disSlid));
            bottomScreenOptions.push_back(std::string("Player 3 ") << (beamStatuses[2] ? enSlid : disSlid));
            bottomScreenOptions.push_back("Save changes");
            bottomScreenOptions.push_back("Disable entry");

            kbd.GetMessage() = title;
            kbd.Populate(bottomScreenOptions);

            switch (kbd.Open()) {
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

    void setBeamCooldown(MenuEntry* entry) {
        u32 playerOffset = 0x10000;

        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; ++iterateThruPlayers) {
            if (beamStatuses[iterateThruPlayers])
                Process::Write8(AddressList::SwordBeamCD.addr + (playerOffset * iterateThruPlayers), 0x1E);
        }  
    }

    void seeChestContents(MenuEntry* entry) {
        showChestContents = !showChestContents;

        if (showChestContents) {
            Process::Patch(AddressList::SeeChestContents.addr, 0xEA000017);
            entry->SetName("Disable forced-visibility of Treasure Chest contents");
        } 
        else {
            Process::Patch(AddressList::SeeChestContents.addr, 0x0A000017);
            entry->SetName("Force visibility of Treasure Chest contents");
        }
    }
}