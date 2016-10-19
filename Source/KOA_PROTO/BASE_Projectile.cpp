// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "BASE_Projectile.h"


// Sets default values
ABASE_Projectile::ABASE_Projectile() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// INIT //
	ProjDamage = 0.0f;
	ProjMaxRange = 0.0f;
	Gravity = 0.0f;
	StartVelocity = FVector(0, 0, 0);
	ProjTrajectory = EProjectileTrajectory::NONE;
	ProjMesh = nullptr;
}

// Called when the game starts or when spawned
void ABASE_Projectile::BeginPlay() {
	Super::BeginPlay();
	if (ProjTrajectory == EProjectileTrajectory::PT_LINEAR)
	{
		velocity = StartVelocity * FVector(0,1,1);
		startPos = this->GetActorLocation();
		hasExcededMaxRange = false;
	}
	else if (ProjTrajectory == EProjectileTrajectory::PT_PARABOLIC)
	{
		velocity = StartVelocity * FVector(0, 1, 1);
		startPos = this->GetActorLocation();
		hasExcededMaxRange = false;
	}
}

// Called every frame
void ABASE_Projectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (ProjTrajectory == EProjectileTrajectory::PT_LINEAR)
	{
		if (!hasExcededMaxRange)
		{
			FHitResult outSweepHitResult;
			FVector newLocation = this->GetActorLocation() + velocity * DeltaTime;
			SetActorLocation(newLocation, true, &outSweepHitResult);
			if ((newLocation - startPos).Size() >= ProjMaxRange)
			{
				OnReachedMaxDistance();
				hasExcededMaxRange = true;
			}
		}
	}
	else if (ProjTrajectory == EProjectileTrajectory::PT_PARABOLIC)
	{
		if (!hasExcededMaxRange)
		{
			velocity += FVector(0, 0, -Gravity) * DeltaTime;
			FHitResult outSweepHitResult;
			FVector newLocation = this->GetActorLocation() + velocity * DeltaTime;
			SetActorLocation(newLocation, true, &outSweepHitResult);
			if ((newLocation - startPos).Size() >= ProjMaxRange)
			{
				OnReachedMaxDistance();
				hasExcededMaxRange = true;
			}
		}
	}
}

