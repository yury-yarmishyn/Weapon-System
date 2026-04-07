// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/World.h"
#include "TestTask.h"
#include "Core/TtGameplayTags.h"
#include "Weapons/Data/TtProjectileData.h"
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
		if (OwnerActor->GetClass()->ImplementsInterface(UTtWeaponInterface::StaticClass()))
		{
			WeaponHandler = ITtWeaponInterface::Execute_GetWeaponHandler(OwnerActor);
		}
	}

	OnWeaponSlotByWeaponDataChanged.Broadcast();
	OnAmmoSlotByAmmoDataChanged.Broadcast();
	OnWeaponSlotByAmmoChanged.Broadcast();
	OnWeaponSlotByAmmoSlotChanged.Broadcast();

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] WeaponComponent initialized. WeaponSlots=%d AmmoSlots=%d CurrentWeapon=%d CurrentAmmo=%d"),
		*GetNameSafe(GetOwner()),
		WeaponSlotByWeaponData.Num(),
		AmmoSlotByAmmoData.Num(),
		static_cast<int32>(CurrentWeaponSlot),
		static_cast<int32>(CurrentAmmoSlot));

	if (!bHasWeaponSlot)
	{
		UE_LOG(LogTestTask, Warning, TEXT("[%s] WeaponComponent has no weapon init data"), *GetNameSafe(GetOwner()));
	}

	if (!bHasAmmoSlot)
	{
		UE_LOG(LogTestTask, Warning, TEXT("[%s] WeaponComponent has no ammo init data"), *GetNameSafe(GetOwner()));
	}
}

void UTtWeaponComponent::GrantAbilitiesFromInitData()
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
	int32 GrantedAbilityCount = 0;

	const auto IsAbilityAlreadyGranted = [&](const TSubclassOf<UGameplayAbility>& AbilityClass) -> bool
	{
		if (!AbilityClass)
		{
			return false;
		}

		for (const FGameplayAbilitySpec& ExistingSpec : AbilitySystemComponent->GetActivatableAbilities())
		{
			if (ExistingSpec.Ability && ExistingSpec.Ability->GetClass() == AbilityClass)
			{
				return true;
			}
		}

		return false;
	};

	const auto GiveAbilityIfValid = [&](const TSubclassOf<UGameplayAbility>& AbilityClass)
	{
		if (!AbilityClass || GrantedAbilityClasses.Contains(AbilityClass) || IsAbilityAlreadyGranted(AbilityClass))
		{
			return;
		}

		GrantedAbilityClasses.Add(AbilityClass);
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
		++GrantedAbilityCount;

		UE_LOG(
			LogTestTask,
			Log,
			TEXT("[%s] Granted ability: %s"),
			*GetNameSafe(OwnerActor),
			*GetNameSafe(AbilityClass));
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

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] GrantAbilitiesFromInitData finished. NewAbilities=%d"),
		*GetNameSafe(OwnerActor),
		GrantedAbilityCount);
}

bool UTtWeaponComponent::CanEquipWeapon(const ETtWeaponSlot WeaponSlot) const
{
	const UTtWeaponData* const* WeaponDataPtr = WeaponSlotByWeaponData.Find(WeaponSlot);
	if (!WeaponDataPtr || !*WeaponDataPtr)
	{
		return false;
	}

	return WeaponSlotByAmmo.Contains(WeaponSlot);
}

bool UTtWeaponComponent::CanEquipAmmo(const ETtAmmoSlot AmmoSlot) const
{
	if (!CanEquipWeapon(CurrentWeaponSlot))
	{
		return false;
	}

	const UTtAmmoData* const* AmmoDataPtr = AmmoSlotByAmmoData.Find(AmmoSlot);
	return AmmoDataPtr && *AmmoDataPtr;
}

bool UTtWeaponComponent::CanFire(const ETtAmmoSlot AmmoSlot) const
{
	if (!GetWorld())
	{
		return false;
	}

	if (!CanEquipWeapon(CurrentWeaponSlot) || !CanEquipAmmo(AmmoSlot))
	{
		return false;
	}

	const int32* CurrentAmmoPtr = WeaponSlotByAmmo.Find(CurrentWeaponSlot);
	if (!CurrentAmmoPtr || *CurrentAmmoPtr <= 0)
	{
		return false;
	}

	const UTtAmmoData* const* AmmoDataPtr = AmmoSlotByAmmoData.Find(AmmoSlot);
	const UTtAmmoData* AmmoData = AmmoDataPtr ? *AmmoDataPtr : nullptr;
	if (!AmmoData || !AmmoData->ProjectileData || !AmmoData->ProjectileData->ProjectileClass)
	{
		return false;
	}

	return true;
}

