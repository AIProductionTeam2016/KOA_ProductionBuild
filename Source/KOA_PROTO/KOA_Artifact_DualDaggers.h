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
public:
	UKOA_Artifact_DualDaggers(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(EditAnywhere, Category = "Ability,E")
	float E_MaxTargetRange;

public:
	void Tick(float DeltaTime);

	// Press Ability
	void PressAbilityQ();
	//void PressAbilityE();

	// Release Ability
	void ReleaseAbilityQ();
	//void ReleaseAbilityE();

	// SETTERS
	UFUNCTION(BlueprintCallable, Category="Ability,E")
	float GetE_MaxTargetRange() const {
		return E_MaxTargetRange;
	}

	FORCEINLINE FVector GetCurrentCapsuleLocation() const {
		return CurrentCapsuleLocation;
	}

	// SETTERS
	void SetCurrentCapsuleLocation(const FVector& location) {
		CurrentCapsuleLocation = location;
	}
private:
	FVector CurrentCapsuleLocation;
private:	
	void E_LocationSwap();
};
