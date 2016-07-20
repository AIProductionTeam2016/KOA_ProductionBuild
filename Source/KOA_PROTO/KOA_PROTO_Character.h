// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "KOA_BASE_Artifact.h"
#include "KOA_PROTO_Wall.h"
#include "KOA_PROTO_Character.generated.h"

UENUM(BlueprintType)
enum class ECustomMovementType : uint8 {
	CMT_WallSlide = 0	UMETA(DisplayName = "Wall Slide")
};

USTRUCT()
struct KOA_PROTO_API FDetectWallHitInfo {
	GENERATED_USTRUCT_BODY()

	// Display debug information duration
	bool CanJump;
	// Wall direction relative to player
	// -1: Left
	//  1: Right
	//  0: NULL
	int8 WallDirection;
	// WallHit infomation
	float WallHitSlideVelocity;
	float WallHitSlideAcceration;
	float WallHitHoldDuration;

	FDetectWallHitInfo() {
		CanJump = false;
		WallDirection = 0;
		WallHitSlideVelocity = 0.0f;
		WallHitSlideAcceration = 0.0f;
		WallHitHoldDuration = 0.0f;
	}

	void SetCanJump(const bool& value) {
		CanJump = value;
	}
	void SetWallDirection(const int8& value) {
		(value < 2 && value > -2) ? (WallDirection = value) : (WallDirection = 0);
	}
	void SetWallHitInfo(const FWallInformation& value) {
		WallHitSlideVelocity	= value.InitSlideVelocity;
		WallHitSlideAcceration	= value.SlideAcceration;
		WallHitHoldDuration		= value.WallHoldDuration;
	}

	FORCEINLINE bool GetCanJump() const {
		return CanJump;
	}
	FORCEINLINE int8 GetWallDirection() const {
		return WallDirection;
	}
};

// Structure for storing all player jump related variables and methods.
USTRUCT()
struct KOA_PROTO_API FPlayerJumpVariables {
	GENERATED_USTRUCT_BODY()

	// Display debug info
	UPROPERTY(EditAnywhere, Category = "Jump")
	bool DebugInfo;
	// The power of the jump
	UPROPERTY(EditAnywhere, Category = "Jump")
	float Power;

	//TODO: Move SlideAcceleration && SlideVelocity to private after init values are found
	//TODO:    and after demonstrating the feature.
	// The acceleration of the wall slide
	float SlideAcceleration;
	float CurrSlideVelocity;
	float MaxSlideVelocity;
	float FallOffWallHeight;

	/****** Constructor ******/
	FPlayerJumpVariables() {
		DebugInfo			= true;
		Power				= 1000.0f;
		HangingOnWall		= false;
		WallOnPlayerSide	= 0;
		CanDoubleJump		= false;
		DoubleJumpEnabled	= false;
		CurrSlideVelocity	= 1.0f; // Current Slide Velocity
		SlideAcceleration	= 98.0f; // Slide Acceleration
		MaxSlideVelocity	= 1000.0f;
		WallHoldDuration	= 0.0f;
		FallOffWallHeight	= 0.0f;
	}

	/****** Methods for FPlayerJumpVariables ******/
	void DisplayWallSlideDebugInfo() {
		if (DebugInfo) {
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Blue, TEXT("CurrSlideVelocity: " + FString::SanitizeFloat(CurrSlideVelocity)));
		}
	}
	void ApplyWallSlideAcceleration(const float& DeltaTime) {
		CurrSlideVelocity += (SlideAcceleration*DeltaTime);
		if (CurrSlideVelocity > MaxSlideVelocity) {
			CurrSlideVelocity = MaxSlideVelocity;
		}
	}
	void ResetWallSlideVariables() {
		CurrSlideVelocity = 1.0f;
	}

	/****** Setters for private variables ******/
	void SetHangingOnWall(const bool& value) {
		HangingOnWall = value;
	}
	void SetWallOnPlayerSide(const int8& value) {
		(value < 2 && value > -2) ? (WallOnPlayerSide = value) : (WallOnPlayerSide = 0);
	}
	// Only set CanDoubleJump == value if DoubleJumpEnabled == true, 
	// otherwise CanDoubleJump is ALWAYS false.
	void SetCanDoubleJump(const bool& value) {
		(DoubleJumpEnabled) ? (CanDoubleJump = value) : (CanDoubleJump = false);
	}
	void SetCurrSlideVelocity(const float& value) {
		(value < MaxSlideVelocity) ? (CurrSlideVelocity = value) : (CurrSlideVelocity = MaxSlideVelocity);
	}
	// Set the acceleration of the wall slide
	void SetSlideAcceleration(const float& value) {
		SlideAcceleration = value;
	}
	void SetWallHoldDuration(const float& value) {
		WallHoldDuration = value;
	}

	/****** Getters for private variables ******/
	FORCEINLINE int8 GetWallOnPlayerSide() const {
		return WallOnPlayerSide;
	}
	FORCEINLINE bool GetHangingOnWall() const {
		return HangingOnWall;
	}
	FORCEINLINE bool GetCanDoubleJump() const {
		return CanDoubleJump;
	}
	FORCEINLINE float GetCurrSlideVelocity() const {
		return CurrSlideVelocity;
	}
	FORCEINLINE float GetSlideAcceleration() const {
		return SlideAcceleration;
	}
	FORCEINLINE float GetWallHoldDuration() const {
		return WallHoldDuration;
	}
	
	/****** Turn DoubleJumping on and off ******/
	FORCEINLINE bool IsDoubleJumpEnabled() const {
		return DoubleJumpEnabled;
	}
	void EnableDoubleJumping() {
		DoubleJumpEnabled = true;
		CanDoubleJump = true;
	}
	void DisableDoubleJumping() {
		DoubleJumpEnabled = false;
		CanDoubleJump = false;
	}

