#include "Helpers/DataHelpers/Address.hpp"
#include "AddressList.hpp"

namespace CTRPluginFramework {

	void AddressList::InitAddresses(void) {
		/* Camera */
		DynamicCameraCheck 			= Address(0x00143810, 0x0014382C, 0x001439FC, 0x001435DC);
		GameplayCameraCheck 		= Address(0x00143A7C, 0x00143A98, 0x00143C68, 0x00143848);
		GameplayCameraInit 			= Address(0x00143A80, 0x00143A9C, 0x00143C6C, 0x0014384C);
		RetGameplayCameraInit 		= Address(0x00143B80, 0x00143B9C, 0x00143D6C, 0x0014394C);

		CameraLinkFocus 			= Address(0x32C815D0, 0x32C81650, 0x32CCC010, 0x32C80CD0);
		CameraMode 					= Address(0x32C814BC, 0x32C8153C, 0x32CCBEFC, 0x32C80BBC);
		CameraPosX 					= Address(0x32C814C0, 0x32C81540, 0x32CCBF00, 0x32C80BC0);
		CameraPosY 					= Address(0x32C814C4, 0x32C81544, 0x32CCBF04, 0x32C80BC4);
		CameraPosZ 					= Address(0x32C814C8, 0x32C81548, 0x32CCBF08, 0x32C80BC8);
		CameraRotationX 			= Address(0x32C81520, 0x32C815A0, 0x32CCBF60, 0x32C80C20);
		CameraRotationZ 			= Address(0x32C81524, 0x32C815A4, 0x32CCBF64, 0x32C80C24);
		OrthographicZoom 			= Address(0x32C81528, 0x32C815A8, 0x32CCBF68, 0x32C80C28);
		PerspectiveZoom 			= Address(0x005F529C, 0x005F56CC, 0x005F621C, 0x005F5204);

		/* Chaos */
		HeartsTakeHealth 			= Address(0x0015CA04, 0x0015CA20, 0x0015CEF0, 0x0015C8AC);

		/* Costume */
		CostumeAttrA 				= Address(0x32849510, 0x32849590, 0x32893F50, 0x32848C10);
		CostumeAttrB 				= Address(0x32849512, 0x32849592, 0x32893F52, 0x32848C12);
		CostumeAttrC 				= Address(0x32849513, 0x32849593, 0x32893F53, 0x32848C13);
		CostumeAttrD				= Address(0x32849514, 0x32849594, 0x32893F54, 0x32848C14);
		CurrCostume 				= Address(0x32849564, 0x328495E4, 0x32893FA4, 0x32848C64);
		CostumeCatalogPointer 		= Address(0x32C828E8, 0x32C82968, 0x32CCD328, 0x32C81FE8);
		PlayerStatus 				= Address(0x32849511, 0x32849591, 0x32893F51, 0x32848C11);
		TingleBalloons 				= Address(0x32849680, 0x32849700, 0x328940C0, 0x32848D80);
		UnusedCostumeDataPointers 	= Address(0x0075C390, 0x0075C398, 0x0075C388, 0x0075B378);
		BearMinMaxCheckA			= Address(0x005FAACC, 0x005FAEFC, 0x005FBA64, 0x005FAA4C);
		BearMinMaxCheckB			= Address(0x005FAAD0, 0x005FAF00, 0x005FBA68, 0x005FAA50);

		/* Console */

		/* Emotes */
		GameplayEmotes 				= Address(0x006B6E98, 0x006B6E98, 0x006B6E98, 0x006B5E98);
		LobbyEmotes 				= Address(0x006B6EA0, 0x006B6EA0, 0x006B6EA0, 0x006B5EA0);
		EmoteAGraphic 				= Address(0x32C91830, 0x32C918B0, 0x32CDC420, 0x32C90F30);
		EmoteBGraphic 				= Address(0x32C9225C, 0x32C922DC, 0x32CDCE4C, 0x32C9195C);
		EmoteCGraphic 				= Address(0x32C92BEC, 0x32C92C6C, 0x32CDD7DC, 0x32C922EC);
		EmoteDGraphic 				= Address(0x32C9357C, 0x32C935FC, 0x32CDE16C, 0x32C92C7C);
		EmoteEGraphic 				= Address(0x32C93F0C, 0x32C93F8C, 0x32CDEAFC, 0x32C9360C);
		EmoteFGraphic 				= Address(0x32C9489C, 0x32C9491C, 0x32CDF48C, 0x32C93F9C);
		EmoteGGraphic 				= Address(0x32C9522C, 0x32C952AC, 0x32CDFE1C, 0x32C9492C);
		EmoteHGraphic 				= Address(0x32C95BBC, 0x32C95C3C, 0x32CE07AC, 0x32C952BC);
		UseDoTLayoutAlways 			= Address(0x002F0C6C, 0x002F1010, 0x002F1B94, 0x002F0EDC);

		/* Game Data */
		CurrLevelID 				= Address(0x3280EDC4, 0x3280EE44, 0x32859804, 0x3280E4C4);
		CurrStageID 				= Address(0x3280EDC8, 0x3280EE48, 0x32859808, 0x3280E4C8);
		CurrSpawnID 				= Address(0x3280EDCA, 0x3280EE4A, 0x3285980A, 0x3280E4CA);
		DoppelsEnabled 				= Address(0x32D81E46, 0x32D81EC6, 0x32DD466A, 0x32D81546);
		DrablandsRupeeCount 		= Address(0x32C8178C, 0x32C8180C, 0x32CCC1CC, 0x32C80E8C);
		EnemyDataPointer			= Address(0x32989188, 0x32989208, 0x329D3BC8, 0x32988888);
		HealthCurrent 				= Address(0x32C824F4, 0x32C82574, 0x32CCCF34, 0x32C81BF4);
		HealthMax 					= Address(0x32C824F5, 0x32C82575, 0x32CCCF35, 0x32C81BF5);
		EnergyCurrent				= Address(0x328496C4, 0x32849744, 0x32894104, 0x32848DC4);
		EnergyMax					= Address(0x00192644, 0x001926D4, 0x00192BC8, 0x00192588);
		EnergyMaxPitRecovery		= Address(0x001925F8, 0x00192688, 0x00192B7C, 0x0019253C);
		EnergyMaxMultiplicand		= Address(0x00192684, 0x00192714, 0x00192C08, 0x001925C8);
		FairiesCurrent				= Address(0x32D81F14, 0x32D81F94, 0x32DD4738, 0x32D81614);
		TimeLeft					= Address(0x32D81F18, 0x32D81F98, 0x32DD473C, 0x32D81618);
		CollisionCurrent			= Address(0x328416A4, 0x32841724, 0x3288C0E4, 0x32840DA4);
		SpeedX						= Address(0x32841650, 0x328416D0, 0x3288C090, 0x32840D50);
		SpeedY						= Address(0x32841654, 0x328416D4, 0x3288C094, 0x32840D54);
		SpeedZ						= Address(0x32841658, 0x328416D8, 0x3288C098, 0x32840D58);
		TotemCarrying				= Address(0x32849520, 0x328495A0, 0x32893F60, 0x32848C20);
		TotemCarriedBy				= Address(0x32849524, 0x328495A4, 0x32893F64, 0x32848C24);
		LoadingStatus 				= Address(0x319FE1D0, 0x319FE228, 0x319FDF1C, 0x319FCA80);
		PauseStatus 				= Address(0x3280ED8C, 0x3280EE0C, 0x328597CC, 0x3280E48C);
		PreviousLevelID				= Address(0x3280EDBC, 0x3280EE3C, 0x328597FC, 0x3280E4BC);
		PreviousStageID				= Address(0x3280EDC0, 0x3280EE40, 0x32859800, 0x3280E4C0);
		PreviousSpawnID 			= Address(0x3280EDC2, 0x3280EE42, 0x32859802, 0x3280E4C2);
		TimeElapsed 				= Address(0x3280EDE0, 0x3280EE60, 0x32859820, 0x3280E4E0);
		TargetLevelID				= Address(0x32C81F10, 0x32C81F90, 0x32CCC950, 0x32C81610);
		TargetStageID				= Address(0x32C81F14, 0x32C81F94, 0x32CCC954, 0x32C81614);
		TargetSpawnID				= Address(0x32C81F16, 0x32C81F96, 0x32CCC956, 0x32C81616);
		Warp						= Address(0x3282C4EC, 0x3282C56C, 0x32876F2C, 0x3282BBEC);
		WarpActivation 				= Address(0x32C81F00, 0x0, 0x0, 0x0);
		WarpPointer					= Address(0x32C81E28, 0x32C81EA8, 0x32CCC868, 0x32C81528);
		ChallengeID					= Address(0x32D81E3C, 0x32D81EBC, 0x32DD4660, 0x32D8153C);

		/* Items */
		BowStrafe 					= Address(0x0038CC7C, 0x0038D024, 0x0038DC00, 0x0038CECC);
		FireGloveStrafe 			= Address(0x003C8FD8, 0x003C9380, 0x003C9F5C, 0x003C9228);
		GustStrafe 					= Address(0x003C85A8, 0x003C8950, 0x003C952C, 0x003C87F8);
		HammerStrafe 				= Address(0x003C03E0, 0x003C0788, 0x003C1364, 0x003C0630);
		ShadowLinkItemPointer 		= Address(0x32D81DEC, 0x32D81E6C, 0x32DD4610, 0x32D814EC);
		RoboNiceAtk 				= Address(0x00156AB4, 0x00156AD0, 0x00156FA0, 0x0015695C);
		RoboNiceSFX 				= Address(0x003C2030, 0x003C23D8, 0x003C2FB4, 0x003C2280);
		RoboNiceSpeed 				= Address(0x00156898, 0x001568B4, 0x00156D84, 0x00156740);
		GripshotStrafe 				= Address(0x003C214C, 0x003C24F4, 0x003C30D0, 0x003C239C);
		WaterRodStrafe 				= Address(0x003C59C0, 0x003C5D68, 0x003C6944, 0x003C5C10);

		/* Miscellaneous */
		CameraOnX 					= Address(0x32D92920, 0x32D929A0, 0x32DE5220, 0x32D92020);
		CameraShutter 				= Address(0x32D92E9C, 0x32D92F1C, 0x32DE579C, 0x32D9259C);
		CheckPhotoExist 			= Address(0x31B2FE3D, 0x31B2FEBD, 0x31B2FBBD, 0x31B2E6BD);
		DisplayTopPhoto 			= Address(0x31B2FE21, 0x31B2FEA1, 0x31B2FBA1, 0x31B2E6A1);
		SeeChestContents 			= Address(0x0022FFB8, 0x00230040, 0x00230CDC, 0x0023016C);
		SwordBeamCD 				= Address(0x328495A0, 0x32849620, 0x32893FE0, 0x32848CA0);
		TextBoxSpeed 				= Address(0x0018122C, 0x001812BC, 0x001817AC, 0x0018116C);

		/* Padding */
		TextToRodata 				= Address(0x006A9168, 0x006A9598, 0x006A9FE4, 0x006A8FDC);

		/* Player */
		ActiveLink 					= Address(0x31A03879, 0x31A038D1, 0x31A035C5, 0x31A02129);
		CurrentAnim 				= Address(0x32849188, 0x32849208, 0x32893BC8, 0x32848888);
		CurrentItem 				= Address(0x32841A20, 0x32841AA0, 0x3288C460, 0x32841120);
		//check if jinxed is model or effects also
		IsInvincible 				= Address(0x32849544, 0x328495C4, 0x32893F84, 0x32848C44);
		IsJinxed 					= Address(0x328495AC, 0x3284962C, 0x32893FEC, 0x32848CAC);
		IsSpawned 					= Address(0x32841410, 0x32841490, 0x3288BE50, 0x32840B10);
		IsSwapping 					= Address(0x31A04610, 0x31A04668, 0x31A0435C, 0x31A02EC0);
		IsWaterStorage				= Address(0x32849231, 0x328492B1, 0x32893C71, 0x32848931);
		IsVisible 					= Address(0x32849550, 0x328495D0, 0x32893F90, 0x32848C50);
		LinkVoice 					= Address(0x3284A260, 0x3284A2E0, 0x32894CA0, 0x32849960);
		LiveNameColorG 				= Address(0x007767F0, 0x007767F8, 0x007767E8, 0x007757D8);
		LiveNameColorB 				= Address(0x007767F4, 0x007767FC, 0x007767EC, 0x007757DC);
		LiveNameColorR 				= Address(0x007767F8, 0x00776800, 0x007767F0, 0x007757E0);
		LockMovement 				= Address(0x31A0115B, 0x31A011B3, 0x31A00EA7, 0x319FFA0B);
		NoCollision 				= Address(0x32849508, 0x32849588, 0x32893F48, 0x32848C08);
		PlayerModelSize 			= Address(0x001FAC64, 0x001FACF4, 0x001FB908, 0x001FAB1C);
		PositionX 					= Address(0x328412D8, 0x32841358, 0x3288BD18, 0x328409D8);
		PositionY 					= Address(0x328412DC, 0x3284135C, 0x3288BD1C, 0x328409DC);
		PositionZ 					= Address(0x328412E0, 0x32841360, 0x3288BD20, 0x328409E0);
		PreviousAnim 				= Address(0x32849189, 0x32849209, 0x32893BC9, 0x32848889);
		PVPMode 					= Address(0x32849545, 0x328495C5, 0x32893F85, 0x32848C45);
		RespawnPositionX 			= Address(0x32841750, 0x328417D0, 0x3288C190, 0x32840E50);
		RespawnPositionY 			= Address(0x32841754, 0x328417D4, 0x3288C194, 0x32840E54);
		RespawnPositionZ 			= Address(0x32841758, 0x328417D8, 0x3288C198, 0x32840E58);
		SwordType 					= Address(0x328419EC, 0x32841A6C, 0x3288C42C, 0x328410EC);

		/* Rendering */
		ArrowIndicatorColorBot 		= Address(0x32D91B24, 0x32D91BA4, 0x32DE4424, 0x32D91224);
		ArrowIndicatorColorMid 		= Address(0x32D91A04, 0x32D91A84, 0x32DE4304, 0x32D91104);
		ArrowIndicatorColorTop 		= Address(0x32D918E4, 0x32D91964, 0x32DE41E4, 0x32D90FE4);
		HUDPointer 					= Address(0x32C8289C, 0x32C8291C, 0x32CCD2DC, 0x32C81F9C);
		FogA 						= Address(0x31B31BE4, 0x31B31C64, 0x31B31964, 0x31B30464);
		FogB 						= Address(0x31B31BDC, 0x31B31C5C, 0x31B3195C, 0x31B3045C);
		FogC 						= Address(0x31B31BE0, 0x31B31C60, 0x31B31960, 0x31B30460);
		ScrollingTextOpacity 		= Address(0x00151C40, 0x00151C5C, 0x00152050, 0x00151A0C);
		OffScreenLocation 			= Address(0x32D83094, 0x32D83114, 0x32DD58B8, 0x32D82794);
		TextureName					= Address(0x3033D7C3, 0x30363E43, 0x3037F5C3, 0x302F4043);
		LoadingScreenSPNoChal		= Address(0x002B1410, 0x002B17B4, 0x002B2338, 0x002B1680);
		LoadingScreenSPIcons		= Address(0x00390C08, 0x00390FB0, 0x00391B88, 0x00390E58);
		CheerPomPom					= Address(0x001F9AFC, 0x001F9B8C, 0x001FA7A0, 0x001F99B4);
		Aura						= Address(0x001C3030, 0x001C30C0, 0x001C35B4, 0x001C2F74);

		/* Savefile */
		NormalNCCompletion 			= Address(0x31A060EC, 0x31A06144, 0x31A05E38, 0x31A0499C);
		NormalC1Completion 			= Address(0x31A060F4, 0x31A0614C, 0x31A05E40, 0x31A049A4);
		NormalC2Completion 			= Address(0x31A060FC, 0x31A06154, 0x31A05E48, 0x31A049AC);
		NormalC3Completion 			= Address(0x31A06104, 0x31A0615C, 0x31A05E50, 0x31A049B4);
		SoloNCCompletion 			= Address(0x31A0616C, 0x31A061C4, 0x31A05EB8, 0x31A04A1C);
		SoloC1Completion 			= Address(0x31A06174, 0x31A061CC, 0x31A05EC0, 0x31A04A24);
		SoloC2Completion 			= Address(0x31A0617C, 0x31A061D4, 0x31A05EC8, 0x31A04A2C);
		SoloC3Completion 			= Address(0x31A06184, 0x31A061DC, 0x31A05ED0, 0x31A04A34);
		MultiNCCompletion 			= Address(0x31A0612C, 0x31A06184, 0x31A05E78, 0x31A049DC);
		MultiC1Completion 			= Address(0x31A06134, 0x31A0618C, 0x31A05E80, 0x31A049E4);
		MultiC2Completion 			= Address(0x31A0613C, 0x31A06194, 0x31A05E88, 0x31A049EC);
		MultiC3Completion 			= Address(0x31A06144, 0x31A0619C, 0x31A05E90, 0x31A049F4);

		CostumeObtainStatus 		= Address(0x32C8256A, 0x32C825EA, 0x32CCCFAA, 0x32C81C6A);
		ColiseumWinCount 			= Address(0x31A062AC, 0x31A06304, 0x31A05FF8, 0x31A04B5C);
		EditMerchantStock 			= Address(0x31A077A4, 0x31A077FC, 0x31A074F0, 0x31A0605C);
		HeroPointCount 				= Address(0x31A0611C, 0x31A06174, 0x31A05E68, 0x31A049CC);
		MainVoice 					= Address(0x31A06118, 0x31A06170, 0x31A05E64, 0x31A049C8);
		ResetMerchantStock 			= Address(0x31A077AE, 0x31A07806, 0x31A074FA, 0x31A0605E);

		// SFX and BGM
		BGMType 				= Address(0x005FD3C0, 0x005FD7F0, 0x005FE358, 0x005FD340);
		BGMVolume 					= Address(0x00236444, 0x002364D4, 0x00237170, 0x00236600);
		LobbyBallPointer			= Address(0x32DCAA60, 0x32DCAAE0, 0x32E277F4, 0x32DCA160);
		LinkVoiceSpeed 				= Address(0x3284A25C, 0x3284A2DC, 0x32894C9C, 0x3284995C);
		Voice8bit	 				= Address(0x00194630, 0x001946C0, 0x00194BB4, 0x00194574);
		MasterVolume 				= Address(0x0050BC74, 0x0050BDF0, 0x0050C9C4, 0x0050BC98);

		// Turbo Mode
		DashTimerNormal				= Address(0x0039CE08, 0x00000000, 0x00000000, 0x00000000);
		DashTimerNinja				= Address(0x0039CE0C, 0x00000000, 0x00000000, 0x00000000);
		SwordBeamCDExe				= Address(0x005CC998, 0x00000000, 0x00000000, 0x00000000);
		HammerwearSpeed				= Address(0x003BFD08, 0x00000000, 0x00000000, 0x00000000);
		SL_SwordSpeed				= Address(0x00251538, 0x00000000, 0x00000000, 0x00000000);
		SL_Strafe					= Address(0x00251558, 0x00000000, 0x00000000, 0x00000000);
		SL_RunAnimSpeed				= Address(0x0025155C, 0x00000000, 0x00000000, 0x00000000);
		SL_RunSpeed					= Address(0x00251560, 0x00000000, 0x00000000, 0x00000000);
		BombFlashSpeed				= Address(0x0057FB74, 0x00000000, 0x00000000, 0x00000000);
		LoadingIconAnimSpeed		= Address(0x00390C04, 0x00000000, 0x00000000, 0x00000000);
		LiveMsgSpeed				= Address(0x00152180, 0x00000000, 0x00000000, 0x00000000);
		SwordBeamSpeed				= Address(0x001BD9EC, 0x00000000, 0x00000000, 0x00000000);
		SpeedDamageUndo				= Address(0x0031663C, 0x00000000, 0x00000000, 0x00000000);
		BGMSFXSpeed					= Address(0x0050BA48, 0x00000000, 0x00000000, 0x00000000);
		EnergyBarStretch			= Address(0x005AEBDC, 0x00000000, 0x00000000, 0x00000000);
		SpeedMultiplierNormal		= Address(0x003166A4, 0x00000000, 0x00000000, 0x00000000);
		SpeedMultiplierCheetah		= Address(0x003166B0, 0x00000000, 0x00000000, 0x00000000);
		DashStaggerSpeed			= Address(0x0039B9AC, 0x00000000, 0x00000000, 0x00000000);
		DashSpeed					= Address(0x0039CC2C, 0x00000000, 0x00000000, 0x00000000);
		DashBrakeDuration			= Address(0x0039CC40, 0x00000000, 0x00000000, 0x00000000);
		DashPlayerCrashHeight		= Address(0x0039D4A0, 0x00000000, 0x00000000, 0x00000000);
		DashWallCrashHeight			= Address(0x0039D4AC, 0x00000000, 0x00000000, 0x00000000);
		DashCrashBackwards			= Address(0x0039D4CC, 0x00000000, 0x00000000, 0x00000000);
		FireJumpHeight				= Address(0x003BE9D4, 0x00000000, 0x00000000, 0x00000000);
		LavaBounceHeight			= Address(0x003BCEE8, 0x00000000, 0x00000000, 0x00000000);
		WaterJumpHeight				= Address(0x003A1714, 0x00000000, 0x00000000, 0x00000000);
		WaterJumpForwardSpeed		= Address(0x003A1728, 0x00000000, 0x00000000, 0x00000000);
		SwimBoostSpeedZora			= Address(0x003A204C, 0x00000000, 0x00000000, 0x00000000);
		SwimBoostSpeedNormal		= Address(0x003A2058, 0x00000000, 0x00000000, 0x00000000);
		SwimTurnaroundSpeed			= Address(0x003A2294, 0x00000000, 0x00000000, 0x00000000);
		SwimSpeed					= Address(0x003A2298, 0x00000000, 0x00000000, 0x00000000);
		ArrowSpeedPlayer			= Address(0x0057E084, 0x00000000, 0x00000000, 0x00000000);
		FireballHeight				= Address(0x001855E0, 0x00000000, 0x00000000, 0x00000000);
		FireballSpeed				= Address(0x001855DC, 0x00000000, 0x00000000, 0x00000000);
		FireballBounceHeight		= Address(0x00185DAC, 0x00000000, 0x00000000, 0x00000000);
		FireballThrowHeight			= Address(0x00186AE8, 0x00000000, 0x00000000, 0x00000000);
		FireballReleaseTime			= Address(0x003C8CE8, 0x00000000, 0x00000000, 0x00000000);
		BoomerangReturnSpeed		= Address(0x001B5394, 0x00000000, 0x00000000, 0x00000000);
		BoomerangDistance			= Address(0x001B6300, 0x00000000, 0x00000000, 0x00000000);
		BoomerangSpeed				= Address(0x001B6308, 0x00000000, 0x00000000, 0x00000000);
		GustSpeed					= Address(0x001B24C0, 0x00000000, 0x00000000, 0x00000000);
		GripshotRetractSpeed		= Address(0x001568E4, 0x00000000, 0x00000000, 0x00000000);
		GripshotExtendSpeed			= Address(0x0015690C, 0x00000000, 0x00000000, 0x00000000);

		// Animation Speeds
		Link_Wait					= Address(0x00731008, 0x0073100C, 0x00731000, 0x0072FFFC);
		Link_WalkSlow				= Address(0x00731020, 0x00000000, 0x00000000, 0x00000000);
		Link_Walk					= Address(0x00731038, 0x00000000, 0x00000000, 0x00000000);
		Link_Dash					= Address(0x00731050, 0x00000000, 0x00000000, 0x00000000);
		Link_DashTotem				= Address(0x00731068, 0x00000000, 0x00000000, 0x00000000);
		Link_GrabDash				= Address(0x00731080, 0x00000000, 0x00000000, 0x00000000);
		Link_CutRF					= Address(0x00731098, 0x00000000, 0x00000000, 0x00000000);
		Link_CutRFTotem				= Address(0x007310B0, 0x00000000, 0x00000000, 0x00000000);
		Link_CutRFtoKeep			= Address(0x007310C8, 0x00000000, 0x00000000, 0x00000000);
		Link_KeepWait				= Address(0x007310E0, 0x00000000, 0x00000000, 0x00000000);
		Link_Compete				= Address(0x007310F8, 0x00000000, 0x00000000, 0x00000000);
		Link_CutTurn				= Address(0x00731110, 0x00000000, 0x00000000, 0x00000000);
		Link_CutTurnW				= Address(0x00731128, 0x00000000, 0x00000000, 0x00000000);
		Link_CutTurnB				= Address(0x00731140, 0x00000000, 0x00000000, 0x00000000);
		Link_GrabP_A				= Address(0x00731158, 0x00000000, 0x00000000, 0x00000000);
		Link_GrabP_B				= Address(0x00731170, 0x00000000, 0x00000000, 0x00000000);
		Link_GrabUp					= Address(0x00731188, 0x00000000, 0x00000000, 0x00000000);
		Link_GrabThrow				= Address(0x007311A0, 0x00000000, 0x00000000, 0x00000000);
		Link_GrabWait				= Address(0x007311B8, 0x00000000, 0x00000000, 0x00000000);
		Link_WaitTotem				= Address(0x007311D0, 0x00000000, 0x00000000, 0x00000000);
		Link_GrabYorokeTotem		= Address(0x007311E8, 0x00000000, 0x00000000, 0x00000000);
		Link_YorokeTotem			= Address(0x00731200, 0x00000000, 0x00000000, 0x00000000);
		Link_WaitYoroke				= Address(0x00731218, 0x00000000, 0x00000000, 0x00000000);
		Link_JumpEdS_A				= Address(0x00731230, 0x00000000, 0x00000000, 0x00000000);
		Link_DamF					= Address(0x00731248, 0x00000000, 0x00000000, 0x00000000);
		Link_DamB					= Address(0x00731260, 0x00000000, 0x00000000, 0x00000000);
		Link_FallLava				= Address(0x00731278, 0x00000000, 0x00000000, 0x00000000);
		Link_FallSand				= Address(0x00731290, 0x00000000, 0x00000000, 0x00000000);
		Link_Drown					= Address(0x007312A8, 0x00000000, 0x00000000, 0x00000000);
		Link_DrownLava				= Address(0x007312C0, 0x00000000, 0x00000000, 0x00000000);
		Link_Fall					= Address(0x007312D8, 0x00000000, 0x00000000, 0x00000000);
		Link_DamBiri				= Address(0x007312F0, 0x00000000, 0x00000000, 0x00000000);
		Link_WaitQ					= Address(0x00731308, 0x00000000, 0x00000000, 0x00000000);
		Link_DamBoyon				= Address(0x00731320, 0x00000000, 0x00000000, 0x00000000);
		Link_DamFFup				= Address(0x00731338, 0x00000000, 0x00000000, 0x00000000);
		Link_DamSwimF				= Address(0x00731350, 0x00000000, 0x00000000, 0x00000000);
		Link_DamSwimB				= Address(0x00731368, 0x00000000, 0x00000000, 0x00000000);
		Link_LLhold					= Address(0x00731380, 0x00000000, 0x00000000, 0x00000000);
		Link_LLswallowSt			= Address(0x00731398, 0x00000000, 0x00000000, 0x00000000);
		Link_LLstruggle				= Address(0x007313B0, 0x00000000, 0x00000000, 0x00000000);
		Link_FallTotem				= Address(0x007313C8, 0x00000000, 0x00000000, 0x00000000);
		Link_FallBallon				= Address(0x007313E0, 0x00000000, 0x00000000, 0x00000000);
		Link_BowWalkA				= Address(0x007313F8, 0x00000000, 0x00000000, 0x00000000);
		Link_BowWalkB				= Address(0x00731410, 0x00000000, 0x00000000, 0x00000000);
		Link_Slipice				= Address(0x00731428, 0x00000000, 0x00000000, 0x00000000);
		Link_BowWait				= Address(0x00731440, 0x00000000, 0x00000000, 0x00000000);
		Link_ArrowShoot				= Address(0x00731458, 0x00000000, 0x00000000, 0x00000000);
		Link_RodWait				= Address(0x00731470, 0x00000000, 0x00000000, 0x00000000);
		Link_RodAttack				= Address(0x00731488, 0x00000000, 0x00000000, 0x00000000);
		Link_BoomAttack				= Address(0x007314A0, 0x00000000, 0x00000000, 0x00000000);
		Link_BoomCarried			= Address(0x007314B8, 0x00000000, 0x00000000, 0x00000000);
		Link_HookshotWait			= Address(0x007314D0, 0x00000000, 0x00000000, 0x00000000);
		Link_HookshotAttack			= Address(0x007314E8, 0x00000000, 0x00000000, 0x00000000);
		Link_HookshotDrag			= Address(0x00731500, 0x00000000, 0x00000000, 0x00000000);
		Link_Bump					= Address(0x00731518, 0x00000000, 0x00000000, 0x00000000);
		Link_HookshotPullSt			= Address(0x00731530, 0x00000000, 0x00000000, 0x00000000);
		Link_HookshotPullEd			= Address(0x00731548, 0x00000000, 0x00000000, 0x00000000);
		Link_AirCannonWait			= Address(0x00731560, 0x00000000, 0x00000000, 0x00000000);
		Link_AirCannonAttack		= Address(0x00731578, 0x00000000, 0x00000000, 0x00000000);
		Link_FireGloveWait			= Address(0x00731590, 0x00000000, 0x00000000, 0x00000000);
		Link_FireGloveAttack		= Address(0x007315A8, 0x00000000, 0x00000000, 0x00000000);
		Link_HammerWait				= Address(0x007315C0, 0x00000000, 0x00000000, 0x00000000);
		Link_HammerAttack			= Address(0x007315D8, 0x00000000, 0x00000000, 0x00000000);
		Link_HammerAttackTotem		= Address(0x007315F0, 0x00000000, 0x00000000, 0x00000000);
		Link_HammerMiss				= Address(0x00731608, 0x00000000, 0x00000000, 0x00000000);
		Link_Die					= Address(0x00731620, 0x00000000, 0x00000000, 0x00000000);
		Link_DieSt					= Address(0x00731638, 0x00000000, 0x00000000, 0x00000000);
		Link_DieEd					= Address(0x00731650, 0x00000000, 0x00000000, 0x00000000);
		Link_Talk					= Address(0x00731668, 0x00000000, 0x00000000, 0x00000000);
		Link_ItemGetA_A				= Address(0x00731680, 0x00000000, 0x00000000, 0x00000000);
		Link_ItemGetA_B				= Address(0x00731698, 0x00000000, 0x00000000, 0x00000000);
		Link_ItemGetShort			= Address(0x007316B0, 0x00000000, 0x00000000, 0x00000000);
		Link_UnlockB				= Address(0x007316C8, 0x00000000, 0x00000000, 0x00000000);
		Link_PDashReady				= Address(0x007316E0, 0x00000000, 0x00000000, 0x00000000);
		Link_PDash					= Address(0x007316F8, 0x00000000, 0x00000000, 0x00000000);
		Link_DashAchichi			= Address(0x00731710, 0x00000000, 0x00000000, 0x00000000);
		Link_PDashCrash				= Address(0x00731728, 0x00000000, 0x00000000, 0x00000000);
		Link_PDashBrake				= Address(0x00731740, 0x00000000, 0x00000000, 0x00000000);
		Link_PDashStagger			= Address(0x00731758, 0x00000000, 0x00000000, 0x00000000);
		Link_WaitPushPull			= Address(0x00731770, 0x00000000, 0x00000000, 0x00000000);
		Link_WalkPush				= Address(0x00731788, 0x00000000, 0x00000000, 0x00000000);
		Link_WalkPull				= Address(0x007317A0, 0x00000000, 0x00000000, 0x00000000);
		Link_Swim					= Address(0x007317B8, 0x00000000, 0x00000000, 0x00000000);
		Link_SwimWait				= Address(0x007317D0, 0x00000000, 0x00000000, 0x00000000);
		Link_SwimDive				= Address(0x007317E8, 0x00000000, 0x00000000, 0x00000000);
		Link_SwimLand				= Address(0x00731800, 0x00000000, 0x00000000, 0x00000000);
		Link_SwimDrift				= Address(0x00731818, 0x00000000, 0x00000000, 0x00000000);
		Link_SwimScrewAttack_A		= Address(0x00731830, 0x00000000, 0x00000000, 0x00000000);
		Link_SwimScrewAttack_B		= Address(0x00731848, 0x00000000, 0x00000000, 0x00000000);
		Link_SwimWaitPuppet			= Address(0x00731860, 0x00000000, 0x00000000, 0x00000000);
		Link_WaitAtoB				= Address(0x00731878, 0x00000000, 0x00000000, 0x00000000);
		Link_WaitB					= Address(0x00731890, 0x00000000, 0x00000000, 0x00000000);
		Link_WaitFight				= Address(0x007318A8, 0x00000000, 0x00000000, 0x00000000);
		Link_WaitPuppet				= Address(0x007318C0, 0x00000000, 0x00000000, 0x00000000);
		Link_WalkPuppet				= Address(0x007318D8, 0x00000000, 0x00000000, 0x00000000);
		Link_Skyward				= Address(0x007318F0, 0x00000000, 0x00000000, 0x00000000);
		Link_JumpSt					= Address(0x00731908, 0x00000000, 0x00000000, 0x00000000);
		Link_JumpAuto				= Address(0x00731920, 0x00000000, 0x00000000, 0x00000000);
		Link_JumpEdS_B				= Address(0x00731938, 0x00000000, 0x00000000, 0x00000000);
		Link_CarryAway				= Address(0x00731950, 0x00000000, 0x00000000, 0x00000000);
		Link_Call					= Address(0x00731968, 0x00000000, 0x00000000, 0x00000000);
		Link_Rock					= Address(0x00731980, 0x00000000, 0x00000000, 0x00000000);
		Link_Sad					= Address(0x00731998, 0x00000000, 0x00000000, 0x00000000);
		Link_SadWait				= Address(0x007319B0, 0x00000000, 0x00000000, 0x00000000);
		Link_WalkMagic				= Address(0x007319C8, 0x00000000, 0x00000000, 0x00000000);
		Link_TurnMagic				= Address(0x007319E0, 0x00000000, 0x00000000, 0x00000000);
		Link_EdYorokeTotem1			= Address(0x007319F8, 0x00000000, 0x00000000, 0x00000000);
		Link_EdWaitTotem1			= Address(0x00731A10, 0x00000000, 0x00000000, 0x00000000);
		Link_EdYorokeTotem2			= Address(0x00731A28, 0x00000000, 0x00000000, 0x00000000);
		Link_EdWaitTotem2			= Address(0x00731A40, 0x00000000, 0x00000000, 0x00000000);
		Link_WaitPinch				= Address(0x00731A58, 0x00000000, 0x00000000, 0x00000000);
	}

