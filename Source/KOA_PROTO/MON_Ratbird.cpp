// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "MON_Ratbird.h"

AMON_Ratbird::AMON_Ratbird(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CollisionCapsule->AddRelativeRotation(FRotator::MakeFromEuler(FVector(90, 0, 0)));
	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("Movement Component"));
	MovementComponent->UpdatedComponent = CollisionCapsule;
}
