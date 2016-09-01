// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "KOA_BASE_Artifact.generated.h"

// Forward Declaration of classes
class AKOA_PROTO_Character;

UENUM(BlueprintType)
enum class EArtifactID : uint8 {
	ID_DualDaggers = 0,
	ID_FireGlove,
	ID_WindSpear,
	NUM,
	ID_NULL = 99,
};

UENUM(BlueprintType)
enum class EAbilityID : uint8 {
	ABID_Q = 0,
	ABID_W,
	ABID_E,
	ABID_R,
	NONE = 99
};

UENUM(BlueprintType)
enum class EBasicAttack : uint8 {
	LIGHT = 0,
	HEAVY,
	NUM,
	NONE = 99
};

USTRUCT()
struct KOA_PROTO_API FAbility {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString AbilityName;

	UPROPERTY(EditAnywhere, Category = "Texture")
	UTexture* AbilityIconTexture;
	
	UPROPERTY(EditAnywhere, Category = "Cast Range")
	float MaxCastRange;

	UPROPERTY(EditAnywhere, Category = "Timer", DisplayName = "Ability Cooldown")
	float AbilityCooldownDuration;

	UPROPERTY(EditAnywhere, Category = "Cooldown", DisplayName = "Ability Cooldown")
	bool AbilityOnCooldown;
	FTimerHandle AbilityCooldownTimer;
public:
	FAbility() {
		AbilityName = "INVALID";
		MaxCastRange = 0.0f;
		AbilityCooldownDuration = 3.0f;
		AbilityOnCooldown = false;
	}

	FORCEINLINE bool IsAbilityOnCooldown() const {
		return AbilityOnCooldown;
	}

	void SetAbilityOnCooldown() {
		AbilityOnCooldown = true;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, AbilityName + TEXT(" cooldown set!"));
	}
	void ResetAbilityCooldown() {
		AbilityOnCooldown = false;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, AbilityName + TEXT(" cooldown reset!"));
	}
	
};

UCLASS(Blueprintable)
class KOA_PROTO_API UKOA_BASE_Artifact : public UObject
{
	GENERATED_BODY()
public:
	//** STATS **//
	UPROPERTY(EditAnywhere, Category = "Stats")
	FString ArtifactName;
	UPROPERTY(EditAnywhere, Category = "Stats|Combat")
	float LightBasicAttackLockDuration;
	//UPROPERTY(EditAnywhere, Category = "Stats|Combat")
	//float HeavyBasicAttackLockDuration;

	//** ABILITIES **//
	UPROPERTY(EditAnywhere, Category = "Ability")
	FAbility AbilityQ;
	UPROPERTY(EditAnywhere, Category = "Ability")
	FAbility AbilityW;
	UPROPERTY(EditAnywhere, Category = "Ability")
	FAbility AbilityE;
	UPROPERTY(EditAnywhere, Category = "Ability")
	FAbility AbilityR;

	//** TIMERS **//
	FTimerHandle BasicAttackTimer;
	FTimerHandle AbilityQTimer;
	FTimerHandle AbilityWTimer;
	FTimerHandle AbilityETimer;
	FTimerHandle AbilityRTimer;

public:
	UKOA_BASE_Artifact();
	virtual ~UKOA_BASE_Artifact();
	// TICK //
	virtual void Tick(float DeltaTime) {}
	// USE BASIC ATTACK //
	virtual void UseLightAttack();
	// PRESS ABILITY //
	virtual void PressAbilityQ();
	virtual void PressAbilityW();
	virtual void PressAbilityE();
	virtual void PressAbilityR();
	// RELEASE ABILITY //
	virtual void ReleaseAbilityQ();
	virtual void ReleaseAbilityW();
	virtual void ReleaseAbilityE();
	virtual void ReleaseAbilityR();
	// RESET BASIC ATTACK //
	void ResetBasicAttackCooldown();
	// RESET ABILITY //
	void ResetAbilityQCooldown();
	void ResetAbilityWCooldown();
	void ResetAbilityECooldown();
	void ResetAbilityRCooldown();
	// START TIMERS //
	void StartAbilityCooldownTimer(EAbilityID AbilityID);
	void StartBasicAttackCooldownTimer(EBasicAttack TypeOfBA);

	/*-- GETTERS --*/ 
	FORCEINLINE bool GetIsBasicAttackOnCooldown() const {
		return IsBasicAttackOnCooldown;
	}
	UFUNCTION(BlueprintCallable, Category = "BasicAttack")
	EBasicAttack GetBasicAttackInUse() const {
		return BasicAttackInUse;
	}
	FORCEINLINE EAbilityID GetCurrentHeldAbilityButton() const {
		return CurrentHeldAbilityButton;
	}
	AKOA_PROTO_Character* GetPlayerReference();

	/*-- SETTERS --*/ 
	UFUNCTION(BlueprintCallable, Category = "BasicAttack")
	void SetBasicAttackInUse(EBasicAttack TypeOfBA);
	void SetCurrentHeldAbilityButton(EAbilityID ability);
	void SetPlayerReference(AKOA_PROTO_Character* player);

private: // Variables
	AKOA_PROTO_Character* PlayerReference;
	EAbilityID CurrentHeldAbilityButton;
	bool IsBasicAttackOnCooldown;
	EBasicAttack BasicAttackInUse;
};
