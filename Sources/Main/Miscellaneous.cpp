#include "Helpers.hpp"
#include "Main/Miscellaneous.hpp"

#include "CTRPluginFramework/Menu/MenuEntryHotkeys.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Icon.hpp"
#include "CTRPluginFramework/System/Touch.hpp"
#include "CTRPluginFrameworkImpl/Graphics/FloatingButton.hpp"
#include "CTRPluginFrameworkImpl/Menu/HotkeysModifier.hpp"

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework
{
    MenuEntry* instantTextDisplay;
    MenuEntry* autoWriteCameraStatus;
    MenuEntry* autoDisableCamShutter;

    bool _cameraToggle;
    bool showPhoto, useInstantText;
   
    u32 keys;

    void Miscellaneous::buttonSpammer(MenuEntry* entry) 
    {
        // currently a mess; ignore for now

        // intnButtons = entry->Hotkeys.Count();
        // OSD::Notify(std::to_string(chosenButtons));
        // Controller::InjectKey(Controller::GetKeysDown(true));
    }

    void Miscellaneous::manageInstantText(MenuEntry* entry) 
    {
        useInstantText = !useInstantText;

        // not using absolute max of 0xFF to avoid graphical bugs... 0x2D is fast enough
        u8 textSpeed = useInstantText ? 0x2D : 0x1;
        std::string entryName = useInstantText ? "Disable instant text boxes" : "Force instant text boxes";
        
        Process::Write8(AddressList::TextBoxSpeed.addr, textSpeed);
        entry->SetName(entryName);
    }
    
    FloatingButton photoBtn(IntRect(120, 50, 32, 32), Icon::DrawUnsplash);
    void Miscellaneous::managePhotoDisp(MenuEntry* entry) 
    {
        u8 doesPhotoExist;
        Process::Read8(AddressList::CheckPhotoExist.addr, doesPhotoExist);

        showPhotoBtnIntroMsg(entry->WasJustActivated());
        
        if (Level::isInDrablands() && !GeneralHelpers::isLoadingScreen())
        {
            photoBtn.Draw();

            if (!GeneralHelpers::isPauseScreen())
                photoBtn.Update(Touch::IsDown(), IntVector(Touch::GetPosition()));
        }

        if (photoBtn()) 
        {
            if (!showPhoto) 
            {
                if (doesPhotoExist) 
                {
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
        else 
        {
            Process::Write8(AddressList::DisplayTopPhoto.addr, 0x0);
            GeneralHelpers::managePlayerLock(false);
        }
    }
    
    void showPhotoBtnIntroMsg(bool showMsg) 
    {
        if (showMsg) 
        {
            OSD::Notify("[DISPLAY PHOTO TOGGLE]: In the Drablands, drag the camera");
            OSD::Notify("button on the touchscreen to change its location.");
        }
    }

    void displayPhoto(bool photoCheck) 
    {
        if (photoCheck) 
        {
            Process::Write8(AddressList::DisplayTopPhoto.addr, 0xFF);
            
            GeneralHelpers::managePlayerLock(true);
            hideHUD();
        }
    }


    void Miscellaneous::setLobbyBallCounter (MenuEntry* entry)
    {
        // checkbox entry
        // check location
        // verify data pointer
        // open keyboard
        // write edits
    }

    void Miscellaneous::toggleCameraButton(MenuEntry* entry)
    {
        StringVector camOpts = {
            "Disable camera on X",
            "Enable camera on X",
            "Reset changes"
        };

        Keyboard selCamOpt("Select X button's camera function:");
        selCamOpt.Populate(camOpts);

        int choice = selCamOpt.Open();

        switch (choice)
        {
        case 0:
            setCameraEdits(false);
            autoWriteCameraStatus->Enable();
            entry->SetName("Toggle camera on X button: Disabled");
            break;
        case 1:
            setCameraEdits(true);
            autoWriteCameraStatus->Enable();
            entry->SetName("Toggle camera on X button: Enabled");
            break;
        case 2:
            autoWriteCameraStatus->Disable();
            entry->SetName("Toggle camera on X button: No edits");
            break;
        default:
            break;
        }
    }

    void setCameraEdits(bool useCamera)
    {
        _cameraToggle = useCamera;
    }

    bool getCameraStatus(void)
    {
        return _cameraToggle;
    }

    void Miscellaneous::writeCameraEdits(MenuEntry* entry)
    {
        if (Level::isInDrablands)
            Process::Write8(AddressList::CameraOnX.addr, getCameraStatus());
    }

    void Miscellaneous::toggleCameraShutter(MenuEntry* entry)
    {
        u32 shutterVisible = 0x1;

        if (entry->Name() == "Disable camera shutter") 
        {
            autoDisableCamShutter->Enable();
            entry->SetName("Enable camera shutter");
        }
        else 
        {
            autoDisableCamShutter->Disable();
            entry->SetName("Disable camera shutter");
            Process::Write32(AddressList::CameraShutter.addr, shutterVisible);
        }
    }

    void Miscellaneous::writeShutterDisable(MenuEntry* entry)
    {
        u32 shutterNotVisible = 0x0;
        u32 shutterVisible = 0x1;

        if (Level::isInDrablands())
            Process::Write8(AddressList::CameraShutter.addr, shutterNotVisible);
        
        // reset during very last execution after entry is disabled
        if (!entry->IsActivated())
            Process::Write32(AddressList::CameraShutter.addr, shutterVisible);
    }

}