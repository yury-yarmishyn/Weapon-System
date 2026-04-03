// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Centralized native gameplay tags used by the TestTaskSolution runtime.
 */
struct TESTTASK_API FTtGameplayTags
{
public:
	static const FTtGameplayTags& Get();
	static void InitializeNativeGameplayTags();

	FGameplayTag Event_Weapon_Fire;
	FGameplayTag Event_Weapon_Reload;
	FGameplayTag Event_Weapon_Equip;

	FGameplayTag Ammo_Regular;
	FGameplayTag Ammo_Fire;
	FGameplayTag Ammo_Water;
	FGameplayTag Ammo_Healing;

	FGameplayTag Status_Burning;

private:
	static FTtGameplayTags GameplayTags;
};