/****** Private Information - Jump variables ******/
private:
	// Is the player hanging on the wall
	bool HangingOnWall;
	// Wall direction relative to player
	// -1: Left
	//  1: Right
	//  0: NULL
	int8 WallOnPlayerSide;
	// Can the player double jump; can only be positive if DoubleJumpEnabled is true
	bool CanDoubleJump;
	// Determines if Double Jumping turned on in the game. When false, CanDoubleJump will ALWAYS be false
	bool DoubleJumpEnabled;

	float WallHoldDuration;
};

UCLASS()
class KOA_PROTO_API AKOA_PROTO_Character : public ACharacter
{
	GENERATED_BODY()
/********************* PUBLIC VARIABLES *********************/
public:
	// Player Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Info")
	FString PlayerName;

	// Player Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stats")
	float HealthCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stats")
	float HealthMax;

	// Player Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement")
	float RunSpeed;
	// All of the data for the player's jump logic
	UPROPERTY(EditAnywhere, Category = "Player|Jump")
	FPlayerJumpVariables JumpStats;

	/***** ARTIFACTS *****/
	UPROPERTY(EditAnywhere, Category = "Player|Artifacts")
	EArtifactID CurrentArtifact;
	UPROPERTY(EditAnywhere, Category = "Player|Artifacts")
	TArray<TSubclassOf<UKOA_BASE_Artifact>> CollectedArtifacts;

/********************* PUBLIC METHODS *********************/
public:
	/****** CONSTRUCTORS AND INITIALIZERS ******/
	AKOA_PROTO_Character(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/****** MOVEMENT ******/
	void SetMoveSpeedToRun();
	void SetMoveSpeedToWalk();
	void MoveRight(float Amount);

	/****** JUMPING ******/
	FDetectWallHitInfo DetectWall();
	void PlayerJump();
	void PlayerStopJump();
	virtual void Landed(const FHitResult &Hit) override;
	void StartWallSlide();
	void LoseGripAndFall();

	/****** ARTIFACTS ******/
	void EquipDualDaggers();
	void EquipFireGlove();
	// void EquipLightningBow();
	// void EquipMatterHammer();

	bool SetCurrentArtifact(EArtifactID Artifact);

	/****** ABILITIES ******/
	void UseCurrentAbilityQ();

	void PressCurrentAbilityQ();
	void ReleaseCurrentAbilityQ();

	//void UseCurrentAbilityW();
	//void UseCurrentAbilityE();
	//void UseCurrentAbilityR();
	FORCEINLINE bool GetIsAbilityUseLocked() const;
	FORCEINLINE bool GetIsArtifactSwapLocked() const;
	FORCEINLINE EAbilityID GetWhichAbilityPressed() const;
	void SetWhichAbilityPressed(const EAbilityID &AbilityID);
	//void ResetAbilityCooldown(FAbility Ability);

	

	/****** TIMERS ******/
	void StartAbilityCooldownTimer(UKOA_BASE_Artifact* CurrentArtifact, EAbilityID AbilityID);
	//void StartAbilityCooldownTimer(const float &Duration);


	void StartArtifactSwapLockTimer(const float &Duration);
	void StartWallHoldTimer(const float &Duration);
	void StartWallSlideTimer(const float &Duration);
	void ClearWallHoldTimer();
	void ClearWallSlideTimer();

/********************* PRIVATE VARIABLES *********************/
private:
	//TODO: Rename bool variables to have Enabled;
	//TODO: Rename getters to Is________()
	/***** MOVEMENT *****/
	bool IsSlidingDownWall;

	/***** ABILTIES *****/
	bool IsAbilityUseLocked;
	bool IsArtifactSwapLocked;
	float ArtifactSwapLockDuration;
	float AbilityLockDuration;

	EAbilityID AbilityPressed;

	/***** TIMERS *****/
	FTimerHandle AbilityLockTimer;
	FTimerHandle ArtifactSwapLockTimer;
	FTimerHandle WallHoldTimer;
	FTimerHandle WallSlideTimer;

	/***** WORLD *****/
	const UWorld* WorldPtr;
	FORCEINLINE const UWorld* GetWorldPtr() const {
		return WorldPtr;
	}
/********************* PRIVATE METHODS *********************/
private:
	void UnlockAbilityUse();
	void UnlockArtifactSwap();

	void DEBUG_EquipCurrentArtifact();
};
