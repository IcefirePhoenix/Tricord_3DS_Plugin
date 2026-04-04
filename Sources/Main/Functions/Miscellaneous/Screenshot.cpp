#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    Button photoBtn(Button::Icon | Button::Toggle, IntRect(120, 47, 20, 20), Icon::DrawTFHCamera);
    MenuEntry *autoDisableCamShutter;

    u8 shutterNotVisible = 0x0, shutterVisible = 0x1;
    bool isCamDisabled, showPhoto;

    /* ------------------ */

    // Implementation for custom touchscreen button to display current Drablands screenshot
    void Miscellaneous::managePhotoDisp(MenuEntry *entry)
    {
        u8 doesPhotoExist, clearPhotoDisplay = 0x0;

        // init actions...
        Process::Read8(AddressList::getAddress("CheckPhotoExist"), doesPhotoExist);

        // only draw the floating button during level-based gameplay...
        if (Level::isInDrablands() && !GeneralHelpers::isLoadingScreen(false))
        {
            OSDImpl::AddToPriorityDrawQueue(photoBtn);

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
            Process::Write8(AddressList::getAddress("DisplayTopPhoto"), clearPhotoDisplay);
            GeneralHelpers::managePlayerLock(false);
        }
    }

    // Helper function to force-display the Drablands screenshot
    void displayPhoto(bool photoCheck)
    {
        u8 forceDisplayPhoto = 0xFF;
        if (photoCheck)
        {
            Process::Write8(AddressList::getAddress("DisplayTopPhoto"), forceDisplayPhoto);
            Rendering::toggleHUD(true);

            // top screen is occupied by photo so real-time gameplay can't be seen; prevent player from moving
            GeneralHelpers::managePlayerLock(true);
        }
    }

    // Force-changes the screenshot behavior of the X button, regardless of whether the camera is in player inventory
    void Miscellaneous::toggleCameraButton(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
        {
            isCamDisabled = true;
            writeCameraEdits(isCamDisabled);
        }
        else if (!entry->IsActivated())
        {
            isCamDisabled = false;
            writeCameraEdits(isCamDisabled);
        }
    }

    // Helper function to write X button edits to memory
    void Miscellaneous::writeCameraEdits(bool isCamDisabled)
    {
        u32 patchValue = isCamDisabled ? 0xEA000028 : 0x0A000028;
        Process::Write32(AddressList::getAddress("CameraXButtonDisable"), patchValue);
    }

    // For external functions that need to check X button toggle status
    bool Miscellaneous::getCameraStatus(void)
    {
        return isCamDisabled;
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
            Process::Write32(AddressList::getAddress("CameraShutter"), shutterVisible);
        }
    }

    // Helper function for writing the shutter anim edits to memory
    // TODO: reset impl separate
    void Miscellaneous::writeShutterDisable(MenuEntry *entry)
    {
        if (Level::isInDrablands())
            Process::Write8(AddressList::getAddress("CameraShutter"), shutterNotVisible);

        // reset during very last execution after entry is disabled
        if (!entry->IsActivated())
            Process::Write32(AddressList::getAddress("CameraShutter"), shutterVisible);
    }
}