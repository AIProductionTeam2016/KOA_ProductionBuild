// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_BASE_Artifact.h"
#include "KOA_PROTO_Character.h"

UKOA_BASE_Artifact::UKOA_BASE_Artifact() {
	ArtifactName = "INVALID";

	// Initialize Reference to the Player
	PlayerReference = nullptr;
}

UKOA_BASE_Artifact::~UKOA_BASE_Artifact() {}

void UKOA_BASE_Artifact::PressAbilityQ() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Red, "BASE_Artifact::PressedAbilityQ()");
}
void UKOA_BASE_Artifact::PressAbilityW() {}
void UKOA_BASE_Artifact::PressAbilityE() {}
void UKOA_BASE_Artifact::PressAbilityR() {}

void UKOA_BASE_Artifact::ReleaseAbilityQ() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Red, "BASE_Artifact::ReleasedAbilityQ()");
}
void UKOA_BASE_Artifact::ReleaseAbilityW() {}
void UKOA_BASE_Artifact::ReleaseAbilityE() {}
void UKOA_BASE_Artifact::ReleaseAbilityR() {}

void UKOA_BASE_Artifact::ResetAbilityQCooldown() {
	AbilityQ.ResetAbilityCooldown();
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "Reset Current Q timer");
}

/*-- GETTERS --*/
AKOA_PROTO_Character* UKOA_BASE_Artifact::GetPlayerReference() {
	if (PlayerReference == nullptr) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Red, "FATAL ERROR: VampDaggers PlayerRef* == nullptr");
		return nullptr;
	} else {
		return PlayerReference;
	}
}
/*-- SETTERS --*/
void UKOA_BASE_Artifact::SetPlayerReference(AKOA_PROTO_Character* player) {
	PlayerReference = player;
}