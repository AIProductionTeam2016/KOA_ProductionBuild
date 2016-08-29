// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_BASE_EnemyCharacter.h"
#include "KOA_BASE_EnemyMovement.h"


// Sets default values
AKOA_BASE_EnemyCharacter::AKOA_BASE_EnemyCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UKOA_BASE_EnemyMovement>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Init variables to default value
	Speed = 1.0f;
	HitPoints = 3.0f;
	//SightRange = 1200.0f;
	//AttackRange = 175.0f;

	//// Create the monster's SightSphere and attach it to the root
	//SightSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SightSphere"));
	//SightSphere->SetSphereRadius(SightRange);
	//SightSphere->AttachTo(RootComponent);
	//
	//// Create the monster's AttackRangeSphere
	//AttackRangeSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("AttackRangeSphere"));
	//AttackRangeSphere->SetSphereRadius(AttackRange);
	//AttackRangeSphere->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AKOA_BASE_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKOA_BASE_EnemyCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// Basic intelligence: Move the enemy towards the player
	AActor* actor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!actor) return;

	FVector toPlayer = actor->GetActorLocation() - GetActorLocation();
	toPlayer.Normalize();

	// Move the enemy towards the player
	AddMovementInput(toPlayer, Speed * DeltaTime);

	// Have the enemy look at the player
	FRotator toPlayerRotation = toPlayer.Rotation();
	toPlayerRotation.Pitch = 0;
	RootComponent->SetWorldRotation(toPlayerRotation);
}

// Called to bind functionality to input
void AKOA_BASE_EnemyCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

//#if WITH_EDITOR
//void AKOA_BASE_EnemyCharacter::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) {
//	FName propertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
//	if (propertyName == GET_MEMBER_NAME_CHECKED(AKOA_BASE_EnemyCharacter, SightRange)) {
//		SightSphere->SetSphereRadius(SightRange);
//	}
//	else if (propertyName == GET_MEMBER_NAME_CHECKED(AKOA_BASE_EnemyCharacter, AttackRange)) {
//		AttackRangeSphere->SetSphereRadius(AttackRange);
//	}
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//}
//#endif