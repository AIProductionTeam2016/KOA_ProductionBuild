// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ATMA_StatusEffects.h"
#include "BASE_Projectile.generated.h"

UENUM(BlueprintType)
enum class EProjectileTrajectory : uint8 {
	PT_LINEAR = 0 	UMETA(DisplayName="Linear"),
	PT_PARABOLIC	UMETA(DisplayName="Parabola"),
	PT_SQUIGGLY		UMETA(DisplayName="Squiggly"),
	PT_FIXEDSEEK    UMETA(DisplayName="FixedTimeSeek"),
	PT_HELIXSEEK    UMETA(DisplayName = "FixedSeekHelix"),
	NUM				UMETA(Hidden),
	NONE			UMETA(DisplayName="None")
};

UCLASS()
class KOA_PROTO_API ABASE_Projectile : public AActor
{
	GENERATED_BODY()
public:
	// STATS //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ProjDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ProjMaxRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ProjLifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ProjTimeBeforeDelete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FVector TargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EProjectileTrajectory ProjTrajectory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|StatusEffects")
	FStatusEffects ProjStatusEffects;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* ProjMesh;
public:	
	// Sets default values for this actor's properties
	ABASE_Projectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private: 
	float Gravity;

	void DoSquiggleMovement(bool firstFrame, float DeltaSeconds, FVector startPos, FVector targetPos, float totalTime,
		float elapsedTime, float forceMultMin, float forceMultMax, float frequency, float startVel, float &forceMult);

	void DoFixedSeekMovement(bool firstFrame, float DeltaSeconds, FVector startPos, FVector targetPos, float totalTime,
		float elapsedTime, float startVelMax, float damping);

	void DoHelixSeekMovement(bool firstFrame, float DeltaSeconds, FVector startPos, FVector targetPos, float totalTime,
		float elapsedTime, float startVelMax, float spinMax, float damping);

protected:
	FVector velocity;
	FVector startLocation;
	float existedTime;
	float squigglyArcHeight;
	float spin;
};
