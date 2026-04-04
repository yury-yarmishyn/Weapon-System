// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "TestTask.h"
#include "Core/TtGameplayTags.h"
#include "Weapons/Data/TtWeaponData.h"

UTtWeaponComponent::UTtWeaponComponent()
{
	
}

bool UTtWeaponComponent::CanEquipWeapon(const UTtWeaponData* WeaponData) const
{
	// Placeholder for future save system or other non gameplay validation
	
	if (!WeaponData)
	{
		return false;
	}

	if (const AActor* OwnerActor = GetOwner(); !OwnerActor)
	{
		return false;
	}
	
	return true;
}

bool UTtWeaponComponent::CanEquipAmmo(const UTtAmmoData* AmmoData) const
{
	// Placeholder
	const AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return false;
	}
	
	return true;
}

void UTtWeaponComponent::EquipWeapon(const UTtWeaponData* WeaponData)
{
	AActor* OwnerActor = GetOwner();

	FGameplayEventData EventData;
	EventData.EventTag = FTtGameplayTags::Get().Event_Weapon_Equip;
	EventData.Instigator = OwnerActor;
	EventData.OptionalObject = WeaponData;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		OwnerActor,
		EventData.EventTag,
		EventData
	);
}

void UTtWeaponComponent::EquipAmmo(const UTtAmmoData* AmmoData)
{
	
}

void UTtWeaponComponent::Shoot(const UTtAmmoData* AmmoData)
{
}

void UTtWeaponComponent::Reload(const ETtWeaponSlot InWeaponSlot)
{
}

void UTtWeaponComponent::ReloadAll()
{
}

void UTtWeaponComponent::DecreaseAmmoInSlot(ETtWeaponSlot InWeaponSlot, int32 InValue)
{
}

void UTtWeaponComponent::SetCurrentAmmoSlot(ETtAmmoSlot NewAmmoSlot)
{
	const ETtAmmoSlot OldAmmoSlot = GetCurrentAmmoSlot();
	CurrentAmmoSlot = NewAmmoSlot;
	OnAmmoSlotChanged.Broadcast(OldAmmoSlot);
}

void UTtWeaponComponent::SetCurrentWeaponSlot(ETtWeaponSlot NewWeaponSlot)
{
	const ETtWeaponSlot OldWeaponSlot = GetCurrentWeaponSlot();
	CurrentWeaponSlot = NewWeaponSlot;
	OnWeaponSlotChanged.Broadcast(OldWeaponSlot);
}

void UTtWeaponComponent::SetCurrentAmmoByWeaponSlots(const TMap<ETtWeaponSlot, int32>& NewCurrentAmmoByWeaponSlots)
{
	CurrentAmmoByWeaponSlots = NewCurrentAmmoByWeaponSlots;
	OnCurrentAmmoByWeaponSlotsChanged.Broadcast();
}

void UTtWeaponComponent::SetActiveAmmoSlotsByWeaponSlots(
	const TMap<ETtWeaponSlot, ETtAmmoSlot>& NewActiveAmmoSlotsByWeaponSlots)
{
	ActiveAmmoSlotsByWeaponSlots = NewActiveAmmoSlotsByWeaponSlots;
	OnActiveWeaponDataByWeaponSlotsChanged.Broadcast();
}

void UTtWeaponComponent::SetActiveWeaponDataByWeaponSlots(
	const TMap<ETtWeaponSlot, UTtWeaponData*>& NewActiveWeaponDataByWeaponSlots)
{
	ActiveWeaponDataByWeaponSlots = NewActiveWeaponDataByWeaponSlots;
	OnActiveWeaponDataByWeaponSlotsChanged.Broadcast();
}


