// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_PROTO_Wall.h"
#include "KOA_PROTO_Character.h"
#include "KOA_PROTO_CharacterMovementSlide.h"

/**************************************************************************
	CONSTRUCTORS AND INITIALIZERS
**************************************************************************/
// Default Constructor
AKOA_PROTO_Character::AKOA_PROTO_Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UKOA_PROTO_CharacterMovementSlide>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default values
	PlayerName = "Chester";

	// Stats
	HealthCurrent = 75.0f;
	HealthMax = 100.0f;

	// Movement
	WalkSpeed = 450.0f;
	RunSpeed = 900.0f;
	//JumpStats.EnableDoubleJumping();
	IsSlidingDownWall = false;

	// Initialize Current Artifact
	CurrentArtifact = EArtifactID::ID_NULL;

	// Initialize ability and artifact locks
	IsAbilityUseLocked = false;
	IsArtifactSwapLocked = false;
	ArtifactSwapLockDuration = 1.0f;
	AbilityLockDuration = 0.5f;
	AbilityPressed = EAbilityID::NONE;

	// Get a pointer to the current world
	WorldPtr = GetWorld();

	// Create and attach the E_AimingMesh to it's CapsuleComponent
	VD_E_AimingCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("VD_E_AimingCapsule"));
	
	VD_E_AimingMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VD_E_AimingMeshComponent"));
	VD_E_AimingMeshComponent->AttachTo(VD_E_AimingCapsule);
	
	//VD_E_AimingMesh = CreateDefaultSubobject<USkeletalMesh>(TEXT("VD_E_AimingMesh"));
	//VD_E_AimingMeshComponent->SetSkeletalMesh(VD_E_AimingMesh);
}

// Called when the game starts or when spawned
void AKOA_PROTO_Character::BeginPlay() {
	Super::BeginPlay();	
}

// Called every frame
void AKOA_PROTO_Character::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

	
	// if (AbilityPressed != EAbilityID::NONE) {
	// 	// Access the current artifact's tick function
	// 	CurrentArtifact.Tick(DeltaTime);
	// }

	// Check to see if an ability button is being held
	if (AbilityPressed != EAbilityID::NONE) {
		//Access the current artifact's tick function
		UKOA_BASE_Artifact* currArtifact = CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();
		currArtifact->Tick(DeltaTime);
		//GetWorldPtr()->GetFirstPlayerController()->CurrentMouseCursor = (TEnumAsByte<EMouseCursor::Type>)EMouseCursor::Crosshairs;

			//TEnumAsByte<EMouseCursor::Type> CurrentMouseCursor;
	} else {
		//GetWorldPtr()->GetFirstPlayerController()->CurrentMouseCursor = (TEnumAsByte<EMouseCursor::Type>)EMouseCursor::Default;
	}


	// If we are sliding down the wall
	/*if (IsSlidingDownWall) {
		FVector playerLocation = GetActorLocation();
		JumpStats.ApplyWallSlideAcceleration(DeltaTime);
		JumpStats.DisplayWallSlideDebugInfo();
		// playerLocation - (distance * -UpVector * DeltaTime)
		FVector finalLocation = playerLocation - (JumpStats.GetCurrSlideVelocity() * GetActorUpVector() * DeltaTime);
		//TODO: Implement a better floor detection
		if (finalLocation.Z < 110.15) finalLocation.Z = 110.15;
		SetActorLocation(finalLocation);
	}*/
}

