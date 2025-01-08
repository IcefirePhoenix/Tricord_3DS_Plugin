#ifndef ADDRESSLIST_HPP
#define ADDRESSLIST_HPP

#include "CTRPF.hpp"
#include "Helpers/DataHelpers/Address.hpp"

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
		static Address		CostumeAttrD;
		static Address		CurrCostume;
		static Address		CurrCostumeAlt;
		static Address		CostumeCatalogPointer;
		static Address		PlayerStatus;
		static Address		TingleBalloons;
		static Address		DapperSwingCountdown;
		static Address		DapperSpinCheck;
		static Address		HeartDropRate;
		static Address		BearDodge;
		static Address		UnusedCostumeDataPointers;
		static Address		BearMinMaxCheckA;
		static Address		BearMinMaxCheckB;
		static Address		DoppelStageResetA;
		static Address		DoppelStageResetB;
		static Address		DoppelStageResetC;
		static Address		DoppelLobbyReset;
		static Address		CostumeIDOffsetAuraA;
		static Address		CostumeIDOffsetAuraB;
		static Address		CostumeIDOffsetAuraC;
		static Address		CostumeIDOffsetAuraD;
		static Address		CostumeIDOffsetCheetah;
		static Address		CostumeIDOffsetDune;
		static Address		CostumeIDOffsetCheer;
		static Address		CostumeIDOffsetSwordPtcl;
		static Address		CostumeIDFunBLModel;
		static Address		KokiriA;
		static Address		KokiriB;
		static Address		KokiriC;
		static Address		KokiriD;
		static Address		KokiriE;
		static Address		BigBombA;
		static Address		BigBombB;
		static Address		BigBombC;
		static Address		BoomerangerA;
		static Address		BoomerangerB;
		static Address		BoomerangerC;
		static Address		BoomerangerD;
		static Address		BoomerangerE;
		static Address		BoomerangerF;
		static Address		TorrentA;
		static Address		TorrentB;
		static Address		TorrentC;
		static Address		TorrentD;
		static Address		TorrentE;
		static Address		GustGarbA;
		static Address		GustGarbB;
		static Address		GustGarbC;
		static Address		GustGarbD;
		static Address		GustGarbE;
		static Address		GustGarbF;
		static Address		GustGarbG;
		static Address		HammerwearA;
		static Address		HammerwearB;
		static Address		HammerwearC;
		static Address		HammerwearD;
		static Address		HammerwearE;
		static Address		HammerwearF;
		static Address		HammerwearG;
		static Address		RobowearA;
		static Address		RobowearB;
		static Address		RobowearC;
		static Address		FireBlazerA;
		static Address		FireBlazerB;
		static Address		FireBlazerC;
		static Address		FireBlazerD;
		static Address		LightArmorA;
		static Address		LightArmorB;
		static Address		LinebeckTimeA;
		static Address		LinebeckTimeB;
		static Address		LinebeckTimeC;
		static Address		CheetahA;
		static Address		CheetahB;
		static Address		CheetahC;
		static Address		SwordDamageBoostA;
		static Address		SwordDamageBoostB;
		static Address		SwordBeamAbility;
		static Address		QuadBeamA;
		static Address		QuadBeamB;
		static Address		QuadBeamC;
		static Address		QuadBeamSpinA;
		static Address		QuadBeamSpinB;
		static Address		QuadBeamSpinC;
		static Address		KnockbackImmunity;
		static Address		SwordHammerDmgBoost;
		static Address		SwordBeamHealthCheckA;
		static Address		SwordBeamHealthCheckB;
		static Address		SwordType;

		// Console
		static Address 		ABXY;
		static Address 		StartSelLRTouch;
		static Address 		DPadCPad;

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
		static Address		EnemyDataPointer;
		static Address		HealthCurrent;
		static Address		HealthMax;
		static Address      EnergyCurrent;
		static Address		EnergyMax;
		static Address		EnergyMaxPitRecovery;
		static Address		EnergyMaxMultiplicand;
		static Address		LargeEnergyGauge;
		static Address		FairiesCurrent;
		static Address		TimeLeft;
		static Address		CollisionCurrent;
		static Address		SpeedX;
		static Address		SpeedY;
		static Address		SpeedZ;
		static Address		TotemCarrying;
		static Address		TotemCarriedBy;
		static Address		LoadingStatus;
		static Address		PauseStatus;
		static Address		PreviousLevelID;
		static Address		PreviousStageID;
		static Address		PreviousSpawnID;
		static Address		TimeElapsed;
		static Address		TargetLevelID;
		static Address		TargetStageID;
		static Address		TargetSpawnID;
		static Address		Warp;
		static Address 		WarpActivation;
		static Address		WarpPointer;
		static Address		ChallengeID;
		static Address		SpeedMultiplierAlt;

		// Items
		static Address		BowStrafe;
		static Address		FireGloveStrafe;
		static Address		GustStrafe;
		static Address		HammerStrafe;
		static Address		ShadowLinkItemPointer;
		static Address		GripshotStrafe;
		static Address		WaterRodStrafe;

		// Miscellaneous
		static Address		CameraXButtonToggle;
		static Address		CameraShutter;
		static Address		CheckPhotoExist;
		static Address		DisplayTopPhoto;
		static Address		SeeChestContents;
		static Address		SwordBeamCD;
		static Address		TextBoxSpeed;

		// Padding
		static Address		TextToRodata; // ends 0x6AA000 for NA

		// Player
		static Address		ActiveLink;
		static Address		CurrentAnim;
		static Address		CurrentItem;
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
		static Address		PreviousAnim;
		static Address		PVPMode;
		static Address		RespawnPositionX;
		static Address		RespawnPositionY;
		static Address		RespawnPositionZ;

		// Rendering
		static Address		ArrowIndicatorColorBot;
		static Address		ArrowIndicatorColorMid;
		static Address		ArrowIndicatorColorTop;
		static Address		HUDPointer;
		static Address		FogA;
		static Address		FogB;
		static Address		FogC;
		static Address		ScrollingTextOpacity;
		static Address		OffScreenLocation;
		static Address		TextureName;
		static Address		LoadingScreenSPNoChal;
		static Address		LoadingScreenSPIcons;
		static Address		CheerPomPom;
		static Address		Aura;
		static Address		SwordParticle;
		static Address		PlayerLuminanceNormalA;
		static Address		PlayerLuminanceNormalB;
		static Address		PlayerLuminanceLightArmor;

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
		static Address		Voice8bit;
		static Address		LobbyBallPointer;
		static Address		MasterVolume;
		static Address		SwordSwipe8bit;
		static Address		SwordCharge8bit;
		static Address		SwordSpin8bit;

		// Turbo Mode
		static Address		DashTimerNormal;
		static Address		DashTimerNinja;
		static Address		SwordBeamCDExe;
		static Address		HammerwearSpeed;
		static Address		SL_SwordSpeed;
		static Address		SL_Strafe;
		static Address		SL_RunAnimSpeed;
		static Address		SL_RunSpeed;
		static Address		BombFlashSpeed;
		static Address		LoadingIconAnimSpeed;
		static Address		LiveMsgSpeed;
		static Address		SwordBeamSpeed;
		static Address		SpeedDamageUndo;
		static Address		BGMSFXSpeed;
		static Address		EnergyBarStretch;
		static Address		SpeedMultiplierNormal;
		static Address		SpeedMultiplierCheetah;
		static Address		DashStaggerSpeed;
		static Address		DashSpeed;
		static Address		DashBrakeDuration;
		static Address		DashPlayerCrashHeight;
		static Address		DashWallCrashHeight;
		static Address		DashCrashBackwards;
		static Address		FireJumpHeight;
		static Address		LavaBounceHeight;
		static Address		WaterJumpHeight;
		static Address		WaterJumpForwardSpeed;
		static Address		SwimBoostSpeedZora;
		static Address		SwimBoostSpeedNormal;
		static Address		SwimTurnaroundSpeed;
		static Address		SwimSpeed;
		static Address		ArrowSpeedPlayer;
		static Address		FireballHeight;
		static Address		FireballSpeed;
		static Address		FireballBounceHeight;
		static Address		FireballThrowHeight;
		static Address		FireballReleaseTime;
		static Address		BoomerangReturnSpeed;
		static Address		BoomerangDistance;
		static Address		BoomerangSpeed;
		static Address		GustSpeed;
		static Address		GripshotRetractSpeed;
		static Address		GripshotExtendSpeed;

		// Animation Speeds
		static Address		Link_Wait;
		static Address		Link_WalkSlow;
		static Address		Link_Walk;
		static Address		Link_Dash;
		static Address		Link_DashTotem;
		static Address		Link_GrabDash;
		static Address		Link_CutRF;
		static Address		Link_CutRFTotem;
		static Address		Link_CutRFtoKeep;
		static Address		Link_KeepWait;
		static Address		Link_Compete;
		static Address		Link_CutTurn;
		static Address		Link_CutTurnW;
		static Address		Link_CutTurnB;
		static Address		Link_GrabP_A;
		static Address		Link_GrabP_B; // Duplicate; Default speed -1.0
		static Address		Link_GrabUp;
		static Address		Link_GrabThrow;
		static Address		Link_GrabWait;
		static Address		Link_WaitTotem;
		static Address		Link_GrabYorokeTotem;
		static Address		Link_YorokeTotem;
		static Address		Link_WaitYoroke;
		static Address		Link_JumpEdS_A; // Default speed 0.75
		static Address		Link_DamF;
		static Address		Link_DamB;
		static Address		Link_FallLava;
		static Address		Link_FallSand;
		static Address		Link_Drown;
		static Address		Link_DrownLava;
		static Address		Link_Fall;
		static Address		Link_DamBiri;
		static Address		Link_WaitQ;
		static Address		Link_DamBoyon;
		static Address		Link_DamFFup;
		static Address		Link_DamSwimF;
		static Address		Link_DamSwimB;
		static Address		Link_LLhold;
		static Address		Link_LLswallowSt;
		static Address		Link_LLstruggle;
		static Address		Link_FallTotem;
		static Address		Link_FallBallon;
		static Address		Link_BowWalkA;
		static Address		Link_BowWalkB;
		static Address		Link_Slipice;
		static Address		Link_BowWait;
		static Address		Link_ArrowShoot;
		static Address		Link_RodWait;
		static Address		Link_RodAttack;
		static Address		Link_BoomAttack;
		static Address		Link_BoomCarried;
		static Address		Link_HookshotWait;
		static Address		Link_HookshotAttack;
		static Address		Link_HookshotDrag;
		static Address		Link_Bump;
		static Address		Link_HookshotPullSt;
		static Address		Link_HookshotPullEd;
		static Address		Link_AirCannonWait;
		static Address		Link_AirCannonAttack;
		static Address		Link_FireGloveWait;
		static Address		Link_FireGloveAttack;
		static Address		Link_HammerWait;
		static Address		Link_HammerAttack;
		static Address		Link_HammerAttackTotem;
		static Address		Link_HammerMiss;
		static Address		Link_Die;
		static Address		Link_DieSt;
		static Address		Link_DieEd;
		static Address		Link_Talk;
		static Address		Link_ItemGetA_A;
		static Address		Link_ItemGetA_B; // Duplicate
		static Address		Link_ItemGetShort;
		static Address		Link_UnlockB;
		static Address		Link_PDashReady;
		static Address		Link_PDash;
		static Address		Link_DashAchichi;
		static Address		Link_PDashCrash;
		static Address		Link_PDashBrake;
		static Address		Link_PDashStagger;
		static Address		Link_WaitPushPull;
		static Address		Link_WalkPush;
		static Address		Link_WalkPull;
		static Address		Link_Swim;
		static Address		Link_SwimWait;
		static Address		Link_SwimDive;
		static Address		Link_SwimLand;
		static Address		Link_SwimDrift;
		static Address		Link_SwimScrewAttack_A;
		static Address		Link_SwimScrewAttack_B; // Duplicate
		static Address		Link_SwimWaitPuppet;
		static Address		Link_WaitAtoB;
		static Address		Link_WaitB;
		static Address		Link_WaitFight;
		static Address		Link_WaitPuppet;
		static Address		Link_WalkPuppet;
		static Address		Link_Skyward;
		static Address		Link_JumpSt;
		static Address		Link_JumpAuto;
		static Address		Link_JumpEdS_B; // Duplicate; Default speed 0.75
		static Address		Link_CarryAway;
		static Address		Link_Call;
		static Address		Link_Rock;
		static Address		Link_Sad;
		static Address		Link_SadWait;
		static Address		Link_WalkMagic;
		static Address		Link_TurnMagic;
		static Address		Link_EdYorokeTotem1;
		static Address		Link_EdWaitTotem1;
		static Address		Link_EdYorokeTotem2;
		static Address		Link_EdWaitTotem2;
		static Address		Link_WaitPinch;

		// facial expression data
		static Address FileTableRewrite;
		static Address FileTableStart;
		static Address CustomInfoHeaders;
		static Address		CustomFacePointerList;
		static Address		EditFaceStatusBit;
		static Address 		EyeTitleReference;
		static Address 		HytopiaPMAStart;
		static Address 		IndivPMAStart;
		static Address 		CustomAnimData;
	};
}

#endif