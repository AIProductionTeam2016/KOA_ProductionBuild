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
			0, 30, 60, 0.001f, squigglyArcHeight);
	}
}

// Called every frame
void ABASE_Projectile::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
	existedTime += DeltaTime;
	if (ProjTrajectory == EProjectileTrajectory::PT_SQUIGGLY)
	{
		DoSquiggleMovement(false, DeltaTime, startLocation, TargetLocation, ProjLifeTime,
			existedTime, 30, 60, 0.001f, squigglyArcHeight);
	}
	if (existedTime > ProjLifeTime)
	{
		this->Destroy();
	}
}

void ABASE_Projectile::DoSquiggleMovement(bool firstFrame, float DeltaSeconds, FVector startPos, FVector targetPos, float totalTime,
	float elapsedTime, float minAngle, float maxAngle, float startVel, float &arcHeightAlpha)
{
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
		//yVel = startVel * (startPos - targetPos).Size();
	}
	//Set our x velocity based on the distance to the target and the remaining time
	float xVel = (targetPos - previousPos).Size() / (totalTime - elapsedTime);

	//Get our distance from the startpos to targetpos line
	float yPos = FVector::DotProduct(startPos - previousPos, yVec);

	float targetAngle = FMath::Lerp(minAngle, maxAngle, arcHeightAlpha);

	//convert our new coordinate system velocity back to the world coordinate system, and store it in velocity
	velocity = xVec * xVel + yVec * yVel;
	//apply velocity
	this->SetActorLocation(previousPos + velocity * DeltaSeconds);
}