// Called to bind functionality to input
void AKOA_PROTO_Character::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);
	check(InputComponent);

	// Bind Axis
	InputComponent->BindAxis("MoveRight", this, &AKOA_PROTO_Character::MoveRight);
	// Bind Actions
	InputComponent->BindAction("Run", IE_Pressed, this, &AKOA_PROTO_Character::SetMoveSpeedToRun);
	InputComponent->BindAction("Run", IE_Released, this, &AKOA_PROTO_Character::SetMoveSpeedToWalk);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AKOA_PROTO_Character::PlayerJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AKOA_PROTO_Character::PlayerStopJump);
	//TODO: Add a release binding for AbilityQ
	InputComponent->BindAction("AbilityQ", IE_Pressed, this, &AKOA_PROTO_Character::PressCurrentAbilityQ);
	InputComponent->BindAction("AbilityQ", IE_Released, this, &AKOA_PROTO_Character::ReleaseCurrentAbilityQ);

	InputComponent->BindAction("EquipArtifact_DualDaggers", IE_Pressed, this, &AKOA_PROTO_Character::EquipDualDaggers);
	InputComponent->BindAction("EquipArtifact_FireGlove", IE_Pressed, this, &AKOA_PROTO_Character::EquipFireGlove);

	//TODO: QuickArtifactSelect press release
}


