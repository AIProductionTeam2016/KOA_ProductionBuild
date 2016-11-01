// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KOA_BASE_Artifact.h"
#include "KOA_Artifact_DualDaggers.generated.h"

/**
 * 
 */
class AKOA_PROTO_Character;

UCLASS(meta = (ShowWorldContextPin))
class KOA_PROTO_API UKOA_Artifact_DualDaggers : public UKOA_BASE_Artifact
{
	GENERATED_BODY()
public: // Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Stats")
	float ABILQ_HealAmount;
	
public: // Methods
	UKOA_Artifact_DualDaggers(const FObjectInitializer& ObjectInitializer);
	void Tick(float DeltaTime);

	// Extra Stats
	UFUNCTION(BlueprintCallable, Category = "Ability|Stats")
	float GetABILQHealAmount() const {
		return ABILQ_HealAmount;
	}
	// Basic Attacks
	void UseLightAttack();

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
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Attacks/Ablities")
	void OnPressAbilityE();
	UFUNCTION(BlueprintImplementableEvent, Category = "Attacks/Ablities")
	void OnReleaseAbilityE();

	// GETTERS //
	FORCEINLINE FVector GetCurrentECapsuleLocation() const {
		return CurrentCapsuleLocation;
	}

	// SETTERS //
	void SetCurrentCapsuleLocation(const FVector& location) {
		CurrentCapsuleLocation = location;
	}

	UFUNCTION(BlueprintCallable, Category = "World Context Object")
		AKOA_PROTO_Character* GetPlayer();

private: // Variables
	FVector CurrentCapsuleLocation;
private: // Methods
	void E_LocationSwap();
};
