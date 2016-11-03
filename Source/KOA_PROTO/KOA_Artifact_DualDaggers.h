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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Stats")
	float ABILR_SquigglyTendrilLifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Stats")
	float ABILR_HelixTendrilLifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Stats")
	float ABILR_HealAmount;

	// Actor references
	UPROPERTY(BlueprintReadOnly, Category = "Ability|Actor References")
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
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Events")
	void OnPressAbilityE();
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Events")
	void OnReleaseAbilityE();
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Events")
	void OnPressAbilityR();
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Events")
	void OnReleaseAbilityR();
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Tick")
	void OnTick(float deltaTime);
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|BloodStorm")
	void OnBloodStormDamage();
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|BloodStorm")
	void OnBloodStormHeal();

	// Spawning Projectiles
	UFUNCTION(BlueprintCallable, Category = "Ability|Projectile Spawning")
	AActor* SpawnSquigglyProjectile(FVector startPosition);
	UFUNCTION(BlueprintCallable, Category = "Ability|Projectile Spawning")
	AActor* SpawnHelixProjectile(FVector startPosition);

	// GETTERS //
	FORCEINLINE FVector GetCurrentECapsuleLocation() const {
		return CurrentCapsuleLocation;
	}

	// SETTERS //
	void SetCurrentCapsuleLocation(const FVector& location) {
		CurrentCapsuleLocation = location;
	}

	UFUNCTION(BlueprintPure, Category = "Ability|Player")
		AKOA_PROTO_Character* GetPlayer();

private: // Variables
	FVector CurrentCapsuleLocation;
	TSubclassOf<AActor> BloodStormSphereClass;
	bool bBloodStormSphereFound;
	TSubclassOf<AActor> SquigglyProjClass;
	bool bSquigglyProjFound; 
	TSubclassOf<AActor> HelixProjClass;
	bool bHelixProjFound;

	FTimerHandle hnd_BS_TimerBeforeDamage;
	FTimerHandle hnd_BS_TimerBeforeHeal;
private: // Methods
	void E_LocationSwap();
};
