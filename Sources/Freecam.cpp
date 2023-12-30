#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "CTRPluginFramework/Menu/MenuEntryHotkeys.hpp"

#include "3ds.h"

namespace CTRPluginFramework
{
	float cameraXcoord;
	float cameraYcoord;
	float cameraZcoord;
	float cameraXrotation;
	float cameraZrotation;
	float cameraZoom;

	// TODO: cam-anim freeze bypass in boss stages (remove support?)
	enum cameraMode { GAMEPLAY, DYNAMIC, CUTSCENE, CAM_ANIMATION}; // is this used anywhere?
	cameraMode currentCamera;

	// TODO: determine (recommended) min/max values
	float shiftSensitivity = 0.2;
	float heightSensitivity = 0.6;
	float zoomSensitivity = 0.05;
	float rotationSensitivity = 0.5;

	u32 cameraXrawValue;
	u32 cameraZrawValue;
	
	bool isFreecamInUse;
	bool isCameraLocked;

	std::string cameraLockStatus = "";

	// TODO: player movement compensation

	void useFreecam(MenuEntry* entry) {
		// enable/disable freecam
		if (entry->Hotkeys[0].IsPressed()) {
			isFreecamInUse = !isFreecamInUse;

			if (isFreecamInUse) {
				Process::Write8(AddressList::CameraMode.addr, 0x02);
				OSD::Notify("[FREECAM] Freecam in-use.");
			}
			else {
				Process::Write8(AddressList::CameraMode.addr, 0x00);
				OSD::Notify("[FREECAM] Freecam disabled.");
			}
		}

		managePlayerLock();

		// toggle camera lock
		if (entry->Hotkeys[1].IsPressed()) {
			// swap status
			isCameraLocked = !isCameraLocked;

			if (isCameraLocked)
				cameraLockStatus = "locked.";
			else
				cameraLockStatus = "unlocked.";

			OSD::Notify("[FREECAM] Camera position has been manually" + cameraLockStatus);
		}
		
		// lock camera impl
		lockCamera();

		// reset camera
		if (entry->Hotkeys[2].IsPressed()) {
			Process::Write8(AddressList::CameraMode.addr, 0x00); // use GAMEPLAY cam to re-orient camera back to player 
			
			// reset rotation as this doesn't happen automatically
			Process::Write32(AddressList::CameraRotationX.addr, 0x271C71C6); 
			Process::Write32(AddressList::CameraRotationX.addr, 0x00000000);
			
			if (isFreecamInUse)
				Process::Write8(AddressList::CameraMode.addr, 0x02); // if freecam is activated, use CUTSCENE cam

			OSD::Notify("[FREECAM] Camera position has been reset.");
		}

		// basic camera controls
		if (isFreecamInUse == true) {
			if (entry->Hotkeys[3].IsDown())
				shiftCamSouth();

			if (entry->Hotkeys[4].IsDown())
				shiftCamNorth();

			if (entry->Hotkeys[5].IsDown())
				shiftCamEast();

			if (entry->Hotkeys[6].IsDown())
				shiftCamWest();

			if (entry->Hotkeys[7].IsDown())
				zoomCamIn();

			if (entry->Hotkeys[8].IsDown())
				zoomCamOut();

			if (entry->Hotkeys[9].IsDown())
				raiseCam();

			if (entry->Hotkeys[10].IsDown())
				lowerCam();

			if (entry->Hotkeys[11].IsDown())
				rotateCamXCounter();

			if (entry->Hotkeys[12].IsDown())
				rotateCamXClockwise();

			if (entry->Hotkeys[13].IsDown())
				rotateCamZCounter();

			if (entry->Hotkeys[14].IsDown())
				rotateCamZClockwise();
		}
	}

	void shiftCamNorth(void) {
		Process::ReadFloat(AddressList::CameraPosZ.addr, cameraZcoord);
		Process::WriteFloat(AddressList::CameraPosZ.addr, (cameraZcoord + shiftSensitivity));
	}

	void shiftCamSouth(void) {
		Process::ReadFloat(AddressList::CameraPosZ.addr, cameraZcoord);
		Process::WriteFloat(AddressList::CameraPosZ.addr, (cameraZcoord - shiftSensitivity));
	}

	void shiftCamEast(void) {
		Process::ReadFloat(AddressList::CameraPosX.addr, cameraXcoord);
		Process::WriteFloat(AddressList::CameraPosX.addr, (cameraXcoord + shiftSensitivity));
	}

	void shiftCamWest(void) {
		Process::ReadFloat(AddressList::CameraPosX.addr, cameraXcoord);
		Process::WriteFloat(AddressList::CameraPosX.addr, (cameraXcoord - shiftSensitivity));
	}

	// experiment with 32C zoom and master zoom
	// orthographic(?)
	void zoomCamIn(void) {
		Process::ReadFloat(AddressList::MasterZoom.addr, cameraZoom);
		Process::WriteFloat(AddressList::MasterZoom.addr, (cameraZoom * (1 + zoomSensitivity)));
	}

