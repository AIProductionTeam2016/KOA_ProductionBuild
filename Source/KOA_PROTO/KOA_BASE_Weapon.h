// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */





class KOA_PROTO_API KOA_BASE_Weapon {
public:
	FString Name;

public:
	KOA_BASE_Weapon();
	virtual ~KOA_BASE_Weapon();

	//void UseSpell(EAbilityButton Button);

private:
	virtual void SpellQ() = 0;
	virtual void SpellW() = 0;
	virtual void SpellE() = 0;
	virtual void SpellR() = 0;
};
