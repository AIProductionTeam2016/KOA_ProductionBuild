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
struct KOA_PROTO_API FAbilityStats {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FString AbilityName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture* AbilityIconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HitDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AbilityCooldownDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxCastRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool AbilityOnCooldown;
public:
	FAbilityStats() {
		AbilityName = "INVALID NAME";
		AbilityIconTexture = nullptr;
 		HitDamage = 0.0f;
 	    MaxCastRange = 0.0f;
 		AbilityCooldownDuration = 0.0f;
        AbilityOnCooldown = false;
	}
	
	void operator=(const FAbilityStats& Stats) {
		this->AbilityName = Stats.AbilityName;
		this->AbilityIconTexture = Stats.AbilityIconTexture;
 		this->HitDamage = Stats.HitDamage;
 		this->MaxCastRange = Stats.MaxCastRange;
 		this->AbilityCooldownDuration = Stats.AbilityCooldownDuration;
 		this->AbilityOnCooldown = Stats.AbilityOnCooldown;
	}
};

USTRUCT()
struct KOA_PROTO_API FAbility {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString AbilityName;

	UPROPERTY(EditAnywhere, Category = "Texture")
	UTexture* AbilityIconTexture;
	
	UPROPERTY(EditAnywhere, Category = "Damage")
 	float HitDamage;
	
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
		HitDamage = 0.0f;
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
	
	FAbilityStats GetAbilityStats() const {
		FAbilityStats stats;
		stats.AbilityName = this->AbilityName;
		stats.AbilityIconTexture = this->AbilityIconTexture;
		stats.HitDamage = this->HitDamage;
		stats.MaxCastRange = this->MaxCastRange;
		stats.AbilityCooldownDuration = this->AbilityCooldownDuration;
		stats.AbilityOnCooldown = this->AbilityOnCooldown;
		return stats;
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
	UFUNCTION(BlueprintCallable, Category = "Ability|Stats")
	FAbilityStats GetAbilityStats(EAbilityID Ability) const {
		FAbilityStats stats;
		switch(Ability) {
		case EAbilityID::ABID_Q:
			stats = AbilityQ.GetAbilityStats();
			break;
		case EAbilityID::ABID_W:
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "GetAbilityStats:W");
			stats = AbilityW.GetAbilityStats();
			break;
		case EAbilityID::ABID_E:
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "GetAbilityStats:E");
			stats = AbilityE.GetAbilityStats();
			break;
		case EAbilityID::ABID_R:
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "GetAbilityStats:R");
			stats = AbilityR.GetAbilityStats();
			break;
		default:
			break;
		}
		return stats;
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
