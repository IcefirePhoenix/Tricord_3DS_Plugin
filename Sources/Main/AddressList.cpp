#include "Helpers/DataHelpers/Address.hpp"
#include "AddressList.hpp"

namespace CTRPluginFramework {

	void AddressList::InitAddresses(void) {
		/* Camera */ 
		DynamicCameraCheck 			= Address(0x00143810, 0x00000000, 0x00000000, 0x00000000);
		GameplayCameraCheck 		= Address(0x00143A7C, 0x00000000, 0x00000000, 0x00000000);
		GameplayCameraInit 			= Address(0x00143A80, 0x00000000, 0x00000000, 0x00000000);
		RetGameplayCameraInit 		= Address(0x00143B80, 0x00000000, 0x00000000, 0x00000000);

		CameraLinkFocus 			= Address(0x32C815D0, 0x32C81650, 0x00000000, 0x00000000);
		CameraMode 					= Address(0x32C814BC, 0x32C8153C, 0x00000000, 0x00000000);
		CameraPosX 					= Address(0x32C814C0, 0x32C81540, 0x00000000, 0x00000000);
		CameraPosY 					= Address(0x32C814C4, 0x32C81544, 0x00000000, 0x00000000);
		CameraPosZ 					= Address(0x32C814C8, 0x32C81548, 0x00000000, 0x00000000);
		CameraRotationX 			= Address(0x32C81520, 0x32C815A0, 0x00000000, 0x00000000);
		CameraRotationZ 			= Address(0x32C81524, 0x32C815A4, 0x00000000, 0x00000000);
		OrthographicZoom 			= Address(0x32C81528, 0x32C815A8, 0x00000000, 0x00000000);
		PerspectiveZoom 			= Address(0x005F529C, 0x00000000, 0x00000000, 0x00000000);

		/* Chaos */ 
		HeartsTakeHealth 			= Address(0x0015CA04, 0x0015CA20, 0x00000000, 0x00000000);

		/* Costume */ 
		CostumeAttrA 				= Address(0x32849510, 0x00000000, 0x00000000, 0x00000000);
		CostumeAttrB 				= Address(0x32849512, 0x00000000, 0x00000000, 0x00000000);
		CostumeAttrC 				= Address(0x32849513, 0x00000000, 0x00000000, 0x00000000);
		CurrCostume 				= Address(0x32849564, 0x328495E4, 0x00000000, 0x00000000);
		CostumeCatalogPointer 		= Address(0x32C828E8, 0x32C82968, 0x00000000, 0x00000000);
		PlayerStatus 				= Address(0x32849511, 0x00000000, 0x00000000, 0x00000000);
		TingleBalloons 				= Address(0x32849680, 0x32849700, 0x00000000, 0x00000000);
		UnusedCostumeDataPointers 	= Address(0x0075C390, 0x00000000, 0x00000000, 0x00000000);

		/* Console */ 

		/* Emotes */ 
		GameplayEmotes 				= Address(0x006B6E98, 0x006B6E98, 0x00000000, 0x00000000);
		LobbyEmotes 				= Address(0x006B6EA0, 0x006B6EA0, 0x00000000, 0x00000000);
		EmoteAGraphic 				= Address(0x32C91830, 0x32C918B0, 0x00000000, 0x00000000);
		EmoteBGraphic 				= Address(0x32C9225C, 0x32C922DC, 0x00000000, 0x00000000);
		EmoteCGraphic 				= Address(0x32C92BEC, 0x32C92C6C, 0x00000000, 0x00000000);
		EmoteDGraphic 				= Address(0x32C9357C, 0x32C935FC, 0x00000000, 0x00000000);
		EmoteEGraphic 				= Address(0x32C93F0C, 0x32C93F8C, 0x00000000, 0x00000000);
		EmoteFGraphic 				= Address(0x32C9489C, 0x32C9491C, 0x00000000, 0x00000000);
		EmoteGGraphic 				= Address(0x32C9522C, 0x32C952AC, 0x00000000, 0x00000000);
		EmoteHGraphic 				= Address(0x32C95BBC, 0x32C95C3C, 0x00000000, 0x00000000);
		UseDoTLayoutAlways 			= Address(0x002F0C6C, 0x002F1010, 0x00000000, 0x00000000);

		/* Game Data */ 
		CurrLevelID 				= Address(0x3280EDC4, 0x3280EE44, 0x00000000, 0x00000000);
		CurrStageID 				= Address(0x3280EDC8, 0x3280EE48, 0x00000000, 0x00000000);
		CurrSpawnID 				= Address(0x3280EDCA, 0x3280EE4C, 0x00000000, 0x00000000);
		DoppelsEnabled 				= Address(0x32D81E46, 0x32D81EC6, 0x00000000, 0x00000000);
		DrablandsRupeeCount 		= Address(0x32C8178C, 0x32C8180C, 0x00000000, 0x00000000);
		EnemyDataPointer			= Address(0x32989188, 0x00000000, 0x00000000, 0x00000000);
		HealthCurrent 				= Address(0x32C824F4, 0x32C82574, 0x00000000, 0x00000000);
		HealthMax 					= Address(0x32C824F5, 0x32C82575, 0x00000000, 0x00000000);
		LoadingStatus 				= Address(0x319FE1D0, 0x00000000, 0x00000000, 0x00000000);
		PauseStatus 				= Address(0x3280ED8C, 0x3280EE0C, 0x00000000, 0x00000000);
		PreviousLevelID				= Address(0x3280EDBC, 0x3280EE3C, 0x00000000, 0x00000000);
		PreviousStageID				= Address(0x3280EDC0, 0x3280EE40, 0x00000000, 0x00000000);
		PreviousSpawnID 			= Address(0x3280EDC2, 0x3280EE42, 0x00000000, 0x00000000);
		TimeElapsed 				= Address(0x3280EDE0, 0x3280EE60, 0x00000000, 0x00000000);

		/* Items */ 
		BowStrafe 					= Address(0x0038CC7C, 0x0038D024, 0x00000000, 0x00000000);
		FireGloveStrafe 			= Address(0x003C8FD8, 0x003C9380, 0x00000000, 0x00000000);
		GustStrafe 					= Address(0x003C85A8, 0x003C8950, 0x00000000, 0x00000000);
		HammerStrafe 				= Address(0x003C03E0, 0x003C0788, 0x00000000, 0x00000000);
		ShadowLinkItemPointer 		= Address(0x32D81DEC, 0x32D81E6C, 0x00000000, 0x00000000);
		RoboNiceAtk 				= Address(0x00156AB4, 0x00000000, 0x00000000, 0x00000000);
		RoboNiceSFX 				= Address(0x003C2030, 0x00000000, 0x00000000, 0x00000000);
		RoboNiceSpeed 				= Address(0x00156898, 0x00000000, 0x00000000, 0x00000000);
		RoboStrafe 					= Address(0x003C214C, 0x003C24F4, 0x00000000, 0x00000000);
		WaterRodStrafe 				= Address(0x003C59C0, 0x003C5D68, 0x00000000, 0x00000000);

		/* Miscellaneous */ 
		CameraOnX 					= Address(0x32D92920, 0x32D929A0, 0x00000000, 0x00000000);
		CameraShutter 				= Address(0x32D92E9C, 0x32D92F1C, 0x00000000, 0x00000000);		
		CheckPhotoExist 			= Address(0x31B2FE3D, 0x00000000, 0x00000000, 0x00000000);
		DisplayTopPhoto 			= Address(0x31B2FE21, 0x31B2FEA1, 0x00000000, 0x00000000);
		SeeChestContents 			= Address(0x0022FFB8, 0x00230040, 0x00000000, 0x00000000);
		SwordBeamCD 				= Address(0x328495A0, 0x32849620, 0x00000000, 0x00000000);
		TextBoxSpeed 				= Address(0x0018122C, 0x001812BC, 0x00000000, 0x00000000);

		/* Padding */ 
		TextToRodata 				= Address(0x006A9168, 0x006A9598, 0x00000000, 0x00000000);

		/* Player */ 
		ActiveLink 					= Address(0x31A03879, 0x31A038D1, 0x00000000, 0x00000000);
		CurrentAnim 				= Address(0x32849188, 0x32849208, 0x00000000, 0x00000000);
		CurrentItem 				= Address(0x32841A20, 0x32841AA0, 0x00000000, 0x00000000);
		Energy 						= Address(0x328496C4, 0x32849744, 0x00000000, 0x00000000);
		EnergyMax 					= Address(0x328496C5, 0x32849745, 0x00000000, 0x00000000);
		EnergyMultiplier 			= Address(0x328496CC, 0x00000000, 0x00000000, 0x00000000);
		//check if jinxed is model or effects also
		IsInvincible 				= Address(0x32849544, 0x328495C4, 0x00000000, 0x00000000);
		IsJinxed 					= Address(0x328495AC, 0x3284962C, 0x00000000, 0x00000000);
		IsSpawned 					= Address(0x32841410, 0x32841490, 0x00000000, 0x00000000);
		IsSwapping 					= Address(0x31A04610, 0x00000000, 0x00000000, 0x00000000);
		IsWaterStorage				= Address(0x32849231, 0x328492B1, 0x00000000, 0x00000000);
		IsVisible 					= Address(0x32849550, 0x328495D0, 0x00000000, 0x00000000);
		LinkVoice 					= Address(0x3284A260, 0x3284A2E0, 0x00000000, 0x00000000);
		LiveNameColorG 				= Address(0x007767F0, 0x007767F8, 0x00000000, 0x00000000);
		LiveNameColorB 				= Address(0x007767F4, 0x007767FC, 0x00000000, 0x00000000);
		LiveNameColorR 				= Address(0x007767F8, 0x00776800, 0x00000000, 0x00000000);
		LockMovement 				= Address(0x31A0115B, 0x31A011B3, 0x00000000, 0x00000000);
		NoCollision 				= Address(0x32849508, 0x32849588, 0x00000000, 0x00000000);
		PlayerModelSize 			= Address(0x001FAC64, 0x001FACF4, 0x00000000, 0x00000000);
		PositionX 					= Address(0x328412D8, 0x32841358, 0x00000000, 0x00000000);
		PositionY 					= Address(0x328412DC, 0x3284135C, 0x00000000, 0x00000000);
		PositionZ 					= Address(0x328412E0, 0x32841360, 0x00000000, 0x00000000);
		PreviousAnim 				= Address(0x32849189, 0x32849209, 0x00000000, 0x00000000);
		PVPMode 					= Address(0x32849545, 0x328495C5, 0x00000000, 0x00000000);
		RespawnPositionX 			= Address(0x32841750, 0x328417D0, 0x00000000, 0x00000000);
		RespawnPositionY 			= Address(0x32841754, 0x328417D4, 0x00000000, 0x00000000);
		RespawnPositionZ 			= Address(0x32841758, 0x328417D8, 0x00000000, 0x00000000);
		SwordType 					= Address(0x328419EC, 0x32841A6C, 0x00000000, 0x00000000);

		/* Rendering */ 
		ArrowIndicatorColorBot 		= Address(0x32D91B24, 0x00000000, 0x00000000, 0x00000000);
		ArrowIndicatorColorMid 		= Address(0x32D91A04, 0x00000000, 0x00000000, 0x00000000);
		ArrowIndicatorColorTop 		= Address(0x32D918E4, 0x00000000, 0x00000000, 0x00000000);
		HUDPointer 					= Address(0x32C8289C, 0x00000000, 0x00000000, 0x00000000);
		FogA 						= Address(0x31B31BE4, 0x00000000, 0x00000000, 0x00000000);
		FogB 						= Address(0x31B31BDC, 0x00000000, 0x00000000, 0x00000000);
		FogC 						= Address(0x31B31BE0, 0x00000000, 0x00000000, 0x00000000);
		ScrollingTextOpacity 		= Address(0x00151C40, 0x00000000, 0x00000000, 0x00000000);
		OffScreenLocation 			= Address(0x32D83094, 0x00000000, 0x00000000, 0x00000000);

		/* Savefile */ 
		NormalNCCompletion 			= Address(0x31A060EC, 0x00000000, 0x00000000, 0x00000000);
		NormalC1Completion 			= Address(0x31A060F4, 0x00000000, 0x00000000, 0x00000000);
		NormalC2Completion 			= Address(0x31A060FC, 0x00000000, 0x00000000, 0x00000000);
		NormalC3Completion 			= Address(0x31A06104, 0x00000000, 0x00000000, 0x00000000);
		SoloNCCompletion 			= Address(0x31A0616C, 0x00000000, 0x00000000, 0x00000000);
		SoloC1Completion 			= Address(0x31A06174, 0x00000000, 0x00000000, 0x00000000);
		SoloC2Completion 			= Address(0x31A0617C, 0x00000000, 0x00000000, 0x00000000);
		SoloC3Completion 			= Address(0x31A06184, 0x00000000, 0x00000000, 0x00000000);
		MultiNCCompletion 			= Address(0x31A0612C, 0x00000000, 0x00000000, 0x00000000);
		MultiC1Completion 			= Address(0x31A06134, 0x00000000, 0x00000000, 0x00000000);
		MultiC2Completion 			= Address(0x31A0613C, 0x00000000, 0x00000000, 0x00000000);
		MultiC3Completion 			= Address(0x31A06144, 0x00000000, 0x00000000, 0x00000000);

		CostumeObtainStatus 		= Address(0x32C8256A, 0x00000000, 0x00000000, 0x00000000);
		ColiseumWinCount 			= Address(0x31A062AC, 0x00000000, 0x00000000, 0x00000000);
		EditMerchantStock 			= Address(0x31A077A4, 0x00000000, 0x00000000, 0x00000000);
		HeroPointCount 				= Address(0x31A0611C, 0x00000000, 0x00000000, 0x00000000);
		MainVoice 					= Address(0x31A06118, 0x00000000, 0x00000000, 0x00000000);
		ResetMerchantStock 			= Address(0x31A077AE, 0x00000000, 0x00000000, 0x00000000);

		// SFX and BGM
		BGMType 					= Address(0x005FD3C0, 0x00000000, 0x00000000, 0x00000000);
		BGMVolume 					= Address(0x00236444, 0x00000000, 0x00000000, 0x00000000);
		LobbyBallPointer			= Address(0x32DCAA60, 0x00000000, 0x00000000, 0x00000000);
		LinkVoiceSpeed 				= Address(0x3284A25C, 0x00000000, 0x00000000, 0x00000000);
		MasterVolume 				= Address(0x0050BC74, 0x00000000, 0x00000000, 0x00000000);
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
	Address		AddressList::CostumeCatalogPointer;
	Address		AddressList::CurrCostume;
	Address		AddressList::UnusedCostumeDataPointers;
	Address		AddressList::PlayerStatus;
	Address		AddressList::TingleBalloons;

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
	Address		AddressList::LoadingStatus;
	Address		AddressList::PauseStatus;
	Address		AddressList::PreviousLevelID;
	Address		AddressList::PreviousStageID;
	Address		AddressList::PreviousSpawnID;	
	Address		AddressList::TimeElapsed;

	// Items
	Address		AddressList::BowStrafe;
	Address		AddressList::FireGloveStrafe;
	Address		AddressList::GustStrafe;
	Address		AddressList::HammerStrafe;
	Address		AddressList::ShadowLinkItemPointer;
	Address		AddressList::RoboNiceAtk;
	Address		AddressList::RoboNiceSFX;
	Address		AddressList::RoboNiceSpeed;
	Address		AddressList::RoboStrafe;
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
	Address		AddressList::Energy;
	Address		AddressList::EnergyMax;
	Address		AddressList::EnergyMultiplier;
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
	Address		AddressList::LobbyBallPointer;
	Address		AddressList::MasterVolume;
}