	// Camera
	Address		AddressList::DynamicCameraCheck;
	Address		AddressList::GameplayCameraCheck;
	Address		AddressList::GameplayCameraInit;
	Address		AddressList::RetGameplayCameraInit;
	Address		AddressList::CameraLinkFocus;
	Address		AddressList::CameraMode;
	Address		AddressList::CameraPosX;
	Address		AddressList::CameraPosY;
	Address		AddressList::CameraPosZ;
	Address		AddressList::CameraRotationX;
	Address		AddressList::CameraRotationY;
	Address		AddressList::CameraRotationZ;
	Address		AddressList::OrthographicZoom;
	Address		AddressList::PerspectiveZoom;

	// Chaos
	Address		AddressList::HeartsTakeHealth;

	// Costume
	Address		AddressList::CostumeAttrA;
	Address		AddressList::CostumeAttrB;
	Address		AddressList::CostumeAttrC;
	Address		AddressList::CostumeAttrD;
	Address		AddressList::CostumeCatalogPointer;
	Address		AddressList::CurrCostume;
	Address		AddressList::UnusedCostumeDataPointers;
	Address		AddressList::PlayerStatus;
	Address		AddressList::TingleBalloons;
	Address		AddressList::BearMinMaxCheckA;
	Address		AddressList::BearMinMaxCheckB;

