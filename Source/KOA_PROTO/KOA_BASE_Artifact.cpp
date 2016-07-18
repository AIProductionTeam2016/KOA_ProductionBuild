// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_BASE_Artifact.h"

UKOA_BASE_Artifact::UKOA_BASE_Artifact() {
	ArtifactName = "INVALID";
}

UKOA_BASE_Artifact::~UKOA_BASE_Artifact() {

}

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