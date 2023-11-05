#include "Helpers/Address.hpp"
#include "AddressList.hpp"


namespace CTRPluginFramework {
	void AddressList::InitAddresses(void) {

		// Camera
		CameraLinkFocus = Address(0x32C815D0, 0x00000000, 0x00000000, 0x00000000);
		CameraMode = Address(0x32C814BC, 0x00000000, 0x00000000, 0x00000000);
		CameraPosX = Address(0x32C814C0, 0x00000000, 0x00000000, 0x00000000);
		CameraPosY = Address(0x32C814C4, 0x00000000, 0x00000000, 0x00000000);
		CameraPosZ = Address(0x32C814C8, 0x00000000, 0x00000000, 0x00000000);
		CameraRotationX = Address(0x32C81520, 0x00000000, 0x00000000, 0x00000000);
		CameraRotationY = Address(0x32C81524, 0x00000000, 0x00000000, 0x00000000);
		CameraRotationZ = Address(0x32C81528, 0x00000000, 0x00000000, 0x00000000);
		MasterZoom = Address(0x005F529C, 0x00000000, 0x00000000, 0x00000000);

		// Chaos
		HeartsTakeHealth = Address(0x0015CA04, 0x00000000, 0x00000000, 0x00000000);

		// Costume
		CostumeAttrA = Address(0x32849510, 0x00000000, 0x00000000, 0x00000000);
		CostumeAttrB = Address(0x32849512, 0x00000000, 0x00000000, 0x00000000);
		CostumeAttrC = Address(0x32849513, 0x00000000, 0x00000000, 0x00000000);
		CurrCostume = Address(0x32849564, 0x00000000, 0x00000000, 0x00000000);
		CostumeCatalogPointer = Address(0x32C828E8, 0x00000000, 0x00000000, 0x00000000);
		// CurrCostumeG = Address(0x3284)
		PlayerStatus = Address(0x32849511, 0x00000000, 0x00000000, 0x00000000);
		TingleBalloons = Address(0x32849680, 0x00000000, 0x00000000, 0x00000000);
		UnusedCostumeDataPointerList = Address(0x0075C390, 0x00000000, 0x00000000, 0x00000000);

		// Console

		// Emotes
		DoTEmotes = Address(0x006B6E98, 0x00000000, 0x00000000, 0x00000000);
		LobbyEmotes = Address(0x006B6EA0, 0x00000000, 0x00000000, 0x00000000);

		// Game Data
		CurrLevelID = Address(0x3280EDC4, 0x00000000, 0x00000000, 0x00000000);
		CurrStageID = Address(0x3280EDC8, 0x00000000, 0x00000000, 0x00000000);
		CurrSpawnID = Address(0x3280EDCA, 0x00000000, 0x00000000, 0x00000000);
		DoppelsEnabled = Address(0x32D81E46, 0x00000000, 0x00000000, 0x00000000);
		DrablandsRupeeCount = Address(0x32C8178C, 0x00000000, 0x00000000, 0x00000000);
		HealthCurrent = Address(0x32C824F4, 0x00000000, 0x00000000, 0x00000000);
		HealthMax = Address(0x32C824F5, 0x00000000, 0x00000000, 0x00000000);
		LoadingStatus = Address(0x3280EDE8, 0x00000000, 0x00000000, 0x00000000);
		PreviousLevelID = Address(0x3280EDBC, 0x00000000, 0x00000000, 0x00000000);
		PreviousStageID = Address(0x3280EDC0, 0x00000000, 0x00000000, 0x00000000);
		PreviousSpawnID = Address(0x3280EDC2, 0x00000000, 0x00000000, 0x00000000);
		TimeElasped = Address(0x3280EDE0, 0x00000000, 0x00000000, 0x00000000);

		// Items
		// Note: Strafe refers to speed
		BowStrafe = Address(0x0038CC7C, 0x00000000, 0x00000000, 0x00000000);
		FireGloveStrafe = Address(0x300C8FD8, 0x00000000, 0x00000000, 0x00000000);
		GustStrafe = Address(0x003C85A8, 0x00000000, 0x00000000, 0x00000000);
		HammerStrafe = Address(0x003C03E0, 0x00000000, 0x00000000, 0x00000000);
		RoboNiceAtk = Address(0x00156AB4, 0x00000000, 0x00000000, 0x00000000);
		RoboNiceSFX = Address(0x003C2030, 0x00000000, 0x00000000, 0x00000000);
		RoboNiceSpeed = Address(0x00156898, 0x00000000, 0x00000000, 0x00000000);
		RoboStrafe = Address(0x003C214C, 0x00000000, 0x00000000, 0x00000000);
		WaterRodStrafe = Address(0x300C59C0, 0x00000000, 0x00000000, 0x00000000);

		// Miscellaneous
		DisplayTopPhoto = Address(0x31B2FE21, 0x00000000, 0x00000000, 0x00000000);
		LobbyBallCounter = Address(0x329F18F4, 0x00000000, 0x00000000, 0x00000000);
		LobbyBallSong = Address(0x329F193C, 0x00000000, 0x00000000, 0x00000000);
		SeeChestContents = Address(0x0022FFB8, 0x00000000, 0x00000000, 0x00000000);
		TextBoxSpeed = Address(0x0018122C, 0x00000000, 0x00000000, 0x00000000);

		// Padding
		TextToRodata = Address(0x006A9168, 0x00000000, 0x00000000, 0x00000000);

		// Player
		ActiveLink = Address(0x31A03879, 0x00000000, 0x00000000, 0x00000000);
		CurrentItem = Address(0x32841A20, 0x00000000, 0x00000000, 0x00000000);
		Energy = Address(0x328496C4, 0x00000000, 0x00000000, 0x00000000);
		EnergyMax = Address(0x328496C5, 0x00000000, 0x00000000, 0x00000000);
		EnergyMultiplier = Address(0x328496CC, 0x00000000, 0x00000000, 0x00000000);
		//check if jinxed is model or effects also
		IsInvincible = Address(0x32849544, 0x00000000, 0x00000000, 0x00000000);
		IsJinxed = Address(0x328495AC, 0x00000000, 0x00000000, 0x00000000);
		IsSpawned = Address(0x32841410, 0x00000000, 0x00000000, 0x00000000);
		IsSwapping = Address(0x31A04610, 0x00000000, 0x00000000, 0x00000000);
		IsWaterStorage = Address(0x32849231, 0x00000000, 0x00000000, 0x00000000);
		IsVisible = Address(0x32849550, 0x00000000, 0x00000000, 0x00000000);
		LinkVoice = Address(0x3284A260, 0x00000000, 0x00000000, 0x00000000);
		LiveNameColorG = Address(0x007767F0, 0x00000000, 0x00000000, 0x00000000);
		LiveNameColorB = Address(0x007767F4, 0x00000000, 0x00000000, 0x00000000);
		LiveNameColorR = Address(0x007767F8, 0x00000000, 0x00000000, 0x00000000);
		LockMovement = Address(0x31A0115B, 0x00000000, 0x00000000, 0x00000000);
		NoCollision = Address(0x32849508, 0x00000000, 0x00000000, 0x00000000);
		PlayerModelSize = Address(0x1FAC64, 0x00000000, 0x00000000, 0x00000000);
		PositionX = Address(0x328412D8, 0x00000000, 0x00000000, 0x00000000);
		PositionY = Address(0x328412DC, 0x00000000, 0x00000000, 0x00000000);
		PositionZ = Address(0x328412E0, 0x00000000, 0x00000000, 0x00000000);
		PVPMode = Address(0x32849545, 0x00000000, 0x00000000, 0x00000000);
		RespawnPositionX = Address(0x32841750, 0x00000000, 0x00000000, 0x00000000);
		RespawnPositionY = Address(0x32841754, 0x00000000, 0x00000000, 0x00000000);
		RespawnPositionZ = Address(0x32841758, 0x00000000, 0x00000000, 0x00000000);
		SwordType = Address(0x328419EC, 0x00000000, 0x00000000, 0x00000000);

		// Rendering
		ArrowIndicatorColorBot = Address(0x32D91B24, 0x00000000, 0x00000000, 0x00000000);
		ArrowIndicatorColorMid = Address(0x32D91A04, 0x00000000, 0x00000000, 0x00000000);
		ArrowIndicatorColorTop = Address(0x32D918E4, 0x00000000, 0x00000000, 0x00000000);
		ArrowIndicatorLocationBot = Address(0x32D91AE8, 0x00000000, 0x00000000, 0x00000000);
		ArrowIndicatorLocationMid = Address(0x32D919C8, 0x00000000, 0x00000000, 0x00000000);
		ArrowIndicatorLocationTop = Address(0x32D918A8, 0x00000000, 0x00000000, 0x00000000);
		ArrowIndicatorVisibilityBot = Address(0x32D91B28, 0x00000000, 0x00000000, 0x00000000);
		ArrowIndicatorVisibilityMid = Address(0x32D91A08, 0x00000000, 0x00000000, 0x00000000);
		ArrowIndicatorVisibilityTop = Address(0x32D918E8, 0x00000000, 0x00000000, 0x00000000);
		HUDPointer = Address(0x32C8289C, 0x00000000, 0x00000000, 0x00000000);
		FogA = Address(0x31B31BE4, 0x00000000, 0x00000000, 0x00000000);
		FogB = Address(0x31B31BDC, 0x00000000, 0x00000000, 0x00000000);
		FogC = Address(0x31B31BE0, 0x00000000, 0x00000000, 0x00000000);
		ScrollingTextOpacity = Address(0x00151C40, 0x00000000, 0x00000000, 0x00000000);

		// Savefile
		CostumeObtainStatus = Address(0x32C8256A, 0x00000000, 0x00000000, 0x00000000);
		ColiseumWinCount = Address(0x31A062AC, 0x00000000, 0x00000000, 0x00000000);
		EditMerchantStock = Address(0x31A077A4, 0x00000000, 0x00000000, 0x00000000);
		HeroPointCount = Address(0x31A0611C, 0x00000000, 0x00000000, 0x00000000);
		MainVoice = Address(0x31A06118, 0x00000000, 0x00000000, 0x00000000);
		ResetMerchantStock = Address(0x31A077AE, 0x00000000, 0x00000000, 0x00000000);

		// SFX and BGM
		BGMType = Address(0x005FD3C0, 0x00000000, 0x00000000, 0x00000000);
		BGMVolume = Address(0x236444, 0x00000000, 0x00000000, 0x00000000);
		LinkVoiceSpeed = Address(0x3284A25C, 0x00000000, 0x00000000, 0x00000000);
		MasterVolume = Address(0x004C0AA0, 0x00000000, 0x00000000, 0x00000000);

		}


