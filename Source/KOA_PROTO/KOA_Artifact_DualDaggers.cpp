// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_Artifact_DualDaggers.h"
#include "KOA_PROTO_Character.h"

UKOA_Artifact_DualDaggers::UKOA_Artifact_DualDaggers(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) {
	ArtifactName = "Dual Daggers of Something, Probably";
	AbilityQ.AbilityName = "Vampire's Kiss with Blood; Not Sparkles";
	//AbilityQ.ResetAbilityCooldown();
	E_MaxTargetRange = 1000.0;

	// Set a reference to the player
	//PlayerReference = Cast<AKOA_PROTO_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	// Set a reference to the E_AimingMesh
	//for (TObjectIterator<AKOA_PROTO_Character> Itr; Itr; ++Itr) {
	//	if (Itr->VD_E_AimingMeshComponent) {
	//		E_AimingMeshComponent = Itr->VD_E_AimingMeshComponent;
	//	}
	//}
	//E_AimingMeshComponent = PlayerReference->VD_E_AimingMeshComponent;
	E_AimingMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMesh>(this, TEXT("E_AimingMesh"));
	//E_AimingMeshComponent->AttachTo(RootComponent);
}

void UKOA_Artifact_DualDaggers::PressAbilityQ() {
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed DualDagger::Vampire's Kiss");
	if (E_AimingMesh) {
		E_AimingMesh->SetVisibility(true);
	}
	else {
		if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "E_AimingMeshComponent == NULLPTR");
	}
}
void UKOA_Artifact_DualDaggers::ReleaseAbilityQ() {
	if (E_AimingMesh) {
		E_AimingMesh->SetVisibility(false);
	}
}
void UKOA_Artifact_DualDaggers::Tick(float DeltaTime) {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "DualDaggers::Tick()");
}