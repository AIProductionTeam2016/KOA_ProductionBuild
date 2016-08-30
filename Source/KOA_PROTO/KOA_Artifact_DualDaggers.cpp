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
	player->VD_E_AimingMeshComponent->SetWorldLocation(mousePos);
}
