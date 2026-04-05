// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "TestTask.h"
#include "Core/TtGameplayTags.h"
#include "TestTaskSolution/Weapons/Interfaces/TtWeaponInterface.h"
#include "Weapons/Data/TtWeaponData.h"

UTtWeaponComponent::UTtWeaponComponent()
{
	
}

void UTtWeaponComponent::InitWeaponComponent(
	const TMap<ETtWeaponSlot, UTtWeaponData*>& DefaultWeaponSlotsByData,
	const TMap<ETtAmmoSlot, UTtAmmoData*>& DefaultAmmoSlotsByData)
{
	WeaponSlotByWeaponData = DefaultWeaponSlotsByData;
	AmmoSlotByAmmoData = DefaultAmmoSlotsByData;

	WeaponSlotByAmmo.Empty();
	WeaponSlotByAmmoSlot.Empty();

	ETtWeaponSlot FirstWeaponSlot = ETtWeaponSlot::Slot1;
	bool bHasWeaponSlot = false;
	for (const TPair<ETtWeaponSlot, UTtWeaponData*>& WeaponPair : WeaponSlotByWeaponData)
	{
		FirstWeaponSlot = WeaponPair.Key;
		bHasWeaponSlot = true;
		break;
	}

	ETtAmmoSlot FirstAmmoSlot = ETtAmmoSlot::Slot1;
	bool bHasAmmoSlot = false;
	for (const TPair<ETtAmmoSlot, UTtAmmoData*>& AmmoPair : AmmoSlotByAmmoData)
	{
		FirstAmmoSlot = AmmoPair.Key;
		bHasAmmoSlot = true;
		break;
	}

	// Can be extended with explicit per-weapon ammo-slot mapping data.
	for (const TPair<ETtWeaponSlot, UTtWeaponData*>& WeaponPair : WeaponSlotByWeaponData)
	{
		const int32 InitialMagazineAmmo = WeaponPair.Value ? WeaponPair.Value->MagazineSize : 0;
		WeaponSlotByAmmo.Add(WeaponPair.Key, InitialMagazineAmmo);
		WeaponSlotByAmmoSlot.Add(WeaponPair.Key, bHasAmmoSlot ? FirstAmmoSlot : ETtAmmoSlot::Slot1);
	}

	CurrentWeaponSlot = bHasWeaponSlot ? FirstWeaponSlot : ETtWeaponSlot::Slot1;
	CurrentAmmoSlot = bHasAmmoSlot ? FirstAmmoSlot : ETtAmmoSlot::Slot1;

	WeaponHandler = nullptr;
	if (AActor* OwnerActor = GetOwner())
	{
		if (ITtWeaponInterface* WeaponInterface = Cast<ITtWeaponInterface>(OwnerActor))
		{
			WeaponHandler = WeaponInterface->GetWeaponHandler();
		}
	}

	OnWeaponSlotByWeaponDataChanged.Broadcast();
	OnAmmoSlotByAmmoDataChanged.Broadcast();
	OnWeaponSlotByAmmoChanged.Broadcast();
	OnWeaponSlotByAmmoSlotChanged.Broadcast();
}

void UTtWeaponComponent::GrantAbilitiesFromData()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor || !OwnerActor->HasAuthority())
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	if (!AbilitySystemComponent)
	{
		return;
	}

	TSet<TSubclassOf<UGameplayAbility>> GrantedAbilityClasses;

	const auto GiveAbilityIfValid = [&](const TSubclassOf<UGameplayAbility>& AbilityClass)
	{
		if (!AbilityClass || GrantedAbilityClasses.Contains(AbilityClass))
		{
			return;
		}

		GrantedAbilityClasses.Add(AbilityClass);
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
	};

	for (const TPair<ETtWeaponSlot, UTtWeaponData*>& WeaponPair : WeaponSlotByWeaponData)
	{
		const UTtWeaponData* WeaponData = WeaponPair.Value;
		if (!WeaponData)
		{
			continue;
		}

		GiveAbilityIfValid(WeaponData->FireAbilityClass);
		GiveAbilityIfValid(WeaponData->ReloadAbilityClass);
		GiveAbilityIfValid(WeaponData->EquipAbilityClass);

		// Projectile data ability grant can be added here once dedicated fields are present.
	}

	for (const TPair<ETtAmmoSlot, UTtAmmoData*>& AmmoPair : AmmoSlotByAmmoData)
	{
		const UTtAmmoData* AmmoData = AmmoPair.Value;
		if (!AmmoData)
		{
			continue;
		}

		GiveAbilityIfValid(AmmoData->EquipAmmoAbilityClass);

		// Ammo projectile override ability grant can be added here once dedicated fields are present.
	}
}

bool UTtWeaponComponent::CanEquipWeapon(const UTtWeaponData* WeaponData) const
{
	// Placeholder
	return true;
}

bool UTtWeaponComponent::CanEquipAmmo(const UTtAmmoData* AmmoData) const
{
	// Placeholder
	return true;
}

void UTtWeaponComponent::EquipWeaponBySlot(const ETtWeaponSlot InWeaponSlot)
{
	UTtWeaponData** InWeaponData = WeaponSlotByWeaponData.Find(InWeaponSlot);
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
	UTtAmmoData** InAmmoData = AmmoSlotByAmmoData.Find(InAmmoSlot);
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
	
	for (const TPair<ETtWeaponSlot, UTtWeaponData*>& Pair : WeaponSlotByWeaponData)
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
	if (int32* AmmoPtr = WeaponSlotByAmmo.Find(InWeaponSlot))
	{
		int32 OldValue = *AmmoPtr;
		*AmmoPtr -= InValue;
		
		if (OldValue != *AmmoPtr)
		{
			OnWeaponSlotByAmmoChanged.Broadcast();
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

void UTtWeaponComponent::SetWeaponSlotByAmmo(const TMap<ETtWeaponSlot, int32>& NewWeaponSlotByAmmo)
{
	WeaponSlotByAmmo = NewWeaponSlotByAmmo;
	OnWeaponSlotByAmmoChanged.Broadcast();
}

void UTtWeaponComponent::SetAmmoSlotByAmmoData(const TMap<ETtAmmoSlot, UTtAmmoData*>& NewAmmoSlotByAmmoData)
{
	AmmoSlotByAmmoData = NewAmmoSlotByAmmoData;
	OnAmmoSlotByAmmoDataChanged.Broadcast();
}

void UTtWeaponComponent::SetWeaponSlotByAmmoSlot(
	const TMap<ETtWeaponSlot, ETtAmmoSlot>& NewWeaponSlotByAmmoSlot)
{
	WeaponSlotByAmmoSlot = NewWeaponSlotByAmmoSlot;
	OnWeaponSlotByAmmoSlotChanged.Broadcast();
}

void UTtWeaponComponent::SetWeaponSlotByWeaponData(
	const TMap<ETtWeaponSlot, UTtWeaponData*>& NewWeaponSlotByWeaponData)
{
	WeaponSlotByWeaponData = NewWeaponSlotByWeaponData;
	OnWeaponSlotByWeaponDataChanged.Broadcast();
}
