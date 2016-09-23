// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ATMA_StatusEffects.h"
#include "AMTA_BASE_Throwable.generated.h"

UENUM(Blueprinttype)
enum class ETYPEOF_Throwable : uint8 {
	KNIFE = 0,
	BOMB,
	NUM,
	NONE = 99
};

/********************************************************
 *				UAMTA_BASE_Throwable					*
 ********************************************************/
UCLASS(Blueprintable)
class KOA_PROTO_API UAMTA_BASE_Throwable : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	ETYPEOF_Throwable TypeOf;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	UTexture* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FStatusEffects StatusEffects;
public:
	UAMTA_BASE_Throwable();
	~UAMTA_BASE_Throwable();

};