/**************************************************************************
	MOVEMENT - 
		Methods to handle player movement. 
		Walking, Running, Jumping, Sliding
**************************************************************************/
// SetMoveSpeedToRun(): 
//		Set's the player's MaxWalkSpeed to RunSpeed
void AKOA_PROTO_Character::SetMoveSpeedToRun() {
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

// SetMoveSpeedToWalk(): 
//		Set's the player's MaxWalkSpeed to WalkSpeed
void AKOA_PROTO_Character::SetMoveSpeedToWalk() {
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// MoveRight(Amount): 
//		Move right if Amount > 0; left if Amount is < 0
void AKOA_PROTO_Character::MoveRight(float Amount) {
	// Only move if the controller is set up and Amount is not 0
	if (Controller && Amount) {
		// AddMovementInput in the direction of the right vector
		AddMovementInput(FVector(0.0, 1.0, 0.0), Amount);
	}
}

/**************************************************************************
	JUMPING -
		Methods used to handle jumping logic.
**************************************************************************/
// DetectWall():
//		Used to detect if there is a wall the player can jump off of.
//		If there is, it returns info detailing the wall's properties.
FDetectWallHitInfo AKOA_PROTO_Character::DetectWall() {
	// Create hitinfo
	FDetectWallHitInfo DWHitInfo;
	// Create a local variable to keep track of displaying debug info
	EDrawDebugTrace::Type DebugDuration;
	(JumpStats.DebugInfo) ? (DebugDuration = EDrawDebugTrace::ForDuration) : (DebugDuration = EDrawDebugTrace::None);
	// Create local varialbes to be used in SphereTrace
	// Start at player location
	FVector PlayerLocation = GetActorLocation();
	// from there, go up the players height to the end
	FVector EndLocation = PlayerLocation + (100 * GetActorUpVector());
	// radius of the sphere to trace
	float Radius = 50;
	// Array of object to hit; Need to figure out exactly how to set this up
	// I BELIEVE ObjectTypeQuery1 is WorldStatic and ObjectTypeQuery2 is WorldDynamic
	TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	// Actors to ignore
	TArray<AActor*>ActorsToIgnore;
	// Hit result
	FHitResult HitResult;
	// Perform the sphere trace 
	bool HitSomething = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this, PlayerLocation, EndLocation, Radius, ObjectTypes,
		false, ActorsToIgnore, DebugDuration, HitResult, true);
	// If you hit something
	if (HitSomething) {
		// Check to see if it's a wall
		AKOA_PROTO_Wall* wall = Cast<AKOA_PROTO_Wall>(HitResult.GetActor());
		// If you hit a wall
		if (wall) {
			// Do another sphere trace, at the players feet in the direction they are facing
			EndLocation = PlayerLocation + (25 * GetActorForwardVector());
			Radius = 25;
			bool CanJump = UKismetSystemLibrary::SphereTraceSingleForObjects(
				this, PlayerLocation, EndLocation, Radius, ObjectTypes,
				false, ActorsToIgnore, DebugDuration, HitResult, true);
			// If you CanJump
			if (CanJump) {
				DWHitInfo.SetCanJump(true);
				FVector forward = GetActorForwardVector();
				FVector right = FVector(0.0f, 1.0f, 0.0f);
				// Take the dot product with the forward vector and the right world vector
				// only need the y and z, x never moves
				float dot = forward.Y * right.Y + forward.Z * right.Z;
				// if negative, wall is to the left, if positive it is to the right
				(dot < 0) ? DWHitInfo.SetWallDirection(-1) : DWHitInfo.SetWallDirection(1);
				// Set the wall hit info to the wall you are touching
				DWHitInfo.SetWallHitInfo(wall->GetWallInfo());
			}
		}
	}
	// If you didn't hit a wall, all the values will be the default values; otherwise return the proper results
	return DWHitInfo;
}

// PlayerJump():
//		Main method involded in the player jump logic.
void AKOA_PROTO_Character::PlayerJump() {
	// If the player is grounded, then this is their first jump
	if (!GetCharacterMovement()->IsFalling()) {
		ACharacter::Jump();
	}
	// The player has already jumped or is otherwise in the air
	else {
		// Check to see if we can jump off a wall
		FDetectWallHitInfo WallHitInfo = DetectWall();

		// If you can jump off a wall do it
		if (WallHitInfo.GetCanJump()) {
			// Disable movement to allow user to hang on wall
			//GetCharacterMovement()->DisableMovement();

			//Set movement mode to WallSlide
			GetCharacterMovement()->SetMovementMode(MOVE_Custom, (uint8)ECustomMovementType::CMT_WallSlide);
			// Pass the results up to the player
			JumpStats.SetHangingOnWall(true);
			JumpStats.SetWallOnPlayerSide(WallHitInfo.GetWallDirection());
			// Set the stats that you got from the wall hit
			JumpStats.SetCurrSlideVelocity(WallHitInfo.WallHitSlideVelocity);
			JumpStats.SetSlideAcceleration(WallHitInfo.WallHitSlideAcceration);
			JumpStats.SetWallHoldDuration(WallHitInfo.WallHitHoldDuration);
			// Set the timer for holding onto the wall
			StartWallHoldTimer(JumpStats.GetWallHoldDuration());

		} // Otherwise, check if you can double jump, if so do it,
		else if (JumpStats.GetCanDoubleJump()) {
			JumpStats.SetCanDoubleJump(false);
			ACharacter::Jump();
		}
	}
}

// Using the unit circle, 60deg is at ((+/-)1/2, sqrt(3)/2)
// Use this for the launch angle of the player off of the wall
const float ROOT_TWO_OVER_TWO   = 0.70710678118;
const float ROOT_THREE_OVER_TWO = 0.86602540378;
void AKOA_PROTO_Character::PlayerStopJump() {
	// If you are hanging on the wall, jump off
	if (JumpStats.GetHangingOnWall()) {
		// Multiply JumpStats.GetWallOnPlayerSide into the y value for it to launch in the proper direction
		GetCharacterMovement()->AddImpulse(
			FVector(0.0f,
			-1 * JumpStats.GetWallOnPlayerSide() * JumpStats.Power * 0.5, // -1 is for making sure you apply jump in the proper direction, inverts GetWallOnPlayerSide()
			JumpStats.Power * ROOT_THREE_OVER_TWO)
			, true
			);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		JumpStats.SetHangingOnWall(false);
		IsSlidingDownWall = false;
		ClearWallHoldTimer();
		ClearWallSlideTimer();
	}
	ACharacter::StopJumping();
}

//Landed(const FHitResult &Hit): 
//		Override of ACharacter::Landed(const FHitResult &Hit)
//		Used in jump logic
void AKOA_PROTO_Character::Landed(const FHitResult &Hit) {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Player has landed.");
	Super::Landed(Hit);

	// Not sliding on a wall
	IsSlidingDownWall = false;
	// Reset JumpStats
	JumpStats.SetHangingOnWall(false);
	JumpStats.SetWallOnPlayerSide(0);
	// If you can double jump, reset the jump bool.
	if (JumpStats.IsDoubleJumpEnabled()) JumpStats.SetCanDoubleJump(true);
}

void AKOA_PROTO_Character::StartWallSlide() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "Start to slide down the wall");
	ClearWallHoldTimer();
	
	// Used in the Tick() event to "Slide" the player down the wall
	IsSlidingDownWall = true;

	// Begin the timer for sliding down the wall
	StartWallSlideTimer(2.0f);
}

