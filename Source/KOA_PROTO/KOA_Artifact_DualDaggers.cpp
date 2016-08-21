// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_Artifact_DualDaggers.h"

UKOA_Artifact_DualDaggers::UKOA_Artifact_DualDaggers() : Super() {
	ArtifactName = "Dual Daggers of Something, Probably";
	AbilityQ.AbilityName = "Vampire's Kiss with Blood; Not Sparkles";
	//AbilityQ.ResetAbilityCooldown();
	E_MaxTargetRange = 1000.0;
}

void UKOA_Artifact_DualDaggers::PressAbilityQ() {
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed DualDagger::Vampire's Kiss");
	

}

//void UKOA_Artifact_DualDaggers::PressAbilityE_Implementation() {
//	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed DualDagger::E");
//}