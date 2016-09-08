// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KOA_BASE_Monster.h"
#include "MON_Ratbird.generated.h"

/**
 * 
 */
UCLASS()
class KOA_PROTO_API AMON_Ratbird : public AKOA_BASE_Monster
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	AMON_Ratbird(const FObjectInitializer& ObjectInitializer);

private:
	/** AMON_Ratbird movement component */
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPawnMovementComponent* MovementComponent;
	
	
};