void AKOA_PROTO_Character::LoseGripAndFall() {
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	JumpStats.SetHangingOnWall(false);
	IsSlidingDownWall = false;

	// Clear the wall slide timer
	ClearWallSlideTimer();
}


/**************************************************************************
	ARTIFACTS -
		Methods to handle equipping artifacts 
**************************************************************************/
// EquipDualDaggers():
//		Wrapper method used for binding DualDaggers to a button for gameplay / debug.
void AKOA_PROTO_Character::EquipDualDaggers() {
	// if you are equipping daggers that aren't already equipped
	if (GetIsArtifactSwapLocked() != true && SetCurrentArtifact(EArtifactID::ID_DualDaggers)) {
		DEBUG_EquipCurrentArtifact();
		// Lock artifact swaping
		IsArtifactSwapLocked = true;
		StartArtifactSwapLockTimer(ArtifactSwapLockDuration);
	}
	
	//DEBUG: No DualDaggers in inventory
	if (CollectedArtifacts.Num() <= (uint8)EArtifactID::ID_DualDaggers) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "You don't have DualDaggers dummy");
	}
	if (GetIsArtifactSwapLocked() == true) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "IsArtifactSwapLocked == true");
	}
}

// EquipFireGlove():
//		Same as DualDaggers, except with the FireGlove...
void AKOA_PROTO_Character::EquipFireGlove() {
	if (!GetIsArtifactSwapLocked() && SetCurrentArtifact(EArtifactID::ID_FireGlove)) {
		DEBUG_EquipCurrentArtifact();
		// Lock artifact swaping
		IsArtifactSwapLocked = true;
		StartArtifactSwapLockTimer(ArtifactSwapLockDuration);
	}

	//DEBUG: No FireGlove in inventory
	if (CollectedArtifacts.Num() <= (uint8)EArtifactID::ID_DualDaggers) {
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "You don't have the FireGlove dummy. GO FIND STUFF");
	}
	if (GetIsArtifactSwapLocked() == true) {
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "IsArtifactSwapLocked == true");
	}
}

// GetCurrentArtifact()
//
//
uint8 AKOA_PROTO_Character::GetEquippedArtifact_Implementation() const {
	return (uint8)this->CurrentArtifact;
}

// SetCurrentArtifact(Artifact): 
//		Sets the player's CurrentArtifact ENUM to Artifact. Checks to see if
//		the player has collected the artifact.
//		RETURNS: 
//			true: if you are setting CurrentArtifact to the value of Artifact for the first time.
//			false: if you either don't have it or it is already equipped
bool AKOA_PROTO_Character::SetCurrentArtifact(EArtifactID Artifact) {
	//TODO: Further/Better explain: CollectedArtifacts.Num() > (uint8)Artifact
	// If you have collected the artifact at this point, you can equip it...
	// Also make sure you don't have it equipped
	if (CollectedArtifacts.Num() > (uint8)Artifact && Artifact != CurrentArtifact) {
		CurrentArtifact = Artifact;
		SetCurrArtifactPlayerReference();
		return true;
	} else if (Artifact == CurrentArtifact) {
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Already have that Artifact equipped.");
	}
	return false;
}

void AKOA_PROTO_Character::SetCurrArtifactPlayerReference() {
	UKOA_BASE_Artifact* artifact = CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();
	artifact->SetPlayerReference(this);

}