	// this initializes an address entry with the correct regional address
	// allowing it to be used outside of this file
	
	// Camera
	Address		AddressList::CameraLinkFocus;
	Address		AddressList::CameraMode;
	Address		AddressList::CameraPosX;
	Address		AddressList::CameraPosY;
	Address		AddressList::CameraPosZ;
	Address		AddressList::CameraRotationX;
	Address		AddressList::CameraRotationY;
	Address		AddressList::CameraRotationZ;
	Address		AddressList::MasterZoom;

	// Chaos
	Address		AddressList::HeartsTakeHealth;

	// Costume
	Address		AddressList::CostumeAttrA;
	Address		AddressList::CostumeAttrB;
	Address		AddressList::CostumeAttrC;
	Address		AddressList::CostumeCatalogPointer;
	Address		AddressList::CurrCostume;
	Address		AddressList::UnusedCostumeDataPointerList;
	Address		AddressList::PlayerStatus;
	Address		AddressList::TingleBalloons;

	// Emotes
	Address		AddressList::DoTEmotes;
	Address		AddressList::LobbyEmotes;

	// Game Data
	Address		AddressList::CurrLevelID;
	Address		AddressList::CurrStageID;
	Address		AddressList::CurrSpawnID;
	Address		AddressList::DoppelsEnabled;
	Address		AddressList::DrablandsRupeeCount;
	Address		AddressList::HealthCurrent;
	Address		AddressList::HealthMax;
	Address		AddressList::LoadingStatus;
	Address		AddressList::PreviousLevelID;
	Address		AddressList::PreviousStageID;
	Address		AddressList::PreviousSpawnID;	
	Address		AddressList::TimeElasped


