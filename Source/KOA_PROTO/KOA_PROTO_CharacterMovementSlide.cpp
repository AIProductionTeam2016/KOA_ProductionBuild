// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_PROTO_CharacterMovementSlide.h"
#include "KOA_PROTO_Character.h"

void UKOA_PROTO_CharacterMovementSlide::InitializeComponent()
{
	Super::InitializeComponent();
	//~~~~~~~~~~~~~~~~~

	//UE_LOG //comp Init!
}

//Tick Comp
void UKOA_PROTO_CharacterMovementSlide::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->MovementMode.GetValue() == MOVE_Custom && this->CustomMovementMode == (uint8)ECustomMovementType::CMT_WallSlide)
	{
		FVector playerLocation = GetActorLocation();
		AKOA_PROTO_Character* characterOwner = Cast<AKOA_PROTO_Character>(this->GetCharacterOwner());
		characterOwner->JumpStats.ApplyWallSlideAcceleration(DeltaTime);
		characterOwner->JumpStats.DisplayWallSlideDebugInfo();
		// playerLocation - (distance * -UpVector * DeltaTime)
		FVector vMovement = -(characterOwner->JumpStats.GetCurrSlideVelocity() * characterOwner->GetActorUpVector());
		FHitResult hitResult;
		SafeMoveUpdatedComponent(vMovement * DeltaTime, characterOwner->GetActorRotation(), true, hitResult);
	}
}