	// Emotes
	Address		AddressList::GameplayEmotes;
	Address		AddressList::LobbyEmotes;
	Address		AddressList::EmoteAGraphic;
	Address		AddressList::EmoteBGraphic;
	Address		AddressList::EmoteCGraphic;
	Address		AddressList::EmoteDGraphic;
	Address		AddressList::EmoteEGraphic;
	Address		AddressList::EmoteFGraphic;
	Address		AddressList::EmoteGGraphic;
	Address		AddressList::EmoteHGraphic;
	Address		AddressList::UseDoTLayoutAlways;

	// Game Data
	Address		AddressList::CurrLevelID;
	Address		AddressList::CurrStageID;
	Address		AddressList::CurrSpawnID;
	Address		AddressList::DoppelsEnabled;
	Address		AddressList::DrablandsRupeeCount;
	Address		AddressList::EnemyDataPointer;
	Address		AddressList::HealthCurrent;
	Address		AddressList::HealthMax;
	Address		AddressList::EnergyCurrent;
	Address		AddressList::EnergyMax;
	Address		AddressList::EnergyMaxPitRecovery;
	Address		AddressList::EnergyMaxMultiplicand;
	Address		AddressList::FairiesCurrent;
	Address		AddressList::TimeLeft;
	Address		AddressList::CollisionCurrent;
	Address		AddressList::SpeedX;
	Address		AddressList::SpeedY;
	Address		AddressList::SpeedZ;
	Address		AddressList::TotemCarrying;
	Address		AddressList::TotemCarriedBy;
	Address		AddressList::LoadingStatus;
	Address		AddressList::PauseStatus;
	Address		AddressList::PreviousLevelID;
	Address		AddressList::PreviousStageID;
	Address		AddressList::PreviousSpawnID;
	Address		AddressList::TimeElapsed;
	Address		AddressList::TargetLevelID;
	Address		AddressList::TargetStageID;
	Address		AddressList::TargetSpawnID;
	Address		AddressList::Warp;
	Address 	AddressList::WarpActivation;
	Address		AddressList::WarpPointer;
	Address		AddressList::ChallengeID;

