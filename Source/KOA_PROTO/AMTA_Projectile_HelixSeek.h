// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BASE_Projectile.h"
#include "AMTA_Projectile_HelixSeek.generated.h"

/**
 * 
 */
UCLASS()
class KOA_PROTO_API AAMTA_Projectile_HelixSeek : public ABASE_Projectile
{
	GENERATED_BODY()
	
public:
	//STATS//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float ProjLifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float ProjTimeBeforeDelete;

public:
	AAMTA_Projectile_HelixSeek();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	
private:
	void DoHelixSeekMovement(bool firstFrame, float DeltaSeconds, FVector startPos, FVector targetPos, float totalTime,
		float elapsedTime, float startVelMax, float spinMax, float damping);

protected:
	FVector velocity;
	FVector startLocation;
	float existedTime;
	float spin;
};
