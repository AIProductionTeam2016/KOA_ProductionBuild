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
	// Init variables to default value //
	// STATS //
	Name = "INVALID_ENEMY_NAME";
	EnemyID = ETypeOfEnemy::NONE;
	
	// -- HP -- //
	HPCurr = 75.0f;
	HPMax = 100.0f;
	// -- STATUS -- //
	IsDead = false;
	// -- MOVEMENT -- //
	Speed = 0.0f;

}

// Called when the game starts or when spawned
void AKOA_BASE_EnemyCharacter::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AKOA_BASE_EnemyCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// If HP ever drops below 0.0, you die 
	if (HPCurr <= 0.0 && !IsDead) {
		OnDeath();
	}


	// Basic intelligence: Move the enemy towards the player
	//AActor* actor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	//if (!actor) return;
	//
	//FVector toPlayer = actor->GetActorLocation() - GetActorLocation();
	//toPlayer.Normalize();
	//
	//// Move the enemy towards the player
	//AddMovementInput(toPlayer, Speed * DeltaTime);

	// Have the enemy look at the player
	//FRotator toPlayerRotation = toPlayer.Rotation();
	//toPlayerRotation.Pitch = 0;
	//RootComponent->SetWorldRotation(toPlayerRotation);
}

// Called to bind functionality to input
void AKOA_BASE_EnemyCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AKOA_BASE_EnemyCharacter::ReceiveDamage(float Amount) {
	HPCurr -= Amount;
}

void AKOA_BASE_EnemyCharacter::OnDeath() {
	IsDead = true;
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