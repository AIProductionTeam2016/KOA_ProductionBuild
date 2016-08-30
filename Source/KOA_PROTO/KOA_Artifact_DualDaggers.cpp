// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_Artifact_DualDaggers.h"
#include "KOA_PROTO_Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UTIL_MouseFunctionality.h"

UKOA_Artifact_DualDaggers::UKOA_Artifact_DualDaggers(const FObjectInitializer& ObjectInitializer) 
	: Super() {
	ArtifactName = "Dual Daggers of Something, Probably";
	AbilityQ.AbilityName = "Vampire's Kiss with Blood; Not Sparkles";
	AbilityQ.MaxCastRange = 400.0f;
	//AbilityQ.ResetAbilityCooldown();
	E_MaxTargetRange = 1000.0;
}

void UKOA_Artifact_DualDaggers::PressAbilityQ() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed DualDagger::Vampire's Kiss");
	AKOA_PROTO_Character* player = GetPlayerReference();
	player->VD_E_AimingMeshComponent->SetVisibility(true);
	//->VD_E_AimingMeshComponent->SetVisibility(true);
	//VD_E_AimingMeshComponent
}

void UKOA_Artifact_DualDaggers::ReleaseAbilityQ() {
	// Set the aiming mesh component visibility to false
	AKOA_PROTO_Character* player = GetPlayerReference();
	E_LocationSwap();
	player->VD_E_AimingMeshComponent->SetVisibility(false);
}

void UKOA_Artifact_DualDaggers::Tick(float DeltaTime) {
	switch (GetCurrentHeldAbilityButton()) {
	case EAbilityID::ABID_Q:  
	{	
		// Get a reference to the player
		AKOA_PROTO_Character* player = GetPlayerReference();
		// Get the mousePos using the UTILITY function
		FVector mousePos = UTIL_MouseFunctionality::GetMousePosInPlayerPlane(player->GetWorldPtr());

		// Get the playerPos
		FVector playerPos = player->GetActorLocation();
		// Check distance from player to mouse
		float distFromPlayerToMouse = FVector::Dist(playerPos, mousePos);
		
		// Initialize the finalPos
		FVector finalPos;
		// If the dist is less than the MaxCastRange
		if (distFromPlayerToMouse < AbilityQ.MaxCastRange) {
			finalPos = mousePos;
		} else {
			// Make it so the AimingCapsule cant go beyond the MaxCastRange
			FVector vectorFromPlayerToMouse = FVector(mousePos - playerPos);
			vectorFromPlayerToMouse.Normalize();
			finalPos = playerPos + vectorFromPlayerToMouse * AbilityQ.MaxCastRange;
		}

		// Make sure the AimingCapsule is not lower in the world than the player.
		// This prevents the player from spawning inthe ground and other issues.
		if (finalPos.Z < playerPos.Z) finalPos.Z = playerPos.Z;

		// Set the VD_E_AimingCapsule to the finalPos
		SetCurrentCapsuleLocation(finalPos);
		player->VD_E_AimingCapsule->SetWorldLocation(finalPos);
	}
		break;
	case EAbilityID::ABID_W:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, "Using Dagger: W");
		break;
	case EAbilityID::ABID_E:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, "Using Dagger: E");
		break;
	case EAbilityID::ABID_R:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, "Using Dagger: R");
		break;
	default:
		break;
	}
}

void UKOA_Artifact_DualDaggers::E_LocationSwap() {
	// Set the player to the mouse position
	AKOA_PROTO_Character* player = GetPlayerReference();
	player->SetActorLocation(GetCurrentCapsuleLocation());
}