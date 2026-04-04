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
	return true;
}

bool UTtWeaponComponent::CanEquipAmmo(const UTtAmmoData* AmmoData) const
{
	return true;
}

void UTtWeaponComponent::EquipWeaponBySlot(const ETtWeaponSlot InWeaponSlot)
{
	UTtWeaponData** InWeaponData = ActiveWeaponDataByWeaponSlots.Find(InWeaponSlot);
	EquipWeaponByData(*InWeaponData);
}

void UTtWeaponComponent::EquipWeaponByData(const UTtWeaponData* WeaponData)
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

void UTtWeaponComponent::EquipAmmoBySlot(const ETtAmmoSlot InAmmoSlot)
{
	UTtAmmoData** InAmmoData = AmmoDataBySlot.Find(InAmmoSlot);
	EquipAmmoByData(*InAmmoData);
}

void UTtWeaponComponent::EquipAmmoByData(const UTtAmmoData* AmmoData)
{
	AActor* OwnerActor = GetOwner();

	FGameplayEventData EventData;
	EventData.EventTag = FTtGameplayTags::Get().Event_Ammo_Equip;
	EventData.Instigator = OwnerActor;
	EventData.OptionalObject = AmmoData;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		OwnerActor,
		EventData.EventTag,
		EventData
	);
}

void UTtWeaponComponent::Fire(const UTtAmmoData* AmmoData)
{
	AActor* OwnerActor = GetOwner();

	FGameplayEventData EventData;
	EventData.EventTag = FTtGameplayTags::Get().Event_Weapon_Fire;
	EventData.Instigator = OwnerActor;
	EventData.OptionalObject = AmmoData;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		OwnerActor,
		EventData.EventTag,
		EventData
	);
}

void UTtWeaponComponent::Reload(const ETtWeaponSlot InWeaponSlot)
{
	AActor* OwnerActor = GetOwner();

	FGameplayEventData EventData;
	EventData.EventTag = FTtGameplayTags::Get().Event_Weapon_Reload;
	EventData.Instigator = OwnerActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		OwnerActor,
		EventData.EventTag,
		EventData
	);
}

void UTtWeaponComponent::ReloadAll()
{
	AActor* OwnerActor = GetOwner();
	
	for (const TPair<ETtWeaponSlot, UTtWeaponData*>& Pair : ActiveWeaponDataByWeaponSlots)
	{
		UTtWeaponData* WeaponData = Pair.Value;
		if (!WeaponData)
		{
			continue;
		}

		FGameplayEventData EventData;
		EventData.EventTag = FTtGameplayTags::Get().Event_Weapon_Reload;
		EventData.Instigator = OwnerActor;
		EventData.OptionalObject = WeaponData;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			OwnerActor,
			EventData.EventTag,
			EventData
		);
	}
}

void UTtWeaponComponent::DecreaseAmmoInSlot(ETtWeaponSlot InWeaponSlot, int32 InValue)
{	
	if (int32* AmmoPtr = AmmoByWeaponSlots.Find(InWeaponSlot))
	{
		int32 OldValue = *AmmoPtr;
		*AmmoPtr -= InValue;
		
		if (OldValue != *AmmoPtr)
		{
			OnAmmoByWeaponSlotsChanged.Broadcast();
		}
	}
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

void UTtWeaponComponent::SetAmmoByWeaponSlots(const TMap<ETtWeaponSlot, int32>& NewAmmoByWeaponSlots)
{
	AmmoByWeaponSlots = NewAmmoByWeaponSlots;
	OnAmmoByWeaponSlotsChanged.Broadcast();
}

void UTtWeaponComponent::SetAmmoDataBySlot(const TMap<ETtAmmoSlot, UTtAmmoData*>& NewAmmoDataBySlot)
{
	AmmoDataBySlot = NewAmmoDataBySlot;
	OnAmmoDataBySlotChanged.Broadcast();
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