	void zoomCamOut(void) {
		Process::ReadFloat(AddressList::MasterZoom.addr, cameraZoom);
		Process::WriteFloat(AddressList::MasterZoom.addr, (cameraZoom * (1 - zoomSensitivity)));
	}

	void raiseCam(void) {
		Process::ReadFloat(AddressList::CameraPosY.addr, cameraYcoord);
		Process::WriteFloat(AddressList::CameraPosY.addr, (cameraYcoord + heightSensitivity));
	}

	void lowerCam(void) {
		Process::ReadFloat(AddressList::CameraPosY.addr, cameraYcoord);
		Process::WriteFloat(AddressList::CameraPosY.addr, (cameraYcoord - heightSensitivity));
	}
	void rotateCamXCounter(void) {
		Process::ReadFloat(AddressList::CameraRotationX.addr, cameraXrotation);
		if ((cameraXrotation * rotationSensitivity) != 0.0)
			Process::WriteFloat(AddressList::CameraRotationX.addr, (cameraXrotation * rotationSensitivity));
	
		// if value becomes invalid, fix it after
		Process::Read32(AddressList::CameraRotationX.addr, cameraXrawValue);
		if (cameraXrawValue == 0x00000000)
			Process::Write32(AddressList::CameraRotationX.addr, 0x00000001);
	}

	void rotateCamXClockwise(void) {
		Process::ReadFloat(AddressList::CameraRotationX.addr, cameraXrotation);
		Process::WriteFloat(AddressList::CameraRotationX.addr, (cameraXrotation / rotationSensitivity));
	
		// if value becomes invalid, fix it after
		Process::Read32(AddressList::CameraRotationX.addr, cameraXrawValue);
		if (cameraXrawValue == 0x7F800000)
			Process::Write32(AddressList::CameraRotationX.addr, 0x7F7F0000);
	}

	// TODO: finish z camera
	void rotateCamZCounter(void) {
		Process::ReadFloat(AddressList::CameraRotationZ.addr, cameraZrotation);
		Process::WriteFloat(AddressList::CameraRotationZ.addr, (cameraZrotation * rotationSensitivity));

		// if value becomes invalid, fix it after
		Process::Read32(AddressList::CameraRotationZ.addr, cameraZrawValue);
		if (cameraZrawValue == 0x00000000) 
			Process::Write32(AddressList::CameraRotationZ.addr, 0xFE7F0000);

		if (cameraZrawValue == 0x000081FF || cameraZrawValue == 0x00008180)
			Process::Write32(AddressList::CameraRotationZ.addr, 0x00007E80);
	
	}

	void rotateCamZClockwise(void) {
		Process::ReadFloat(AddressList::CameraRotationZ.addr, cameraZrotation);
		Process::WriteFloat(AddressList::CameraRotationZ.addr, (cameraZrotation / rotationSensitivity));

		// if value becomes invalid, fix it after
		Process::Read32(AddressList::CameraRotationZ.addr, cameraZrawValue);
		if (cameraZrawValue == 0x00000000 || cameraZrawValue == 0xFF800000) { 
			Process::Write32(AddressList::CameraRotationZ.addr, 0x00800000);
		}

		if (cameraZrawValue == 0x7F800000 || cameraZrawValue == 0xFF7F0000 || cameraZrawValue == 0x80000000)
			Process::Write32(AddressList::CameraRotationZ.addr, 0x80800000);
	}

