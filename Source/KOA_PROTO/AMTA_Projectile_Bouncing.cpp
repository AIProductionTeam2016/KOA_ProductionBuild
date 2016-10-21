// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "AMTA_Projectile_Bouncing.h"



AAMTA_Projectile_Bouncing::AAMTA_Projectile_Bouncing() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// INIT //
	ProjDamage = 0.0f;
	ProjMaxRange = 0.0f;
	Gravity = 0.0f;
	StartVelocity = FVector(0, 0, 0);
	ProjTrajectory = EProjectileTrajectory::NONE;
	ProjMesh = nullptr;
	ProjMaxBounces = 0;
	ProjLifetime = 0;
}

// Called when the game starts or when spawned
void AAMTA_Projectile_Bouncing::BeginPlay() {
	Super::BeginPlay();
	velocity = StartVelocity * FVector(0, 1, 1);
	startPos = this->GetActorLocation();
	hasExceededMaxRange = false;
	hasReachedMaxBounces = false;
	hasExceededLifetime = false;
	nTimesBounced = 0;
	timeExisted = 0;
}

// Called every frame
void AAMTA_Projectile_Bouncing::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!hasExceededMaxRange && 
		(nTimesBounced < ProjMaxBounces || ProjMaxBounces == 0) && 
		(timeExisted < ProjLifetime || ProjLifetime == 0))
	{
		velocity += FVector(0, 0, -Gravity) * DeltaTime;
		FHitResult outSweepHitResult;
		FVector newLocation = this->GetActorLocation() + velocity * DeltaTime;
		bool didHit = SetActorLocation(newLocation, true, &outSweepHitResult);
		if (ProjMaxRange != 0 && (newLocation - startPos).Size() >= ProjMaxRange)
		{
			OnReachedMaxDistance();
			hasExceededMaxRange = true;
		}
	}
	timeExisted += DeltaTime;
}

