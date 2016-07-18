// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_Artifact_FireGlove.h"

UKOA_Artifact_FireGlove::UKOA_Artifact_FireGlove() : Super() {
	ArtifactName = "Definitely NOT the Pyromancy Glove";
	AbilityQ.AbilityName = "Kamehame-FI-ha Ball";
}

void UKOA_Artifact_FireGlove::PressAbilityQ() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "You pressed FireGlove::Q");
}

