// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "KOA_BASE_Artifact.generated.h"

/**
 * 
 */
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

USTRUCT()
struct KOA_PROTO_API FAbility {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString AbilityName;

	UPROPERTY(EditAnywhere, Category = "Texture")
	UTexture* AbilityIconTexture;
	
	UPROPERTY(EditAnywhere, Category = "Timer", DisplayName = "Ability Cooldown")
	float AbilityCooldownDuration;

	UPROPERTY(EditAnywhere, Category = "Cooldown", DisplayName = "Ability Cooldown")
	bool AbilityOnCooldown;
	FTimerHandle AbilityCooldownTimer;
public:
	FAbility() {
		AbilityName = "INVALID";
		AbilityCooldownDuration = 0.0f;
		AbilityOnCooldown = false;
	}

	//FORCEINLINE FTimerHandle GetAbilityCooldownTimerHandle() const {
	//	return AbilityCooldownTimer;
	//}

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
	
//private:
	
	
};

UCLASS(Blueprintable)
class KOA_PROTO_API UKOA_BASE_Artifact : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Information")
	FString ArtifactName;
	
	//TODO: Add in a mesh component for the Artifact

	UPROPERTY(EditAnywhere, Category = "Ability")
	FAbility AbilityQ;
	UPROPERTY(EditAnywhere, Category = "Ability")
	FAbility AbilityW;
	UPROPERTY(EditAnywhere, Category = "Ability")
	FAbility AbilityE;
	UPROPERTY(EditAnywhere, Category = "Ability")
	FAbility AbilityR;

	FTimerHandle AbilityQTimer;
	FTimerHandle AbilityWTimer;
	FTimerHandle AbilityETimer;
	FTimerHandle AbilityRTimer;

public:
	UKOA_BASE_Artifact();
	virtual ~UKOA_BASE_Artifact();

	void Tick(float DeltaTime) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Blue, "BASE_Artifact::Tick");
	}

	virtual void PressAbilityQ();
	virtual void PressAbilityW();
	virtual void PressAbilityE();
	virtual void PressAbilityR();
	
	virtual void ReleaseAbilityQ();
	virtual void ReleaseAbilityW();
	virtual void ReleaseAbilityE();
	virtual void ReleaseAbilityR();

	void ResetAbilityQCooldown();

private:
	

};
