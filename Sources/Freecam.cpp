#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "CTRPluginFramework/Menu/MenuEntryHotkeys.hpp"

#include "3ds.h"

namespace CTRPluginFramework
{
	// TODO: cam-anim freeze bypass in boss stages (remove support?)
	// TODO: player movement compensation

	float cameraXcoord;
	float cameraYcoord;
	float cameraZcoord;
	float cameraXrotation;
	float cameraZrotation;
	float cameraZoom;

	enum cameraMode { GAMEPLAY, DYNAMIC, CUTSCENE, CAM_ANIMATION };

	float shiftSensitivity = 0.2;
	float heightSensitivity = 0.6;
	float zoomSensitivity = 0.03;
	float rotationSensitivity = 0.5;

	u32 cameraXrawValue;
	u32 cameraZrawValue;

	bool isFreecamInUse;
	bool isCameraLocked;
	bool usePerspectiveZoom = true;

	std::string cameraLockStatus = "";

	void useFreecam(MenuEntry* entry) {
		// enable/disable freecam
		if (entry->Hotkeys[0].IsPressed()) {
			isFreecamInUse = !isFreecamInUse;

			int freecamStatus = isFreecamInUse ? static_cast<int>(CUTSCENE) : static_cast<int>(GAMEPLAY);
			std::string notif = isFreecamInUse ? "[FREECAM] Freecam in-use." : "[FREECAM] Freecam disabled.";

			Process::Write8(AddressList::CameraMode.addr, freecamStatus);
			OSD::Notify(notif);
		}

		managePlayerLock();

		// toggle camera lock
		if (entry->Hotkeys[1].IsPressed()) {
			isCameraLocked = !isCameraLocked;

			std::string lockNotif = isFreecamInUse ? "locked." : "unlocked.";
			OSD::Notify("[FREECAM] Camera position has been manually" + lockNotif);
		}

		lockCamera();

		// reset camera
		if (entry->Hotkeys[2].IsPressed()) {
			Process::Write8(AddressList::CameraMode.addr, static_cast<int>(GAMEPLAY)); // use GAMEPLAY cam to re-orient camera back to player 

			// reset rotation and perspective zoom as this doesn't happen automatically
			Process::WriteFloat(AddressList::PerspectiveZoom.addr, 1.0);
			Process::Write32(AddressList::OrthographicZoom.addr, 0x41D80000);
			Process::Write32(AddressList::CameraRotationX.addr, 0x271C71C6);
			Process::Write32(AddressList::CameraRotationZ.addr, 0x00000000);

			if (isFreecamInUse)
				Process::Write8(AddressList::CameraMode.addr, static_cast<int>(CUTSCENE)); // if freecam is activated, use CUTSCENE cam

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

	void zoomCamIn(void) {
		Address zoom = usePerspectiveZoom ? AddressList::PerspectiveZoom : AddressList::OrthographicZoom.addr;

		Process::ReadFloat(zoom.addr, cameraZoom);
		Process::WriteFloat(zoom.addr, (cameraZoom * (1 + zoomSensitivity)));
	}

	void zoomCamOut(void) {
		Address zoom = usePerspectiveZoom ? AddressList::PerspectiveZoom : AddressList::OrthographicZoom.addr;

		Process::ReadFloat(zoom.addr, cameraZoom);
		Process::WriteFloat(zoom.addr, (cameraZoom * (1 - zoomSensitivity)));
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

	void rotateCamZCounter(void) {
		Process::ReadFloat(AddressList::CameraRotationZ.addr, cameraZrotation);
		Process::WriteFloat(AddressList::CameraRotationZ.addr, (cameraZrotation * rotationSensitivity));

		// if value becomes invalid, fix it after
		Process::Read32(AddressList::CameraRotationZ.addr, cameraZrawValue);
		if (cameraZrawValue == 0x00000000)
			Process::Write32(AddressList::CameraRotationZ.addr, 0xFE7F0000);

		if (cameraZrawValue == 0x000081FF || cameraZrawValue == 0x00008180)
			Process::Write32(AddressList::CameraRotationZ.addr, 0x00007E80);

		if (cameraZrawValue == 0x80000000)
			Process::Write32(AddressList::CameraRotationZ.addr, 0x7F000000);
	}

	void rotateCamZClockwise(void) {
		Process::ReadFloat(AddressList::CameraRotationZ.addr, cameraZrotation);
		Process::WriteFloat(AddressList::CameraRotationZ.addr, (cameraZrotation / rotationSensitivity));

		// if value becomes invalid, fix it after
		Process::Read32(AddressList::CameraRotationZ.addr, cameraZrawValue);
		if (cameraZrawValue == 0x7F8F0000 || cameraZrawValue == 0x7F800000)
			Process::Write32(AddressList::CameraRotationZ.addr, 0x80800000);

		if (cameraZrawValue == 0xFF7F0000 || cameraZrawValue == 0x00000000 || cameraZrawValue == 0xFF800000)
			Process::Write32(AddressList::CameraRotationZ.addr, 0x00800000);

	}

	void editHotkeys(MenuEntry* entry)
	{
		// placeholder
		Keyboard kbd("dummy text");

		std::string title;
		StringVector opts;

		bool loop = true;
		kbd.CanAbort(false);

		while (loop) {
			title = "Choose a Freecam hotkey to edit.\n\nNote: ZL/ZR and C-stick controls are available\nonly on n2/3DS models.";

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
			opts.push_back(std::string("Counterclockwise X-rotation"));
			opts.push_back(std::string("Clockwise X-rotation"));
			opts.push_back(std::string("Counterclockwise Z-rotation"));
			opts.push_back(std::string("Clockwise Z-rotation"));

			kbd.GetMessage() = title;
			kbd.Populate(opts);

			int chose = kbd.Open();
			if (chose == 0) {
				// end loop = exit the menu
				loop = false;
				break;
			}
			else menuFreecam->Hotkeys[chose - 1].AskForKeys();
		}
	}

	void lockCamera(void) {
		u32 edits[8] = {
			0xE3500000,
			0xE3500002,
			0x1A000002,
			0xEA000008,
			0xE3500001,
			0xE3500000,
			0x0A000002,
			0x0A000008
		};

		int index = (isFreecamInUse == true || isCameraLocked == true) ? 0 : 4;

		Process::Patch(AddressList::DynamicCameraCheck.addr, edits[0 + index]);
		Process::Patch(AddressList::GameplayCameraCheck.addr, edits[1 + index]);
		Process::Patch(AddressList::GameplayCameraInit.addr, edits[2 + index]);
		Process::Patch(AddressList::RetGameplayCameraInit.addr, edits[3 + index]);
	}

	void managePlayerLock(void) {
		int lock = isFreecamInUse ? 0x10 : 0x00;
		Process::Write8(AddressList::LockMovement.addr, lock);
	}

	void editSensitivity(MenuEntry* entry)
	{
		// placeholder
		Keyboard kbd("dummy text");

		std::string title;
		StringVector opts;

		bool loop = true;
		kbd.CanAbort(false);

		while (loop) {
			title = "Current Freecam sensitivity levels:\n\n";
			title.append("Shift sensitivity: " + std::to_string(shiftSensitivity));
			title.append("\nHeight sensitivity: " + std::to_string(heightSensitivity));
			title.append("\nZoom sensitivity: " + std::to_string(zoomSensitivity));
			title.append("\nRotation sensitivity: " + std::to_string(rotationSensitivity));

			// update bottom screen options
			opts.clear();
			opts.push_back(std::string("Set shift sensitivity"));
			opts.push_back(std::string("Set height sensitivity"));
			opts.push_back(std::string("Set zoom sensitivity"));
			opts.push_back(std::string("Set rotation sensitivity"));
			opts.push_back(std::string("Save and exit"));

			kbd.GetMessage() = title;
			kbd.Populate(opts);

			switch (kbd.Open()) {
			case 0:
			{
				shiftSensitivity = setSensitivity("Shift Sensitivity Value\n\nRecommended values: [0.05 - 0.30]");
				break;
			}
			case 1:
			{
				heightSensitivity = setSensitivity("Height Sensitivity Value\n\nRecommended values : [0.40 - 1.00] ");
				break;
			}
			case 2:
			{
				zoomSensitivity = setSensitivity("Zoom Sensitivity Value\n\nRecommended values: [0.02 - 0.04]");
				break;
			}
			case 3:
			{
				rotationSensitivity = setSensitivity("Rotation Sensitivity Value\n\nRecommended values: [0.10 - 0.50]");
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
		sizeKB.SetMaxLength(4);
		sizeKB.CanAbort(true);
		sizeKB.IsHexadecimal(false);
		sizeKB.Open(result);

		if (result < 0.0)
			MessageBox(Color::Gainsboro << "Error", "Sensitivity cannot be negative.")();

		return result;
	}

	void setZoomType(MenuEntry* entry) {
		usePerspectiveZoom = !usePerspectiveZoom;

		std::string name = usePerspectiveZoom ? "Swap to Orthographic Zoom" : "Swap to Perspective Zoom";
		swapZoom->SetName(name);
	}
}

