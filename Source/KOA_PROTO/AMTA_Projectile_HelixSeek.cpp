// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "AMTA_Projectile_HelixSeek.h"

static const float VERY_SMALL_VALUE = 0.01f; //used to prevent the projectile from moving to crazily at the end of its lifetime

AAMTA_Projectile_HelixSeek::AAMTA_Projectile_HelixSeek() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// INIT //
	ProjDamage = 0.0f;
	ProjMaxRange = 0.0f;
	ProjLifeTime = 0.0f;
	ProjTimeBeforeDelete = 0.0f;
	TargetLocation = FVector(0.0, 0.0, 0.0);
	ProjTrajectory = EProjectileTrajectory::NONE;
	ProjMesh = nullptr;
}

void AAMTA_Projectile_HelixSeek::BeginPlay() {
	Super::BeginPlay();
	startLocation = this->GetActorLocation();
	existedTime = 0;
}

void AAMTA_Projectile_HelixSeek::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (existedTime == 0)
	{
		DoHelixSeekMovement(true, DeltaTime, startLocation, TargetLocation, ProjLifeTime,
			existedTime, 1, 1, 20);
	}
	else if (existedTime <= ProjLifeTime)
	{
		DoHelixSeekMovement(false, DeltaTime, startLocation, TargetLocation, ProjLifeTime,
			existedTime, 1, 1, 20);
	}
	if (existedTime > ProjTimeBeforeDelete)
	{
		this->Destroy();
	}
	existedTime += DeltaTime;
}

void AAMTA_Projectile_HelixSeek::DoHelixSeekMovement(bool firstFrame, float DeltaSeconds, FVector startPos, FVector targetPos, float totalTime,
	float elapsedTime, float startVelMax, float spinMax, float damping)
{
	if (firstFrame)
	{
		this->SetActorLocation(startPos);
	}
	FVector currentPos = this->GetActorLocation();
	float totalLength = (startPos - targetPos).Size();
	//Create the vectors that will make the x and y axis of a new coordinate system, for simplification purposes
	//The x axis will be in the direction of the line from our current position to the targetPos
	FVector xVec = (targetPos - currentPos);
	xVec.Normalize();
	FVector zVec = FVector::CrossProduct(xVec, velocity.GetSafeNormal());
	if (zVec == FVector::ZeroVector)
		zVec = FVector::CrossProduct(xVec, FVector(1, 0, 0));
	zVec.Normalize();
	FVector yVec = FVector::CrossProduct(xVec, zVec);
	yVec.Normalize();

	//Get our y velocity in our new coordinate system;
	float yVel = FVector::DotProduct(velocity, yVec);

	//If it's the first frame, set the y velocity to add a "curve" to the trajectory
	if (firstFrame)
	{
		yVel = FMath::FRandRange(-startVelMax, startVelMax) * totalLength / totalTime;
		spin = FMath::FRandRange(-spinMax, spinMax) / totalTime;
	}

	//Set our x velocity based on the distance to the target and the remaining time
	float xVel = (targetPos - currentPos).Size() / (totalTime - elapsedTime + VERY_SMALL_VALUE);

	//Set our z velocity based on spin and time left
	float zVel = ((totalTime - elapsedTime) / totalTime) * yVel * spin / 10;

	//Dampen y velocity
	yVel /= FMath::Pow(1 + damping, DeltaSeconds);

	//convert our new coordinate system velocity back to the world coordinate system, and store it in velocity
	velocity = xVec * xVel + yVec * yVel + zVec * zVel;
	//apply velocity
	this->SetActorLocation(currentPos + velocity * DeltaSeconds);
}