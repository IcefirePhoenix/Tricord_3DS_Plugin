#ifndef ADDRESSLIST_HPP
#define ADDRESSLIST_HPP

#include <CTRPluginFramework.hpp>
#include <Helpers/Address.hpp>

namespace CTRPluginFramework {
	class AddressList {
	public:
		static void     InitAddresses(void);

		// Camera
		static Address		DynamicCameraCheck;
		static Address		GameplayCameraCheck;
		static Address		GameplayCameraInit;
		static Address		RetGameplayCameraInit;
		static Address		CameraLinkFocus;
		static Address		CameraMode;
		static Address		CameraPosX;
		static Address		CameraPosY;
		static Address		CameraPosZ;
		static Address		CameraRotationX;
		static Address		CameraRotationY;
		static Address		CameraRotationZ;
		static Address		OrthographicZoom;
		static Address		PerspectiveZoom;

		// Chaos
		static Address		HeartsTakeHealth;

		// Costume
		static Address		CostumeAttrA;
		static Address		CostumeAttrB;
		static Address		CostumeAttrC;
		static Address		CurrCostume;
		static Address		CostumeCatalogPointer;
		static Address		PlayerStatus;
		static Address		TingleBalloons;
		static Address		UnusedCostumeDataPointerList;

		// Emotes
		static Address		GameplayEmotes;
		static Address		LobbyEmotes;
		static Address		EmoteAGraphic;
		static Address		EmoteBGraphic;
		static Address		EmoteCGraphic;
		static Address		EmoteDGraphic;
		static Address		EmoteEGraphic;
		static Address		EmoteFGraphic;
		static Address		EmoteGGraphic;
		static Address		EmoteHGraphic;
		static Address		UseDoTLayoutAlways;

		// Game Data
		static Address		CurrLevelID;
		static Address		CurrStageID;
		static Address		CurrSpawnID;
		static Address		DoppelsEnabled;
		static Address		DrablandsRupeeCount;
		static Address		HealthCurrent;
		static Address		HealthMax;
		static Address		LoadingStatus;
		static Address		PauseStatus;
		static Address		PreviousLevelID;
		static Address		PreviousStageID;
		static Address		PreviousSpawnID;
		static Address		TimeElapsed;

		// Items
		static Address		BowStrafe;
		static Address		FireGloveStrafe;
		static Address		GustStrafe;
		static Address		HammerStrafe;
		static Address		ShadowLinkItemPointer;
		static Address		RoboNiceAtk;
		static Address		RoboNiceSFX;
		static Address		RoboNiceSpeed;
		static Address		RoboStrafe;
		static Address		WaterRodStrafe;

		// Miscellaneous
		static Address		CheckPhotoExist;
		static Address		DisplayTopPhoto;
		static Address		LobbyBallCounter;
		static Address		LobbyBallSong;
		static Address		SeeChestContents;
		static Address		SwordBeamCD;
		static Address		TextBoxSpeed;

		// Padding
		static Address		TextToRodata; // ends 0x6AA000 for NA

		// Player
		static Address		ActiveLink;
		static Address		CurrentItem;
		static Address		Energy;
		static Address		EnergyMax;
		static Address		EnergyMultiplier;
		static Address		IsInvincible;
		static Address		IsJinxed;
		static Address		IsSpawned;
		static Address		IsSwapping;
		static Address		IsWaterStorage;
		static Address		IsVisible;
		static Address		LinkVoice;
		static Address		LiveNameColorG;
		static Address		LiveNameColorB;
		static Address		LiveNameColorR;
		static Address		LockMovement;
		static Address		NoCollision;
		static Address		PlayerModelSize;
		static Address		PositionX;
		static Address		PositionY;
		static Address		PositionZ;
		static Address		PVPMode;
		static Address		RespawnPositionX;
		static Address		RespawnPositionY;
		static Address		RespawnPositionZ;
		static Address		SwordType;

		// Rendering
		static Address		ArrowIndicatorColorBot;
		static Address		ArrowIndicatorColorMid;
		static Address		ArrowIndicatorColorTop;
		static Address		ArrowIndicatorLocationBot;
		static Address		ArrowIndicatorLocationMid;
		static Address		ArrowIndicatorLocationTop;
		static Address		ArrowIndicatorVisibilityBot;
		static Address		ArrowIndicatorVisibilityMid;
		static Address		ArrowIndicatorVisibilityTop;
		static Address		HUDPointer;
		static Address		FogA;
		static Address		FogB;
		static Address		FogC;
		static Address		ScrollingTextOpacity;
		static Address		OffScreenLocation;


		// Savefile
		static Address		NormalNCCompletion;
		static Address		NormalC1Completion;
		static Address		NormalC2Completion;
		static Address		NormalC3Completion;
		static Address		SoloNCCompletion;
		static Address		SoloC1Completion;
		static Address		SoloC2Completion;
		static Address		SoloC3Completion;
		static Address		MultiNCCompletion;
		static Address		MultiC1Completion;
		static Address		MultiC2Completion;
		static Address		MultiC3Completion;

		static Address		CostumeObtainStatus;
		static Address		ColiseumWinCount;
		static Address		EditMerchantStock;
		static Address		HeroPointCount;
		static Address		MainVoice;
		static Address		ResetMerchantStock;

		// SFX and BGM
		static Address		BGMType;
		static Address		BGMVolume;
		static Address		LinkVoiceSpeed;
		static Address		MasterVolume;
		
	};
}

#endif