	// TODO: edit Hotkey Manager to use C-pad support
	void editHotkeys(MenuEntry* entry)
	{
		// placeholders 
		Keyboard kbd("dummy text");
		std::string title;
		StringVector opts;

		// this menu stays open regardless of input UNLESS the user specifies they wish to exit
		bool loop = true;
		kbd.CanAbort(false);

		while (loop) {
			// update top screen info
			title = "Choose a Freecam hotkey to edit\n"; 
			title.append(menuFreecam->Hotkeys[0].ToString(true) + "\n");
			title.append(menuFreecam->Hotkeys[1].ToString(true) + "       " + menuFreecam->Hotkeys[2].ToString(true) + "\n");
			title.append(menuFreecam->Hotkeys[3].ToString(true) + "       " + menuFreecam->Hotkeys[4].ToString(true) + "\n");
			title.append(menuFreecam->Hotkeys[5].ToString(true) + "       " + menuFreecam->Hotkeys[6].ToString(true) + "\n");
			title.append(menuFreecam->Hotkeys[7].ToString(true) + "       " + menuFreecam->Hotkeys[8].ToString(true) + "\n");
			title.append(menuFreecam->Hotkeys[9].ToString(true) + "       " + menuFreecam->Hotkeys[10].ToString(true) + "\n");
			title.append(menuFreecam->Hotkeys[11].ToString(true) + "\n");
			title.append(menuFreecam->Hotkeys[12].ToString(true) + "\n");
			title.append(menuFreecam->Hotkeys[13].ToString(true) + "\n");			
			title.append(menuFreecam->Hotkeys[14].ToString(true));

			// update bottom screen info
			opts.clear();
			opts.push_back(std::string("Save and exit"));
			opts.push_back(std::string("Freecam toggle"));
			opts.push_back(std::string("Camera lock toggle"));
			opts.push_back(std::string("Reset camera toggle"));
			opts.push_back(std::string("Shift camera north"));
			opts.push_back(std::string("Shift camera south"));
			opts.push_back(std::string("Shift camera east"));
			opts.push_back(std::string("Shift camera west"));
			opts.push_back(std::string("Zoom camera in"));
			opts.push_back(std::string("Zoom camera out"));
			opts.push_back(std::string("Raise camera"));
			opts.push_back(std::string("Lower camera"));
			opts.push_back(std::string("Rotate counterclockwise (X-axis)"));
			opts.push_back(std::string("Rotate clockwise (X-axis)"));
			opts.push_back(std::string("Rotate counterclockwise (Z-axis)"));
			opts.push_back(std::string("Rotate clockwise (Z-axis)"));

			// display top screen info
			kbd.GetMessage() = title;

			// populate bottom screen options
			kbd.Populate(opts);

			// begin watching for changes
			int chose = kbd.Open();
			if (chose == 0) {
				// end loop = exit the menu
				loop = false;
				break;
			} 
			else {
				// set new hotkey
				menuFreecam->Hotkeys[chose - 1].AskForKeys();
				break;	
			}
		}
	}

	void lockCamera(void) {
		if (isFreecamInUse == true || isCameraLocked == true) {
			Process::Patch(AddressList::DynamicCameraCheck.addr, 0xE3500000);
			Process::Patch(AddressList::GameplayCameraCheck.addr, 0xE3500002);
			Process::Patch(AddressList::GameplayCameraInit.addr, 0x1A000002);
			Process::Patch(AddressList::RetGameplayCameraInit.addr, 0xEA000008);

		}
		else {
			Process::Patch(AddressList::DynamicCameraCheck.addr, 0xE3500001);
			Process::Patch(AddressList::GameplayCameraCheck.addr, 0xE3500000);
			Process::Patch(AddressList::GameplayCameraInit.addr, 0x0A000002);
			Process::Patch(AddressList::RetGameplayCameraInit.addr, 0x0A000008);
		}
	}

	void managePlayerLock(void) {
		if (isFreecamInUse)
			Process::Write8(AddressList::LockMovement.addr, 0x10);
		else
			Process::Write8(AddressList::LockMovement.addr, 0x00);
	}

	void editSensitivity(MenuEntry* entry)
	{
		// placeholders 
		Keyboard kbd("dummy text");
		std::string title;
		StringVector opts;

		// this menu stays open regardless of input UNLESS the user specifies they wish to exit
		bool loop = true;
		kbd.CanAbort(false);

		while (loop) {
			// update top screen info
			title = "Freecam sensitivity levels:\n\n";
			title.append("Shift sensitivity: " + std::to_string(shiftSensitivity));
			title.append("\nHeight sensitivity: " + std::to_string(heightSensitivity));
			title.append("\nZoom sensitivity: " + std::to_string(zoomSensitivity));
			title.append("\nRotation sensitivity: " + std::to_string(rotationSensitivity));

			// update bottom screen info
			opts.clear();
			opts.push_back(std::string("Set shift sensitivity"));
			opts.push_back(std::string("Set height sensitivity"));
			opts.push_back(std::string("Set zoom sensitivity"));
			opts.push_back(std::string("Set rotation sensitivity"));
			opts.push_back(std::string("Save and exit"));

			// display top screen info
			kbd.GetMessage() = title;

			// populate bottom screen options
			kbd.Populate(opts);

			// begin watching for changes
			switch (kbd.Open()) {
				case 0:
				{
					shiftSensitivity = setSensitivity("Shift Sensitivity Value\n\nRecommended values: []");
					break;
				}
				case 1:
				{
					heightSensitivity = setSensitivity("Height Sensitivity Value\n\nRecommended values : [] ");
					break;
				}
				case 2:
				{
					zoomSensitivity = setSensitivity("Zoom Sensitivity Value\n\nRecommended values: []");
					break;
				}
				case 3:
				{
					rotationSensitivity = setSensitivity("Rotation Sensitivity Value\n\nRecommended values: []");
					break;
				}
				default:
				{
					// end loop = exit the menu
					loop = false;
					break;
				}
			}
		}
	}

	float setSensitivity(std::string message) {
		float result;

		Keyboard sizeKB(message);
		sizeKB.IsHexadecimal(false);
		sizeKB.Open(result);

		if (result < 0.0)
			MessageBox(Color::Gainsboro << "Error", "Sensitivity cannot be negative.")();

		return result;
	}
}