	// Items
	Address		AddressList::BowStrafe;
	Address		AddressList::FireGloveStrafe;
	Address		AddressList::GustStrafe;
	Address		AddressList::HammerStrafe;
	Address		AddressList::RoboNiceAtk;
	Address		AddressList::RoboNiceSFX;
	Address		AddressList::RoboNiceSpeed;
	Address		AddressList::RoboStrafe;
	Address		AddressList::WaterRodStrafe;


	// Miscellaneous
	Address		AddressList::DisplayTopPhoto;
	Address		AddressList::LobbyBallCounter;
	Address		AddressList::LobbyBallSong;
	Address		AddressList::SeeChestContents;
	Address		AddressList::TextBoxSpeed;
	
	// Padding
	Address		AddressList::TextToRodata;

	// Player
	Address		AddressList::ActiveLink;
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
	Address		AddressList::PVPMode;
	Address		AddressList::RespawnPositionX;
	Address		AddressList::RespawnPositionY;
	Address		AddressList::RespawnPositionZ;
	Address		AddressList::SwordType;

	// Rendering
	Address		AddressList::ArrowIndicatorColorTop;
	Address		AddressList::ArrowIndicatorColorMid;
	Address		AddressList::ArrowIndicatorColorBot;
	Address		AddressList::ArrowIndicatorLocationTop;
	Address		AddressList::ArrowIndicatorLocationMid;
	Address		AddressList::ArrowIndicatorLocationBot;
	Address		AddressList::ArrowIndicatorVisibilityTop;
	Address		AddressList::ArrowIndicatorVisibilityMid;
	Address		AddressList::ArrowIndicatorVisibilityBot;
	Address		AddressList::HUDPointer;
	Address		AddressList::FogA;
	Address		AddressList::FogB;
	Address		AddressList::FogC;
	Address		AddressList::ScrollingTextOpacity;


	// Savefile
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
	Address		AddressList::MasterVolume;
}