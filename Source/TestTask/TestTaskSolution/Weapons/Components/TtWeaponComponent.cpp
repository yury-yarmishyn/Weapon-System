// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"
#include "TestTask.h"

UTtWeaponComponent::UTtWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTtWeaponComponent::SelectAmmoSlot(ETtAmmoSlot NewSlot)
{
	if (CurrentAmmoSlot == NewSlot)
	{
		return;
	}

	CurrentAmmoSlot = NewSlot;

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("Ammo slot changed to %d for %s"),
		static_cast<int32>(CurrentAmmoSlot) + 1,
		*GetNameSafe(GetOwner()));
}