	// Items
	Address		AddressList::BowStrafe;
	Address		AddressList::FireGloveStrafe;
	Address		AddressList::GustStrafe;
	Address		AddressList::HammerStrafe;
	Address		AddressList::ShadowLinkItemPointer;
	Address		AddressList::RoboNiceAtk;
	Address		AddressList::RoboNiceSFX;
	Address		AddressList::RoboNiceSpeed;
	Address		AddressList::GripshotStrafe;
	Address		AddressList::WaterRodStrafe;

	// Miscellaneous
	Address		AddressList::CameraOnX;
	Address		AddressList::CameraShutter;
	Address		AddressList::CheckPhotoExist;
	Address		AddressList::DisplayTopPhoto;
	Address		AddressList::SeeChestContents;
	Address		AddressList::SwordBeamCD;
	Address		AddressList::TextBoxSpeed;

	// Padding
	Address		AddressList::TextToRodata;

	// Player
	Address		AddressList::ActiveLink;
	Address		AddressList::CurrentAnim;
	Address		AddressList::CurrentItem;
	Address		AddressList::IsInvincible;
	Address		AddressList::IsJinxed;
	Address		AddressList::IsSpawned;
	Address		AddressList::IsSwapping;
	Address		AddressList::IsWaterStorage;
	Address		AddressList::IsVisible;
	Address		AddressList::LinkVoice;
	Address		AddressList::LiveNameColorG;
	Address		AddressList::LiveNameColorB;
	Address		AddressList::LiveNameColorR;
	Address		AddressList::LockMovement;
	Address		AddressList::NoCollision;
	Address		AddressList::PlayerModelSize;
	Address		AddressList::PositionX;
	Address		AddressList::PositionY;
	Address		AddressList::PositionZ;
	Address		AddressList::PreviousAnim;
	Address		AddressList::PVPMode;
	Address		AddressList::RespawnPositionX;
	Address		AddressList::RespawnPositionY;
	Address		AddressList::RespawnPositionZ;
	Address		AddressList::SwordType;

