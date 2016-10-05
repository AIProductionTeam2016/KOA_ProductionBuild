// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "BASE_Projectile.h"

static const float SMALL_VALUE = 0.05f; //used to prefent the projectile from moving to crazily at the end of its lifetime
static const float BIG_VALUE = 100000; //multiplied in when determining acceleration
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
			0, 1.5, 0.5, 1.0, 20.0f, squigglyArcHeight);
	}
}

// Called every frame
void ABASE_Projectile::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
	existedTime += DeltaTime;
	if (ProjTrajectory == EProjectileTrajectory::PT_SQUIGGLY)
	{
		DoSquiggleMovement(false, DeltaTime, startLocation, TargetLocation, ProjLifeTime,
			existedTime, 1.5, 0.5, 1.0, 20.0f, squigglyArcHeight);
	}
	if (existedTime > ProjLifeTime)
	{
		this->Destroy();
	}
}

void ABASE_Projectile::DoSquiggleMovement(bool firstFrame, float DeltaSeconds, FVector startPos, FVector targetPos, float totalTime,
	float elapsedTime, float forceMultMax, float forceMultMin, float frequency, float startVel, float &forceMult)
{
	float totalLength = (startPos - targetPos).Size();
	FVector previousPos = this->GetActorLocation();
	//Create the vectors that will make the x and y axis of a new coordinate system, for simplification purposes
	FVector xVec = (targetPos - startPos);
	xVec.Normalize();
	FVector yVec = FVector::CrossProduct(xVec, FVector(1, 0, 0));
	yVec.Normalize();

	//Get our y velocity in our new coordinate system;
	float yVel = FVector::DotProduct(velocity, yVec);
	//If it's the first frame, set our y velocity based on the distance to the target
	if (firstFrame)
	{
		yVel = startVel * totalLength / totalTime;
	}

	//Get the distance from ourselves to the startpos to targetpos line
	float yPos = FVector::DotProduct(previousPos - startPos, yVec);

	//Get the previousPos projected onto the line from the startPos to the targetPos
	FVector projectedPos = previousPos - (yVec * yPos);

	//Set our x velocity based on the distance to the target and the remaining time
	float xVel = (targetPos - projectedPos).Size() / (totalTime - elapsedTime);

	//Find the best acceleration
	float targetAccel = yPos * BIG_VALUE * frequency * forceMult / (totalLength) / FMath::Pow(totalTime - elapsedTime + SMALL_VALUE, 2) / totalTime;

	//Add the acceleration
	yVel -= targetAccel * DeltaSeconds;

	//convert our new coordinate system velocity back to the world coordinate system, and store it in velocity
	velocity = xVec * xVel + yVec * yVel;
	//apply velocity
	this->SetActorLocation(previousPos + velocity * DeltaSeconds);

	float newYPos = FVector::DotProduct(this->GetActorLocation() - startPos, yVec);

	if (FMath::Sign(yPos) != FMath::Sign(newYPos))
	{
		forceMult = FMath::FRandRange(forceMultMin, forceMultMax);
	}
}
