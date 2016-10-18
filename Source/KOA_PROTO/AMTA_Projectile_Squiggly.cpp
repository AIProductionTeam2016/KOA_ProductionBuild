// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "AMTA_Projectile_Squiggly.h"

static const float SMALL_VALUE_SQUIGGLY = 0.2f; //used to prevent the projectile from moving to crazily at the end of its lifetime
static const float VERY_SMALL_VALUE = 0.01f; //used to prevent the projectile from moving to crazily at the end of its lifetime
static const float ACCEL_CLAMP = 1000000; //clamp the acceleration to prevent crazy movement

AAMTA_Projectile_Squiggly::AAMTA_Projectile_Squiggly() {
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

void AAMTA_Projectile_Squiggly::BeginPlay() {
	Super::BeginPlay();
	startLocation = this->GetActorLocation();
	existedTime = 0;
}

void AAMTA_Projectile_Squiggly::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (existedTime == 0)
	{
		DoSquiggleMovement(true, DeltaTime, startLocation, TargetLocation, ProjLifeTime,
			existedTime, 0.4, 3, 0.5, 2.0f, squigglyArcHeight);
	}
	else if (existedTime <= ProjLifeTime)
	{
		DoSquiggleMovement(false, DeltaTime, startLocation, TargetLocation, ProjLifeTime,
			existedTime, 0.4, 3, 0.5, 2.0f, squigglyArcHeight);
	}
	if (existedTime > ProjTimeBeforeDelete)
	{
		this->Destroy();
	}
	existedTime += DeltaTime;
}

void AAMTA_Projectile_Squiggly::DoSquiggleMovement(bool firstFrame, float DeltaSeconds, FVector startPos, FVector targetPos, float totalTime,
	float elapsedTime, float forceMultMin, float forceMultMax, float frequency, float startVel, float &forceMult)
{
	if (firstFrame)
	{
		this->SetActorLocation(startPos);
	}
	float totalLength = (startPos - targetPos).Size();
	FVector currentPos = this->GetActorLocation();
	//Create the vectors that will make the x and y axis of a new coordinate system, for simplification purposes
	//The x axis will be in the direction of the line from the startPos to the targetPos
	FVector xVec = (targetPos - startPos);
	xVec.Normalize();
	FVector yVec = FVector::CrossProduct(xVec, FVector(1, 0, 0));
	yVec.Normalize();

	//Get our y velocity in our new coordinate system;
	float yVel = FVector::DotProduct(velocity, yVec);
	//If it's the first frame, set our y velocity based on the distance to the target
	if (firstFrame)
	{
		int randomlyNegative = FMath::RandBool() ? -1 : 1;
		yVel = startVel * randomlyNegative * totalLength / totalTime;
	}

	//Get the distance from ourselves to the startpos to targetpos line
	float yPos = FVector::DotProduct(currentPos - startPos, yVec);

	//Get the previousPos projected onto the line from the startPos to the targetPos
	FVector projectedPos = currentPos - (yVec * yPos);

	//Set our x velocity based on the distance to the target and the remaining time
	float xVel = (targetPos - projectedPos).Size() / (totalTime - elapsedTime + VERY_SMALL_VALUE);
	//Make xVel negative if the target moves behind
	if (FVector::DotProduct(targetPos - projectedPos, xVec) < 0)
		xVel = -xVel;

	//Find the acceleration to add to yVel
	float targetAccel = yPos * frequency * forceMult * totalLength / FMath::Pow(totalTime - elapsedTime + SMALL_VALUE_SQUIGGLY, 2) / totalTime;

	targetAccel = FMath::Clamp(targetAccel, -ACCEL_CLAMP, ACCEL_CLAMP);

	//Add the acceleration
	yVel -= targetAccel * DeltaSeconds;

	//convert our new coordinate system velocity back to the world coordinate system, and store it in velocity
	velocity = xVec * xVel + yVec * yVel;
	//apply velocity
	this->SetActorLocation(currentPos + velocity * DeltaSeconds);

	// Check what the value of "ypos" would be if it was calculated after we moved
	float newYPos = FVector::DotProduct(this->GetActorLocation() - startPos, yVec);

	//If we cross the center line, randomize forceMult
	if (FMath::Sign(yPos) != FMath::Sign(newYPos))
	{
		forceMult = FMath::FRandRange(forceMultMin, forceMultMax);
	}
}