	// Rendering
	Address		AddressList::ArrowIndicatorColorTop;
	Address		AddressList::ArrowIndicatorColorMid;
	Address		AddressList::ArrowIndicatorColorBot;
	Address		AddressList::HUDPointer;
	Address		AddressList::FogA;
	Address		AddressList::FogB;
	Address		AddressList::FogC;
	Address		AddressList::ScrollingTextOpacity;
	Address		AddressList::OffScreenLocation;
	Address		AddressList::TextureName;
	Address		AddressList::LoadingScreenSPNoChal;
	Address		AddressList::LoadingScreenSPIcons;
	Address		AddressList::CheerPomPom;
	Address		AddressList::Aura;

	// Savefile
	Address		AddressList::NormalNCCompletion;
	Address		AddressList::NormalC1Completion;
	Address		AddressList::NormalC2Completion;
	Address		AddressList::NormalC3Completion;
	Address		AddressList::SoloNCCompletion;
	Address		AddressList::SoloC1Completion;
	Address		AddressList::SoloC2Completion;
	Address		AddressList::SoloC3Completion;
	Address		AddressList::MultiNCCompletion;
	Address		AddressList::MultiC1Completion;
	Address		AddressList::MultiC2Completion;
	Address		AddressList::MultiC3Completion;

	Address		AddressList::CostumeObtainStatus;
	Address		AddressList::ColiseumWinCount;
	Address		AddressList::EditMerchantStock;
	Address		AddressList::HeroPointCount;
	Address		AddressList::MainVoice;
	Address		AddressList::ResetMerchantStock;

