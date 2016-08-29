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
	void PressAbilityQ();
	void ReleaseAbilityQ();
	void Tick(float DeltaTime);
	
	// Getters
	UFUNCTION(BlueprintCallable, Category="Ability,E")
	float GetE_MaxTargetRange() const {
		return E_MaxTargetRange;
	}

	AKOA_PROTO_Character* GetPlayerReference();

private:
	AKOA_PROTO_Character* PlayerReference;
};
