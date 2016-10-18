// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BASE_Projectile.h"
#include "AMTA_Projectile_Squiggly.generated.h"

/**
 * 
 */
UCLASS()
class KOA_PROTO_API AAMTA_Projectile_Squiggly : public ABASE_Projectile
{
	GENERATED_BODY()
	
public:
	//STATS//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ProjLifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ProjTimeBeforeDelete;

public:
	AAMTA_Projectile_Squiggly();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	
private:
	void DoSquiggleMovement(bool firstFrame, float DeltaSeconds, FVector startPos, FVector targetPos, float totalTime,
		float elapsedTime, float forceMultMin, float forceMultMax, float frequency, float startVel, float &forceMult);
	
protected:
	FVector velocity;
	FVector startLocation;
	float existedTime;
	float squigglyArcHeight;
};