void AKOA_PROTO_Character::UnlockArtifactSwap() {
	IsArtifactSwapLocked = false;
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "IsArtifactSwapLocked == false");
}

void AKOA_PROTO_Character::DEBUG_EquipCurrentArtifact() {
	switch (CurrentArtifact) {
	case EArtifactID::ID_DualDaggers:
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Equipped: DualDaggers");
		break;

	case EArtifactID::ID_FireGlove:	
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Equipped: FireGlove");
		break;

	case EArtifactID::ID_NULL:
	default:
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "ERROR: Either ID_NULL or INVALID EArtifactID");
		break;
	}
}


/**************************************************************************
	ABILITIES -
		Methods to handle using abilities
**************************************************************************/
// DEPRICATED_UseCurrentAbilityQ():
//		Uses the Q Ability that is on the currently selected artifact, 
//		assuming the player has an artifact equipped.
//void AKOA_PROTO_Character::DEPRICATED_UseCurrentAbilityQ() {
//	// Make sure the player has an artifact equipped.
//	if (CurrentArtifact != EArtifactID::ID_NULL) {
//		// If ability usage isn't locked
//		if (GetIsAbilityUseLocked() != true) {
//			// Access the Q Ability on the Artifact that is currently selected.
//			UKOA_BASE_Artifact* artifact = CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();
//			if (artifact->AbilityQ.IsAbilityOnCooldown() == false) {
//				artifact->PressAbilityQ();
//				////CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>()->CastAbilityQ();
//				////float duration = CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>()->AbilityQ.AbilityLockOutDuration;
//				// Lock ability use for duration
//				IsAbilityUseLocked = true;
//				artifact->AbilityQ.SetAbilityOnCooldown();
//				StartAbilityCooldownTimer(artifact, EAbilityID::ABID_Q);
//			} else {
//				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "Ability Q on cooldown.");
//			}
//
//		} 
//	}
//}
// The function to run when the player press Q
void AKOA_PROTO_Character::PressCurrentAbilityQ() {
	// Make sure the player has an Artifact equipped
	if (CurrentArtifact != EArtifactID::ID_NULL) {
		// If abilities aren't locked
		if (GetIsAbilityUseLocked() != true) {
			// Get the current artifact see if it's Q is on cooldown
			UKOA_BASE_Artifact* artifact = CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();
			if (artifact->AbilityQ.IsAbilityOnCooldown() == false) {
				// Lock ability use until you release the button
				IsAbilityUseLocked = true;
				SetWhichAbilityPressed(EAbilityID::ABID_Q);
				// Run the abilityQ press on current artifact
				artifact->PressAbilityQ();
				artifact->SetCurrentHeldAbilityButton(EAbilityID::ABID_Q);
			}
			else {
				//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "ERROR PressCurrentAbilityQ: AbilityQ.IsAbilityOnCooldown() == true");
			}
		} else {
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "ERROR PressCurrentAbilityQ: GetIsAbilityUseLocked() == true");
		}
	} 
}
// The function to run when the player releases Q
void AKOA_PROTO_Character::ReleaseCurrentAbilityQ() {
	// Make sure the player has an Artifact equipped 
	// Make sure the current ability in use is Q

	if (CurrentArtifact != EArtifactID::ID_NULL && GetWhichAbilityPressed() == EAbilityID::ABID_Q) {
		// Access the Q Ability on the Artifact that is currently selected.
		UKOA_BASE_Artifact* artifact = CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();
		if (artifact->AbilityQ.IsAbilityOnCooldown() == false) {
			artifact->AbilityQ.SetAbilityOnCooldown();
			artifact->ReleaseAbilityQ();			
			StartAbilityCooldownTimer(artifact, EAbilityID::ABID_Q);
			AbilityPressed = EAbilityID::NONE;
			artifact->SetCurrentHeldAbilityButton(EAbilityID::NONE);
		}
		else {
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Ability is on cooldown!");
		}
	}
	else {
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Can't ReleaseCurrentAbilityQ()");
	}
}

