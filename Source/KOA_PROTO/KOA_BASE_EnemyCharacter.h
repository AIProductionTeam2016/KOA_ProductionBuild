// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "KOA_BASE_EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class ETypeOfEnemy : uint8 {
	NORMAL = 0,
	ELITE,
	BOSS,
	NUM,
	NONE = 99
};

UCLASS()
class KOA_PROTO_API AKOA_BASE_EnemyCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// STATS //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	ETypeOfEnemy EnemyID;
	// MOVEMENT //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats|Speed")
	float Speed;

	// HP //
	UPROPERTY(EditAnywhere, Category = "EnemyStats|HP")
	float HPCurr;
	UPROPERTY(EditAnywhere, Category = "EnemyStats|HP")
	float HPMax;
public:
	// Sets default values for this character's properties
	AKOA_BASE_EnemyCharacter(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void ReceiveDamage(float Amount);

	void OnDeath();
	
	// GETTERS //
	UFUNCTION(BlueprintCallable, Category = "EnemyStats|HP")
	float GetHPCurr() const {
		return HPCurr;
	}
	UFUNCTION(BlueprintCallable, Category = "EnemyStats|HP")
	float GetHPMax() const {
		return HPMax;
	}
	// SETTERS //
	void SetHPCurr(float AmountHP);

	// Override to update changes made in the editor
	//virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	// PRIVATE GETTERS //
	UFUNCTION(BlueprintCallable, Category = "EnemyStats|Status")
	bool GetIsDead() const {
		return IsDead;
	}
private:
	bool IsDead;
};
