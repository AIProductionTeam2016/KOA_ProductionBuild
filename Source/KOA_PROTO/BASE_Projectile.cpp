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
	ProjLifeTime = 0.0f;
	TargetLocation = FVector(0.0,0.0,0.0);
	ProjTrajectory = EProjectileTrajectory::NONE;
	ProjMesh = nullptr;
}

// Called when the game starts or when spawned
void ABASE_Projectile::BeginPlay() {
	Super::BeginPlay();
	startLocation = this->GetActorLocation();
	existedTime = 0;
	if (ProjTrajectory == EProjectileTrajectory::PT_SQUIGGLY)
	{
		DoSquiggleMovement(true, 0, startLocation, TargetLocation, ProjLifeTime,
			0, 30, 60, 0.4f, squigglyArcHeight);
	}
}

// Called every frame
void ABASE_Projectile::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
	existedTime += DeltaTime;
	if (ProjTrajectory == EProjectileTrajectory::PT_SQUIGGLY)
	{
		DoSquiggleMovement(false, DeltaTime, startLocation, TargetLocation, ProjLifeTime,
			existedTime, 30, 60, 0.4f, squigglyArcHeight);
	}
	if (existedTime > ProjLifeTime)
	{
		this->Destroy();
	}
}

void ABASE_Projectile::DoSquiggleMovement(bool firstFrame, float DeltaSeconds, FVector startPos, FVector targetPos, float totalTime,
	float elapsedTime, float minAngle, float maxAngle, float arcWidth, float &arcHeight)
{
	FVector previousPos = this->GetActorLocation();
	//Calculate value between 0 and 1 saying how far we should be between the startPos and TargetPos
	float alpha = elapsedTime / totalTime;
	FVector nextPos = FMath::Lerp(startPos, targetPos, alpha);
	velocity = previousPos - nextPos;
	this->SetActorLocation(nextPos);
}
