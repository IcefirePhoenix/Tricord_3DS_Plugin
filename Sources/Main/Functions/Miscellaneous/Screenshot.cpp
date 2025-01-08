#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    FloatingButton photoBtn(IntRect(120, 50, 32, 32), Icon::DrawUnsplash);

    MenuEntry *autoWriteCameraStatus;
    MenuEntry *autoDisableCamShutter;

    u8 shutterNotVisible = 0x0, shutterVisible = 0x1;
    bool cameraToggle, showPhoto;

    /* ------------------ */

    // Implementation for custom touchscreen button to display current Drablands screenshot
    void Miscellaneous::managePhotoDisp(MenuEntry *entry)
    {
        u8 doesPhotoExist, clearPhotoDisplay = 0x0;

        // init actions...
        Process::Read8(AddressList::CheckPhotoExist.addr, doesPhotoExist);
        showPhotoBtnIntroMsg(entry->WasJustActivated());

        // only draw the floating button during level-based gameplay...
        if (Level::isInDrablands() && !GeneralHelpers::isLoadingScreen())
        {
            photoBtn.Draw();

            if (!GeneralHelpers::isPauseScreen())
                photoBtn.Update(Touch::IsDown(), IntVector(Touch::GetPosition()));
        }

        // onPress behavior for floating button...
        if (photoBtn())
        {
            if (!showPhoto)
            {
                if (doesPhotoExist)
                {
                    OSD::Notify("[DISPLAY PHOTO TOGGLE]: Currently viewing stored photo.");
                    showPhoto = !showPhoto;
                }
                else
                    OSD::Notify("[DISPLAY PHOTO TOGGLE]: No photo currently stored.");
            }
            else
                showPhoto = !showPhoto;
        }

        // attempt to display the photo if it exists...
        if (showPhoto)
            displayPhoto(doesPhotoExist);
        else
        {
            Process::Write8(AddressList::DisplayTopPhoto.addr, clearPhotoDisplay);
            GeneralHelpers::managePlayerLock(false);
        }
    }

    // Displays intro message
    void showPhotoBtnIntroMsg(bool showMsg)
    {
        if (showMsg)
        {
            OSD::Notify("[DISPLAY PHOTO TOGGLE]: In the Drablands, drag the camera");
            OSD::Notify("button on the touchscreen to change its location.");
        }
    }

    // Helper function to force-display the Drablands screenshot
    void displayPhoto(bool photoCheck)
    {
        u8 forceDisplayPhoto = 0xFF;
        if (photoCheck)
        {
            Process::Write8(AddressList::DisplayTopPhoto.addr, forceDisplayPhoto);
            hideHUD();

            // top screen is occupied by photo so real-time gameplay can't be seen; prevent player from moving
            GeneralHelpers::managePlayerLock(true);
        }
    }

    // Force-changes the screenshot behavior of the X button, regardless of whether the camera is in player inventory
    void Miscellaneous::toggleCameraButton(MenuEntry *entry)
    {
        StringVector camOpts =
        {
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
                cameraToggle = false;
                autoWriteCameraStatus->Enable();
                entry->SetName("Toggle camera on X button: Disabled");
                break;
            case 1:
                cameraToggle = true;
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

    // Helper funcion to write X button edits to memory
    void Miscellaneous::writeCameraEdits(MenuEntry *entry)
    {
        if (Level::isInDrablands)
            Process::Write8(AddressList::CameraXButtonToggle.addr, cameraToggle);
    }

    // Force-toggles the top-screen camera shutter animation
    void Miscellaneous::toggleCameraShutter(MenuEntry *entry)
    {
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

    // Helper function for writing the shutter anim edits to memory
    // TODO: reset impl separate
    void Miscellaneous::writeShutterDisable(MenuEntry *entry)
    {
        if (Level::isInDrablands())
            Process::Write8(AddressList::CameraShutter.addr, shutterNotVisible);

        // reset during very last execution after entry is disabled
        if (!entry->IsActivated())
            Process::Write32(AddressList::CameraShutter.addr, shutterVisible);
    }
}