ETtWeaponSlot UTtWeaponComponent::GetNextWeapon(const ETtWeaponSlot InCurrentWeaponSlot) const
{
	TArray<ETtWeaponSlot> ValidWeaponSlots;
	for (const TPair<ETtWeaponSlot, UTtWeaponData*>& Pair : WeaponSlotByWeaponData)
	{
		if (CanEquipWeapon(Pair.Key))
		{
			ValidWeaponSlots.Add(Pair.Key);
		}
	}

	if (ValidWeaponSlots.IsEmpty())
	{
		return InCurrentWeaponSlot;
	}

	const int32 CurrentIndex = ValidWeaponSlots.IndexOfByKey(InCurrentWeaponSlot);
	if (CurrentIndex == INDEX_NONE)
	{
		return ValidWeaponSlots[0];
	}

	const int32 NextIndex = (CurrentIndex + 1) % ValidWeaponSlots.Num();
	return ValidWeaponSlots[NextIndex];
}

ETtWeaponSlot UTtWeaponComponent::GetPrevWeapon(const ETtWeaponSlot InCurrentWeaponSlot) const
{
	TArray<ETtWeaponSlot> ValidWeaponSlots;
	for (const TPair<ETtWeaponSlot, UTtWeaponData*>& Pair : WeaponSlotByWeaponData)
	{
		if (CanEquipWeapon(Pair.Key))
		{
			ValidWeaponSlots.Add(Pair.Key);
		}
	}

	if (ValidWeaponSlots.IsEmpty())
	{
		return InCurrentWeaponSlot;
	}

	const int32 CurrentIndex = ValidWeaponSlots.IndexOfByKey(InCurrentWeaponSlot);
	if (CurrentIndex == INDEX_NONE)
	{
		return ValidWeaponSlots.Last();
	}

	const int32 PrevIndex = (CurrentIndex - 1 + ValidWeaponSlots.Num()) % ValidWeaponSlots.Num();
	return ValidWeaponSlots[PrevIndex];
}

void UTtWeaponComponent::EquipWeaponBySlot(const ETtWeaponSlot InWeaponSlot)
{
	if (!CanEquipWeapon(InWeaponSlot))
	{
		UE_LOG(
			LogTestTask,
			Warning,
			TEXT("[%s] EquipWeaponBySlot rejected. RequestedSlot=%d"),
			*GetNameSafe(GetOwner()),
			static_cast<int32>(InWeaponSlot));
		return;
	}

	if (UTtWeaponData* const* InWeaponData = WeaponSlotByWeaponData.Find(InWeaponSlot))
	{
		EquipWeaponByData(*InWeaponData);
	}
}

void UTtWeaponComponent::EquipWeaponByData(const UTtWeaponData* WeaponData)
{
	AActor* OwnerActor = GetOwner();

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Weapon equip requested: %s"),
		*GetNameSafe(OwnerActor),
		*GetNameSafe(WeaponData));

	FGameplayEventData EventData;
	EventData.EventTag = TtGameplayTags::TAG_Event_Weapon_Equip;
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
	if (!CanEquipAmmo(InAmmoSlot))
	{
		UE_LOG(
			LogTestTask,
			Warning,
			TEXT("[%s] EquipAmmoBySlot rejected. RequestedSlot=%d"),
			*GetNameSafe(GetOwner()),
			static_cast<int32>(InAmmoSlot));
		return;
	}

	if (UTtAmmoData* const* InAmmoData = AmmoSlotByAmmoData.Find(InAmmoSlot))
	{
		EquipAmmoByData(*InAmmoData);
	}
}

void UTtWeaponComponent::EquipAmmoByData(const UTtAmmoData* AmmoData)
{
	AActor* OwnerActor = GetOwner();

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Ammo equip requested: %s"),
		*GetNameSafe(OwnerActor),
		*GetNameSafe(AmmoData));

	FGameplayEventData EventData;
	EventData.EventTag = TtGameplayTags::TAG_Event_Ammo_Equip;
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

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Fire requested. WeaponSlot=%d AmmoSlot=%d AmmoData=%s"),
		*GetNameSafe(OwnerActor),
		static_cast<int32>(CurrentWeaponSlot),
		static_cast<int32>(CurrentAmmoSlot),
		*GetNameSafe(AmmoData));

	FGameplayEventData EventData;
	EventData.EventTag = TtGameplayTags::TAG_Event_Weapon_Fire;
	EventData.Instigator = OwnerActor;
	EventData.OptionalObject = AmmoData;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		OwnerActor,
		EventData.EventTag,
		EventData
	);
}

void UTtWeaponComponent::FireBySlot(const ETtAmmoSlot AmmoSlot)
{
	if (!CanFire(AmmoSlot))
	{
		UE_LOG(
			LogTestTask,
			Warning,
			TEXT("[%s] FireBySlot rejected. WeaponSlot=%d AmmoSlot=%d"),
			*GetNameSafe(GetOwner()),
			static_cast<int32>(CurrentWeaponSlot),
			static_cast<int32>(AmmoSlot));
		return;
	}

	const UTtAmmoData* const* AmmoDataPtr = AmmoSlotByAmmoData.Find(AmmoSlot);
	const UTtAmmoData* AmmoData = AmmoDataPtr ? *AmmoDataPtr : nullptr;
	if (!AmmoData)
	{
		return;
	}

	Fire(AmmoData);
}

