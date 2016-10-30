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
	TargetLocation = FVector(0, 0, 0);
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
		hasExceededMaxRange = false;
	}
	else if (ProjTrajectory == EProjectileTrajectory::PT_PARABOLIC)
	{
		velocity = StartVelocity * FVector(0, 1, 1);
		startPos = this->GetActorLocation();
		hasExceededMaxRange = false;
	}
}

// Called every frame
void ABASE_Projectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (ProjTrajectory == EProjectileTrajectory::PT_LINEAR)
	{
		if (!hasExceededMaxRange)
		{
			FHitResult outSweepHitResult;
			FVector newLocation = this->GetActorLocation() + velocity * DeltaTime;
			SetActorLocation(newLocation, true, &outSweepHitResult);
			if (ProjMaxRange != 0 && (newLocation - startPos).Size() >= ProjMaxRange)
			{
				OnReachedMaxDistance();
				hasExceededMaxRange = true;
			}
		}
	}
	else if (ProjTrajectory == EProjectileTrajectory::PT_PARABOLIC)
	{
		if (!hasExceededMaxRange)
		{
			velocity += FVector(0, 0, -Gravity) * DeltaTime;
			FHitResult outSweepHitResult;
			FVector newLocation = this->GetActorLocation() + velocity * DeltaTime;
			SetActorLocation(newLocation, true, &outSweepHitResult);
			if (ProjMaxRange != 0 && (newLocation - startPos).Size() >= ProjMaxRange)
			{
				OnReachedMaxDistance();
				hasExceededMaxRange = true;
			}
		}
	}
}

void ABASE_Projectile::GetProjectileVelocity(FVector& outVelocity)
{
	outVelocity = velocity;
}