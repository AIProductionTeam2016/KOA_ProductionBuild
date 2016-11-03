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

	// Actor references
	UPROPERTY(BlueprintReadOnly, Category = "Actor References")
	AActor* BloodStormSphere;
	
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
	
	// Custom Events
	UFUNCTION(BlueprintImplementableEvent, Category = "Attacks/Ablities")
	void OnPressAbilityE();
	UFUNCTION(BlueprintImplementableEvent, Category = "Attacks/Ablities")
	void OnReleaseAbilityE();
	UFUNCTION(BlueprintImplementableEvent, Category = "Attacks/Ablities")
	void OnPressAbilityR();
	UFUNCTION(BlueprintImplementableEvent, Category = "Attacks/Ablities")
	void OnReleaseAbilityR();
	UFUNCTION(BlueprintImplementableEvent, Category = "Tick")
	void OnTick(float deltaTime);

	// Spawning Projectiles
	UFUNCTION(BlueprintCallable, Category = "Projectile Spawning")
	AActor* SpawnSquigglyProjectile();
	UFUNCTION(BlueprintCallable, Category = "Projectile Spawning")
	AActor* SpawnHelixProjectile();

	// GETTERS //
	FORCEINLINE FVector GetCurrentECapsuleLocation() const {
		return CurrentCapsuleLocation;
	}

	// SETTERS //
	void SetCurrentCapsuleLocation(const FVector& location) {
		CurrentCapsuleLocation = location;
	}

	UFUNCTION(BlueprintPure, Category = "World Context Object")
		AKOA_PROTO_Character* GetPlayer();

private: // Variables
	FVector CurrentCapsuleLocation;
	TSubclassOf<AActor> BloodStormSphereClass;
	bool bBloodStormSphereFound;
	TSubclassOf<AActor> SquigglyProjClass;
	bool bSquigglyProjFound; 
	TSubclassOf<AActor> HelixProjClass;
	bool bHelixProjFound;
private: // Methods
	void E_LocationSwap();
};