	// SFX and BGM
	Address		AddressList::BGMType;
	Address		AddressList::BGMVolume;
	Address		AddressList::LinkVoiceSpeed;
	Address		AddressList::Voice8bit;
	Address		AddressList::LobbyBallPointer;
	Address		AddressList::MasterVolume;

	// Turbo Mode
	Address		AddressList::DashTimerNormal;
	Address		AddressList::DashTimerNinja;
	Address		AddressList::SwordBeamCDExe;
	Address		AddressList::HammerwearSpeed;
	Address		AddressList::SL_SwordSpeed;
	Address		AddressList::SL_Strafe;
	Address		AddressList::SL_RunAnimSpeed;
	Address		AddressList::SL_RunSpeed;
	Address		AddressList::BombFlashSpeed;
	Address		AddressList::LoadingIconAnimSpeed;
	Address		AddressList::LiveMsgSpeed;
	Address		AddressList::SwordBeamSpeed;
	Address		AddressList::SpeedDamageUndo;
	Address		AddressList::BGMSFXSpeed;
	Address		AddressList::EnergyBarStretch;
	Address		AddressList::SpeedMultiplierNormal;
	Address		AddressList::SpeedMultiplierCheetah;
	Address		AddressList::DashStaggerSpeed;
	Address		AddressList::DashSpeed;
	Address		AddressList::DashBrakeDuration;
	Address		AddressList::DashPlayerCrashHeight;
	Address		AddressList::DashWallCrashHeight;
	Address		AddressList::DashCrashBackwards;
	Address		AddressList::FireJumpHeight;
	Address		AddressList::LavaBounceHeight;
	Address		AddressList::WaterJumpHeight;
	Address		AddressList::WaterJumpForwardSpeed;
	Address		AddressList::SwimBoostSpeedZora;
	Address		AddressList::SwimBoostSpeedNormal;
	Address		AddressList::SwimTurnaroundSpeed;
	Address		AddressList::SwimSpeed;
	Address		AddressList::ArrowSpeedPlayer;
	Address		AddressList::FireballHeight;
	Address		AddressList::FireballSpeed;
	Address		AddressList::FireballBounceHeight;
	Address		AddressList::FireballThrowHeight;
	Address		AddressList::FireballReleaseTime;
	Address		AddressList::BoomerangReturnSpeed;
	Address		AddressList::BoomerangDistance;
	Address		AddressList::BoomerangSpeed;
	Address		AddressList::GustSpeed;
	Address		AddressList::GripshotRetractSpeed;
	Address		AddressList::GripshotExtendSpeed;

