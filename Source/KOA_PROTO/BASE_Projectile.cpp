// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "BASE_Projectile.h"

static const float SMALL_VALUE_SQUIGGLY = 0.2f; //used to prevent the projectile from moving to crazily at the end of its lifetime
static const float VERY_SMALL_VALUE = 0.01f; //used to prevent the projectile from moving to crazily at the end of its lifetime
static const float ACCEL_CLAMP = 1000000; //clamp the acceleration to prevent crazy movement
// Sets default values
ABASE_Projectile::ABASE_Projectile() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// INIT //
	ProjDamage = 0.0f;
	ProjMaxRange = 0.0f;
	ProjLifeTime = 0.0f;
	ProjTimeBeforeDelete = 0.0f;
	TargetLocation = FVector(0.0,0.0,0.0);
	ProjTrajectory = EProjectileTrajectory::NONE;
	ProjMesh = nullptr;
}

// Called when the game starts or when spawned
void ABASE_Projectile::BeginPlay() {
	Super::BeginPlay();
	startLocation = this->GetActorLocation();
	existedTime = 0;
}

// Called every frame
void ABASE_Projectile::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
	if (ProjTrajectory == EProjectileTrajectory::PT_SQUIGGLY)
	{
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
	}
	else if (ProjTrajectory == EProjectileTrajectory::PT_FIXEDSEEK)
	{
		if (existedTime == 0)
		{
			DoFixedSeekMovement(true, DeltaTime, startLocation, TargetLocation, ProjLifeTime,
				existedTime, 500, 20);
		}
		else if (existedTime <= ProjLifeTime)
		{
			DoFixedSeekMovement(false, DeltaTime, startLocation, TargetLocation, ProjLifeTime,
				existedTime, 500, 20);
		}
	}
	if (existedTime > ProjTimeBeforeDelete)
	{
		this->Destroy();
	}
	existedTime += DeltaTime;
}

void ABASE_Projectile::DoSquiggleMovement(bool firstFrame, float DeltaSeconds, FVector startPos, FVector targetPos, float totalTime,
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

void ABASE_Projectile::DoFixedSeekMovement(bool firstFrame, float DeltaSeconds, FVector startPos, FVector targetPos, float totalTime,
	float elapsedTime, float startVelMax, float damping)
{
	if (firstFrame)
	{
		this->SetActorLocation(startPos);
	}
	FVector currentPos = this->GetActorLocation();
	//Create the vectors that will make the x and y axis of a new coordinate system, for simplification purposes
	//The x axis will be in the direction of the line from our current position to the targetPos
	FVector xVec = (targetPos - currentPos);
	xVec.Normalize();
	FVector yVec = FVector::CrossProduct(xVec, FVector(1, 0, 0));
	yVec.Normalize();

	//Get our y velocity in our new coordinate system;
	float yVel = FVector::DotProduct(velocity, yVec);

	//If it's the first frame, set the y velocity to add a "curve" to the trajectory
	if (firstFrame)
	{
		yVel = FMath::FRandRange(-startVelMax, startVelMax);
	}

	//Set our x velocity based on the distance to the target and the remaining time
	float xVel = (targetPos - currentPos).Size() / (totalTime - elapsedTime + VERY_SMALL_VALUE);

	//Dampen y velocity
	yVel /= FMath::Pow(1+damping, DeltaSeconds);

	//convert our new coordinate system velocity back to the world coordinate system, and store it in velocity
	velocity = xVec * xVel + yVec * yVel;
	//apply velocity
	this->SetActorLocation(currentPos + velocity * DeltaSeconds);
}