void UTtWeaponComponent::Reload(const ETtWeaponSlot InWeaponSlot)
{
	AActor* OwnerActor = GetOwner();
	UTtWeaponData* ReloadWeaponData = nullptr;
	if (UTtWeaponData* const* FoundWeaponData = WeaponSlotByWeaponData.Find(InWeaponSlot))
	{
		ReloadWeaponData = *FoundWeaponData;
	}

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Reload requested for weapon slot %d (%s)"),
		*GetNameSafe(OwnerActor),
		static_cast<int32>(InWeaponSlot),
		*GetNameSafe(ReloadWeaponData));

	FGameplayEventData EventData;
	EventData.EventTag = TtGameplayTags::TAG_Event_Weapon_Reload;
	EventData.Instigator = OwnerActor;
	EventData.OptionalObject = ReloadWeaponData;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		OwnerActor,
		EventData.EventTag,
		EventData
	);
}

void UTtWeaponComponent::ReloadAll()
{
	AActor* OwnerActor = GetOwner();
	
	UE_LOG(LogTestTask, Log, TEXT("[%s] ReloadAll requested"), *GetNameSafe(OwnerActor));
	
	for (const TPair<ETtWeaponSlot, UTtWeaponData*>& Pair : WeaponSlotByWeaponData)
	{
		UTtWeaponData* WeaponData = Pair.Value;
		if (!WeaponData)
		{
			continue;
		}

		FGameplayEventData EventData;
		EventData.EventTag = TtGameplayTags::TAG_Event_Weapon_Reload;
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
		const int32 OldValue = *AmmoPtr;
		*AmmoPtr -= InValue;
		
		if (OldValue != *AmmoPtr)
		{
			UE_LOG(
				LogTestTask,
				Log,
				TEXT("[%s] Ammo consumed. WeaponSlot=%d Old=%d New=%d"),
				*GetNameSafe(GetOwner()),
				static_cast<int32>(InWeaponSlot),
				OldValue,
				*AmmoPtr);
			OnWeaponSlotByAmmoChanged.Broadcast();
		}
	}
}

void UTtWeaponComponent::SetCurrentAmmoSlot(ETtAmmoSlot NewAmmoSlot)
{
	const ETtAmmoSlot OldAmmoSlot = GetCurrentAmmoSlot();
	if (OldAmmoSlot == NewAmmoSlot)
	{
		return;
	}
	
	CurrentAmmoSlot = NewAmmoSlot;
	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Ammo slot changed. Old=%d New=%d"),
		*GetNameSafe(GetOwner()),
		static_cast<int32>(OldAmmoSlot),
		static_cast<int32>(NewAmmoSlot));
	OnAmmoSlotChanged.Broadcast(OldAmmoSlot);
}

void UTtWeaponComponent::SetCurrentWeaponSlot(ETtWeaponSlot NewWeaponSlot)
{
	const ETtWeaponSlot OldWeaponSlot = GetCurrentWeaponSlot();
	if (OldWeaponSlot == NewWeaponSlot)
	{
		return;
	}
	
	CurrentWeaponSlot = NewWeaponSlot;
	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Weapon slot changed. Old=%d New=%d"),
		*GetNameSafe(GetOwner()),
		static_cast<int32>(OldWeaponSlot),
		static_cast<int32>(NewWeaponSlot));
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

void UTtWeaponComponent::SetAmmoSlotForWeapon(ETtWeaponSlot WeaponSlot, ETtAmmoSlot NewAmmoSlot)
{
	if (ETtAmmoSlot* ExistingAmmoSlot = WeaponSlotByAmmoSlot.Find(WeaponSlot))
	{
		if (*ExistingAmmoSlot == NewAmmoSlot)
		{
			return;
		}
	}

	WeaponSlotByAmmoSlot.FindOrAdd(WeaponSlot) = NewAmmoSlot;
	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Ammo mapping changed. WeaponSlot=%d -> AmmoSlot=%d"),
		*GetNameSafe(GetOwner()),
		static_cast<int32>(WeaponSlot),
		static_cast<int32>(NewAmmoSlot));
	OnWeaponSlotByAmmoSlotChanged.Broadcast();
}

void UTtWeaponComponent::SetWeaponSlotByWeaponData(
	const TMap<ETtWeaponSlot, UTtWeaponData*>& NewWeaponSlotByWeaponData)
{
	WeaponSlotByWeaponData = NewWeaponSlotByWeaponData;
	OnWeaponSlotByWeaponDataChanged.Broadcast();
}
