#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
	// TODO: cam-anim freeze bypass in boss stages (remove support?)
	// TODO: set camera on X link -> hotkey
	// TODO: disable Start/Select behavior -> Pause Menu

	MenuEntry* menuFreecam;
    MenuEntry* editFreecamControls;
    MenuEntry* editFreecamSen;
    MenuEntry* swapZoom;

	bool isFreecamInUse;
	bool isCameraLocked;
	bool usePerspectiveZoom = false;

	u32 cameraX_Rotation;
	u32 cameraZ_Rotation;
	u32 rotationFactor = 0x02000000;

	float shiftSensitivity = 0.2;
	float heightSensitivity = 0.6;
	float zoomSensitivity = 0.03;
	float rotationSensitivity = 0.5;
	float cameraX_Coord;
	float cameraY_Coord;
	float cameraZ_Coord;
	float cameraZoom;

	// float CS_DP_ShiftDistance;

	/* ------------------ */

	// Driver code for Freecam
	void Freecam::useFreecam(MenuEntry* entry)
	{
		// if Freecam has been turned off, restore DYNAMIC camera shift ability...
		if (!entry->IsActivated())
		{
			manageDynamicCamShifts(false);
			manageZ_AxisReturnShift(false);
		}

		// enable/disable freecam...
		if (entry->Hotkeys[0].IsPressed())
		{
			isFreecamInUse = !isFreecamInUse;
			isCameraLocked = true;

			// disable DYNAMIC camera movements...
			manageDynamicCamShifts(true);
			manageZ_AxisReturnShift(true);

			// unhook camera from player...
			OSD::Notify(isFreecamInUse ? "[FREECAM] Freecam in-use." : "[FREECAM] Freecam disabled.");
			setCameraType(CUTSCENE);
		}

		// toggle camera lock...
		if (entry->Hotkeys[1].IsPressed())
		{
			if (isFreecamInUse && isCameraLocked)
				OSD::Notify("[FREECAM] Cannot disengage camera lock while Freecam is active.");
			else
			{
				isCameraLocked = !isCameraLocked;

				manageDynamicCamShifts(true);
				manageZ_AxisReturnShift(isCameraLocked);

				// unhook/reattach camera to player...
				setCameraType(isCameraLocked ? CUTSCENE : GAMEPLAY);

				std::string notif = isCameraLocked ? "locked in-place." : "re-attached to player.";
				OSD::Notify("[FREECAM] Camera position has been " + notif);
			}
		}

		// reset camera position...
		if (entry->Hotkeys[2].IsPressed())
		{
			// re-orient camera back to player...
			if (setCameraType(GAMEPLAY))
			{
				// restore DYNAMIC camera behavior...
				manageDynamicCamShifts(false);
				manageZ_AxisReturnShift(true);

				// reset rotation + zoom as these aren't tied to player coordinates...
				Process::WriteFloat(AddressList::getAddress("PerspectiveZoom"), 1.0);
				Process::Write32(AddressList::getAddress("OrthographicZoom"), 0x41D80000);
				Process::Write32(AddressList::getAddress("CameraRotationX"), 0x271C71C6);
				Process::Write32(AddressList::getAddress("CameraRotationZ"), 0x00000000);

				Player::resetOffset();

				// camera previously configured to unhook from player...
				if (isCameraLocked)
				{
					Sleep(Milliseconds(100)); // allow time to reorient camera before desync...

					manageDynamicCamShifts(true);
					setCameraType(CUTSCENE);
				}
				else
					manageZ_AxisReturnShift(false);

				OSD::Notify("[FREECAM] Camera position has been reset.");
			}
		}

		// check status of camera controls...
		if (isFreecamInUse == true)
		{
			if (entry->Hotkeys[3].IsDown())
				shiftCamY(true); // south

			if (entry->Hotkeys[4].IsDown())
				shiftCamY(false); // north

			if (entry->Hotkeys[5].IsDown())
				shiftCamX(true); // east

			if (entry->Hotkeys[6].IsDown())
				shiftCamX(false); // west

			if (entry->Hotkeys[7].IsDown())
				adjustCamZoom(true);

			if (entry->Hotkeys[8].IsDown())
				adjustCamZoom(false);

			if (entry->Hotkeys[9].IsDown())
				adjustCamHeight(true);

			if (entry->Hotkeys[10].IsDown())
				adjustCamHeight(false);

			if (entry->Hotkeys[11].IsDown())
				rotateCamX(true);

			if (entry->Hotkeys[12].IsDown())
				rotateCamX(false);

			if (entry->Hotkeys[13].IsDown())
				rotateCamZ(true);

			if (entry->Hotkeys[14].IsDown())
				rotateCamZ(false);
		}

		// prevent player from moving if Freecam is active...
		GeneralHelpers::managePlayerLock(isFreecamInUse);
		// allow camera lock to be maintained after entering new area...
		if (GeneralHelpers::isLoadingScreen() && isCameraLocked)
			setCameraType(CUTSCENE);
	}

	// Sets the current camera mode -- used to toggle the camera's ability to follow the player
	bool setCameraType(cameraMode mode)
	{
		return Process::Write8(AddressList::getAddress("CameraMode"), static_cast<int>(mode));
	}

	// Shifts the camera position along the X-axis (east-west)
	void shiftCamX(bool west)
	{
		float shiftAmount = west ? shiftSensitivity : -shiftSensitivity;
		Process::ReadFloat(AddressList::getAddress("CameraPosX"), cameraZ_Coord);
		Process::WriteFloat(AddressList::getAddress("CameraPosX"), (cameraZ_Coord + shiftAmount));
	}

	// Shifts the camera position along the Y-axis (north-south)
	void shiftCamY(bool south)
	{
		float shiftAmount = south ? -shiftSensitivity : shiftSensitivity;
		Process::ReadFloat(AddressList::getAddress("CameraPosZ"), cameraZ_Coord);
		Process::WriteFloat(AddressList::getAddress("CameraPosZ"), (cameraZ_Coord + shiftAmount));
	}

	// Adjusts the camera's zoom level
	void adjustCamZoom(bool zoomIn)
	{
		u32 zoomType = usePerspectiveZoom ? AddressList::getAddress("PerspectiveZoom") : AddressList::getAddress("OrthographicZoom");
		float zoomAmount = zoomIn ? zoomSensitivity : -zoomSensitivity;

		Process::ReadFloat(zoomType, cameraZoom);
		Process::WriteFloat(zoomType, (cameraZoom * (1 + zoomAmount)));
	}

	// Sets the camera's position along the Z-axis (vertical axis)
	void adjustCamHeight(bool isRaise)
	{
		float shiftAmount = isRaise ? heightSensitivity : -heightSensitivity;
		Process::ReadFloat(AddressList::getAddress("CameraPosY"), cameraY_Coord);
		Process::WriteFloat(AddressList::getAddress("CameraPosY"), (cameraY_Coord + shiftAmount));
	}

	// Rotates the camera along the X-axis. Overflow correction has also been (lazily) recreated here
	void rotateCamX(bool counterclockwise)
	{
		u32 maxThreshold = 0x40000000;
		s32 rotationScale = static_cast<s32>(rotationFactor * rotationSensitivity);

		if (counterclockwise)
			rotationScale = -rotationScale;

		Process::Read32(AddressList::getAddress("CameraRotationX"), cameraX_Rotation);
		cameraX_Rotation = cameraX_Rotation + rotationScale;

		// manual adjustments to prevent overflow...
		if (cameraX_Rotation > maxThreshold)
			cameraX_Rotation = counterclockwise ? 0x0000FFFF : 0x3FFF0000;
		else if (cameraX_Rotation < 0x0000FFFF)
			cameraX_Rotation = counterclockwise ? 0x0 : cameraX_Rotation;

		Process::Write32(AddressList::getAddress("CameraRotationX"), cameraX_Rotation);
	}

	// Rotates the camera along the Z-axis
	void rotateCamZ(bool counterclockwise)
	{
		s32 rotationScale = static_cast<s32>(rotationFactor * rotationSensitivity);

		if (counterclockwise)
			rotationScale = -rotationScale;

		Process::Read32(AddressList::getAddress("CameraRotationZ"), cameraZ_Rotation);
		Process::Write32(AddressList::getAddress("CameraRotationZ"), (cameraZ_Rotation + rotationScale));
	}

	void Freecam::editHotkeys(MenuEntry* entry)
	{
		Keyboard menu("");

		std::string title;
		StringVector opts;

		bool isMenuOpen = true;
		menu.CanAbort(false);

		while (isMenuOpen)
		{
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

			menu.GetMessage() = title;
			menu.Populate(opts);

			int chose = menu.Open();
			if (chose == 0)
			{
				isMenuOpen = false;
				break;
			}
			else
				menuFreecam->Hotkeys[chose - 1].AskForKeys();
		}
	}

	// Restores/patches out dynamic camera movements caused by item usage, Doppel swaps, swim boosts, etc.
	void manageDynamicCamShifts(bool disableShifts)
	{
		u32 edits[8] =
		{
			0xE3500000,
			0xE3500002,
			0x1A000002,
			0xEA000008,
			0xE3500001,
			0xE3500000,
			0x0A000002,
			0x0A000008
		};

		int index = disableShifts ? 0 : 4;

		Process::Patch(AddressList::getAddress("DynamicCameraCheck"), edits[0 + index]);
		Process::Patch(AddressList::getAddress("GameplayCameraCheck"), edits[1 + index]);
		Process::Patch(AddressList::getAddress("GameplayCameraInit"), edits[2 + index]);
		Process::Patch(AddressList::getAddress("RetGameplayCameraInit"), edits[3 + index]);
	}

	// Toggles the vertical axis gradual return function, allowing the Freecam to reset its Z-position instantly
	void manageZ_AxisReturnShift(bool disableGradualShifts)
	{
		Process::Patch(AddressList::getAddress("VerticalCamShiftUpwards"), disableGradualShifts ? 0x8A000002 : 0x9A000002);
		Process::Patch(AddressList::getAddress("VerticalCamShiftDownwards"), disableGradualShifts ? 0x2A000009 : 0x3A000009);
	}

	// Allows custom sensitivity levels to be chosen for each hotkey group
	void Freecam::editSensitivity(MenuEntry* entry)
	{
		Keyboard menu("");

		std::string title = "Current Freecam sensitivity levels:\n\n";
		StringVector opts;

		bool isMenuOpen = true;
		menu.CanAbort(false);

		while (isMenuOpen)
		{
			title.append("Shift sensitivity: " + std::to_string(shiftSensitivity));
			title.append("\nHeight sensitivity: " + std::to_string(heightSensitivity));
			title.append("\nZoom sensitivity: " + std::to_string(zoomSensitivity));
			title.append("\nRotation sensitivity: " + std::to_string(rotationSensitivity));

			// update bottom screen options...
			opts.clear();
			opts.push_back(std::string("Set shift sensitivity"));
			opts.push_back(std::string("Set height sensitivity"));
			opts.push_back(std::string("Set zoom sensitivity"));
			opts.push_back(std::string("Set rotation sensitivity"));
			opts.push_back(std::string("Save and exit"));

			menu.GetMessage() = title;
			menu.Populate(opts);

			switch (menu.Open())
			{
				case 0:
					shiftSensitivity = setSensitivity("Shift Sensitivity Value\n\nRecommended values: [0.05 - 0.30]");
					break;
				case 1:
					heightSensitivity = setSensitivity("Height Sensitivity Value\n\nRecommended values : [0.40 - 1.00] ");
					break;
				case 2:
					zoomSensitivity = setSensitivity("Zoom Sensitivity Value\n\nRecommended values: [0.02 - 0.04]");
					break;
				case 3:
					rotationSensitivity = setSensitivity("Rotation Sensitivity Value\n\nRecommended values: [0.10 - 0.50]");
					break;
				default:
					isMenuOpen = false;
					break;
			}
		}
	}

	// Helper function to prompt user for custom sensitivity values
	float setSensitivity(std::string message)
	{
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

	// Toggles between Orthographic and Perspective Zoom
	void Freecam::setZoomType(MenuEntry* entry)
	{
		usePerspectiveZoom = !usePerspectiveZoom;

		std::string name = usePerspectiveZoom ? "Swap to Orthographic Zoom" : "Swap to Perspective Zoom";
		swapZoom->SetName(name);
	}
}