	// Animation Speeds
	Address		AddressList::Link_Wait;
	Address		AddressList::Link_WalkSlow;
	Address		AddressList::Link_Walk;
	Address		AddressList::Link_Dash;
	Address		AddressList::Link_DashTotem;
	Address		AddressList::Link_GrabDash;
	Address		AddressList::Link_CutRF;
	Address		AddressList::Link_CutRFTotem;
	Address		AddressList::Link_CutRFtoKeep;
	Address		AddressList::Link_KeepWait;
	Address		AddressList::Link_Compete;
	Address		AddressList::Link_CutTurn;
	Address		AddressList::Link_CutTurnW;
	Address		AddressList::Link_CutTurnB;
	Address		AddressList::Link_GrabP_A;
	Address		AddressList::Link_GrabP_B;
	Address		AddressList::Link_GrabUp;
	Address		AddressList::Link_GrabThrow;
	Address		AddressList::Link_GrabWait;
	Address		AddressList::Link_WaitTotem;
	Address		AddressList::Link_GrabYorokeTotem;
	Address		AddressList::Link_YorokeTotem;
	Address		AddressList::Link_WaitYoroke;
	Address		AddressList::Link_JumpEdS_A;
	Address		AddressList::Link_DamF;
	Address		AddressList::Link_DamB;
	Address		AddressList::Link_FallLava;
	Address		AddressList::Link_FallSand;
	Address		AddressList::Link_Drown;
	Address		AddressList::Link_DrownLava;
	Address		AddressList::Link_Fall;
	Address		AddressList::Link_DamBiri;
	Address		AddressList::Link_WaitQ;
	Address		AddressList::Link_DamBoyon;
	Address		AddressList::Link_DamFFup;
	Address		AddressList::Link_DamSwimF;
	Address		AddressList::Link_DamSwimB;
	Address		AddressList::Link_LLhold;
	Address		AddressList::Link_LLswallowSt;
	Address		AddressList::Link_LLstruggle;
	Address		AddressList::Link_FallTotem;
	Address		AddressList::Link_FallBallon;
	Address		AddressList::Link_BowWalkA;
	Address		AddressList::Link_BowWalkB;
	Address		AddressList::Link_Slipice;
	Address		AddressList::Link_BowWait;
	Address		AddressList::Link_ArrowShoot;
	Address		AddressList::Link_RodWait;
	Address		AddressList::Link_RodAttack;
	Address		AddressList::Link_BoomAttack;
	Address		AddressList::Link_BoomCarried;
	Address		AddressList::Link_HookshotWait;
	Address		AddressList::Link_HookshotAttack;
	Address		AddressList::Link_HookshotDrag;
	Address		AddressList::Link_Bump;
	Address		AddressList::Link_HookshotPullSt;
	Address		AddressList::Link_HookshotPullEd;
	Address		AddressList::Link_AirCannonWait;
	Address		AddressList::Link_AirCannonAttack;
	Address		AddressList::Link_FireGloveWait;
	Address		AddressList::Link_FireGloveAttack;
	Address		AddressList::Link_HammerWait;
	Address		AddressList::Link_HammerAttack;
	Address		AddressList::Link_HammerAttackTotem;
	Address		AddressList::Link_HammerMiss;
	Address		AddressList::Link_Die;
	Address		AddressList::Link_DieSt;
	Address		AddressList::Link_DieEd;
	Address		AddressList::Link_Talk;
	Address		AddressList::Link_ItemGetA_A;
	Address		AddressList::Link_ItemGetA_B;
	Address		AddressList::Link_ItemGetShort;
	Address		AddressList::Link_UnlockB;
	Address		AddressList::Link_PDashReady;
	Address		AddressList::Link_PDash;
	Address		AddressList::Link_DashAchichi;
	Address		AddressList::Link_PDashCrash;
	Address		AddressList::Link_PDashBrake;
	Address		AddressList::Link_PDashStagger;
	Address		AddressList::Link_WaitPushPull;
	Address		AddressList::Link_WalkPush;
	Address		AddressList::Link_WalkPull;
	Address		AddressList::Link_Swim;
	Address		AddressList::Link_SwimWait;
	Address		AddressList::Link_SwimDive;
	Address		AddressList::Link_SwimLand;
	Address		AddressList::Link_SwimDrift;
	Address		AddressList::Link_SwimScrewAttack_A;
	Address		AddressList::Link_SwimScrewAttack_B;
	Address		AddressList::Link_SwimWaitPuppet;
	Address		AddressList::Link_WaitAtoB;
	Address		AddressList::Link_WaitB;
	Address		AddressList::Link_WaitFight;
	Address		AddressList::Link_WaitPuppet;
	Address		AddressList::Link_WalkPuppet;
	Address		AddressList::Link_Skyward;
	Address		AddressList::Link_JumpSt;
	Address		AddressList::Link_JumpAuto;
	Address		AddressList::Link_JumpEdS_B;
	Address		AddressList::Link_CarryAway;
	Address		AddressList::Link_Call;
	Address		AddressList::Link_Rock;
	Address		AddressList::Link_Sad;
	Address		AddressList::Link_SadWait;
	Address		AddressList::Link_WalkMagic;
	Address		AddressList::Link_TurnMagic;
	Address		AddressList::Link_EdYorokeTotem1;
	Address		AddressList::Link_EdWaitTotem1;
	Address		AddressList::Link_EdYorokeTotem2;
	Address		AddressList::Link_EdWaitTotem2;
	Address		AddressList::Link_WaitPinch;
}
