// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KOA_BASE_Artifact.h"
#include "KOA_Artifact_DualDaggers.generated.h"

/**
 * 
 */
UCLASS()
class KOA_PROTO_API UKOA_Artifact_DualDaggers : public UKOA_BASE_Artifact
{
	GENERATED_BODY()
public:
	UKOA_Artifact_DualDaggers();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability,Q")
	void PressAbilityQ();
	virtual void PressAbilityQ_Implementation();
};
