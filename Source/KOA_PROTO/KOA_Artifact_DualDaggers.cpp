// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_Artifact_DualDaggers.h"
#include "KOA_PROTO_Character.h"

UKOA_Artifact_DualDaggers::UKOA_Artifact_DualDaggers(const FObjectInitializer& ObjectInitializer) 
	: Super() {
	ArtifactName = "Dual Daggers of Something, Probably";
	AbilityQ.AbilityName = "Vampire's Kiss with Blood; Not Sparkles";
	//AbilityQ.ResetAbilityCooldown();
	E_MaxTargetRange = 1000.0;

	// Set a reference to the player
	PlayerReference = nullptr;
}

void UKOA_Artifact_DualDaggers::PressAbilityQ() {
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed DualDagger::Vampire's Kiss");
	GetPlayerReference();
}

void UKOA_Artifact_DualDaggers::ReleaseAbilityQ() {
	//if (E_AimingMesh) {
	//	E_AimingMesh->SetVisibility(false);
	//}
}

void UKOA_Artifact_DualDaggers::Tick(float DeltaTime) {
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "DualDaggers::Tick()");
}

AKOA_PROTO_Character* UKOA_Artifact_DualDaggers::GetPlayerReference() {
	if (PlayerReference == nullptr) {
		PlayerReference = Cast<AKOA_PROTO_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (PlayerReference) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, "Daggers PlayerReference created successfully.");
	}
	else {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Red, "ERROR: Daggers PlayerReference FAILED!");
	}
	return PlayerReference;
}