//** Getters **//
bool AKOA_PROTO_Character::GetIsAbilityUseLocked() const {
	return IsAbilityUseLocked;
}
bool AKOA_PROTO_Character::GetIsArtifactSwapLocked() const {
	return IsArtifactSwapLocked;
}

EAbilityID AKOA_PROTO_Character::GetWhichAbilityPressed() const {
	return AbilityPressed;
}

bool AKOA_PROTO_Character::GetIsCurrentArtifactAbilityOnCooldown(const EAbilityID &AbilityID) const {
	// Get the current artifact see if it's Q is on cooldown
	//UKOA_BASE_Artifact* artifact = this->CollectedArtifacts[(uint8)this->CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();
	
	bool isOnCooldown = false;

	switch (AbilityID) {
	case EAbilityID::ABID_Q:
			//isOnCooldown = artifact->AbilityQ.IsAbilityOnCooldown();
			if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, "Current Ability Q IsOnCooldown: " + isOnCooldown);
			break;
		default:
			isOnCooldown = false;
			break;
	}
	
	return isOnCooldown;
}

//** Setters **//
void AKOA_PROTO_Character::SetWhichAbilityPressed(const EAbilityID& AbilityID) {
	AbilityPressed = AbilityID;
}

// UnlockAbilityUse():
//		Allows abilities to be used again.
void AKOA_PROTO_Character::UnlockAbilityUse() {
	IsAbilityUseLocked = false;
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, "UnlockAbilityUse()");
}

//void AKOA_PROTO_Character::ResetAbilityCooldown() {
//	//Ability.ResetAbilityCooldown();
//}

/**************************************************************************
	TIMERS -
		Methods for handling timers.
**************************************************************************/
void AKOA_PROTO_Character::StartAbilityCooldownTimer(UKOA_BASE_Artifact* CurrentArtifact, EAbilityID AbilityID) {
	// When the timer ends, unlock ability use
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(AbilityLockTimer, this, &AKOA_PROTO_Character::UnlockAbilityUse, AbilityLockDuration, false);
		switch (AbilityID) {
		case EAbilityID::ABID_Q:
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "StartAbilityCooldownTimer: ABID_Q");
			GetWorldPtr()->GetTimerManager().SetTimer(CurrentArtifact->AbilityQTimer, CurrentArtifact, &UKOA_BASE_Artifact::ResetAbilityQCooldown, CurrentArtifact->AbilityQ.AbilityCooldownDuration, false);
			break;
		default:
			break;
		}
	}
}

void AKOA_PROTO_Character::StartArtifactSwapLockTimer(const float &Duration) {
	// When the timer ends, unlock artifact swapping
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(ArtifactSwapLockTimer, this, &AKOA_PROTO_Character::UnlockArtifactSwap, Duration, false);
	}
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "StartArtifactSwapLockTimer.");
}

void AKOA_PROTO_Character::StartWallHoldTimer(const float &Duration) {
	// When the timer ends, start sliding down the wall
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(WallHoldTimer, this, &AKOA_PROTO_Character::StartWallSlide, Duration, false);
	}
}

void AKOA_PROTO_Character::StartWallSlideTimer(const float &Duration) {
	// When the timer ends, lose your grip and fall
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(WallHoldTimer, this, &AKOA_PROTO_Character::LoseGripAndFall, Duration, false);
	}
}

void AKOA_PROTO_Character::ClearWallHoldTimer() {
	if (GetWorldPtr()) GetWorldPtr()->GetTimerManager().ClearTimer(WallHoldTimer);
}

void AKOA_PROTO_Character::ClearWallSlideTimer() {
	if (WorldPtr) WorldPtr->GetTimerManager().ClearTimer(WallSlideTimer);
}