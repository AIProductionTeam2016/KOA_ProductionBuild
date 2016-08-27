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

	void PressAbilityQ();
	void ReleaseAbilityQ();

	void Tick(float DeltaTime);

	AKOA_PROTO_Character* PlayerReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//USkeletalMeshComponent* E_AimingMeshComponent;
	USkeletalMesh* E_AimingMesh;

	// Getters
	UFUNCTION(BlueprintCallable, Category="Ability,E")
	float GetE_MaxTargetRange() const {
		return E_MaxTargetRange;
	}

	UPROPERTY(EditAnywhere, Category="Ability,E")
	float E_MaxTargetRange;
private:
	
};
