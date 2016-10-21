// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BASE_Projectile.h"
#include "AMTA_Projectile_Bouncing.generated.h"

/**
 * 
 */
UCLASS()
class KOA_PROTO_API AAMTA_Projectile_Bouncing : public ABASE_Projectile
{
	GENERATED_BODY()
	
public:
	// STATS //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int ProjMaxBounces;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ProjLifetime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ProjBounceHeightMult;
	
public:
	// Sets default values for this actor's properties
	AAMTA_Projectile_Bouncing();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndOfLifetime();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBounce(int bounceNum, FHitResult hitResult);
	UFUNCTION(BlueprintImplementableEvent)
	void OnLastBounce(FHitResult hitResult);

protected:
	int nTimesBounced;
	float timeExisted;
	bool hasReachedMaxBounces;
	bool hasExceededLifetime;
};
