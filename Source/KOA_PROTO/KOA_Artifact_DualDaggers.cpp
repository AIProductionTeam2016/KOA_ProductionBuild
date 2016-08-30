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
	player->VD_E_AimingMeshComponent->SetVisibility(false);
}

void UKOA_Artifact_DualDaggers::Tick(float DeltaTime) {
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "DualDaggers::Tick()");
	AKOA_PROTO_Character* player = GetPlayerReference();
	FVector mousePos = UTIL_MouseFunctionality::GetMousePosInPlayerPlane(player->GetWorldPtr());
	

	//TODO: Add a check to make sure ONLY the specific ability is being used
	// Check distance from player to mouse
	//float distFromPlayerToMouse = 
	float distFromPlayerToMouse = FVector::Dist(player->GetActorLocation(), mousePos);
	if (distFromPlayerToMouse < AbilityQ.MaxCastRange) {
		player->VD_E_AimingCapsule->SetWorldLocation(mousePos);
	} else {
		FVector vectorFromPlayerToMouse =FVector(mousePos - player->GetActorLocation());
		vectorFromPlayerToMouse.Normalize();
		FVector finalPos = player->GetActorLocation() + vectorFromPlayerToMouse * AbilityQ.MaxCastRange;
		player->VD_E_AimingCapsule->SetWorldLocation(finalPos);
	}
	
}
