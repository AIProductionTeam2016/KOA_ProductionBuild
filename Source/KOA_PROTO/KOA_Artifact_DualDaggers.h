// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KOA_BASE_Artifact.h"
#include "KOA_Artifact_DualDaggers.generated.h"

/**
 * 
 */
class AKOA_PROTO_Character;

UCLASS()
class KOA_PROTO_API UKOA_Artifact_DualDaggers : public UKOA_BASE_Artifact
{
	GENERATED_BODY()
public: // Methods
	UKOA_Artifact_DualDaggers(const FObjectInitializer& ObjectInitializer);
	void Tick(float DeltaTime);

	// Press Ability
	void PressAbilityQ();
	void PressAbilityW();
	void PressAbilityE();
	void PressAbilityR();
	
	// Release Ability
	void ReleaseAbilityQ();
	void ReleaseAbilityW();
	void ReleaseAbilityE();
	void ReleaseAbilityR();

	// GETTERS //
	FORCEINLINE FVector GetCurrentECapsuleLocation() const {
		return CurrentCapsuleLocation;
	}

	// SETTERS //
	void SetCurrentCapsuleLocation(const FVector& location) {
		CurrentCapsuleLocation = location;
	}
private: // Variables
	FVector CurrentCapsuleLocation;
private: // Methods
	void E_LocationSwap();
};
