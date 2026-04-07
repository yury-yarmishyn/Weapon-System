// Fill out your copyright notice in the Description page of Project Settings.

#include "TestTaskSolution/Combat/GameplayAbilities/TtReloadAbility.h"

#include "TestTask.h"
#include "Core/TtGameplayTags.h"
#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"
#include "TestTaskSolution/Weapons/Data/TtWeaponData.h"

UTtReloadAbility::UTtReloadAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	const FGameplayTag TriggerTag = TtGameplayTags::TAG_Event_Weapon_Reload;

	if (TriggerTag.IsValid())
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		TriggerData.TriggerTag = TriggerTag;
		AbilityTriggers.Add(TriggerData);
	}
}

void UTtReloadAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	UTtWeaponComponent* WeaponComponent = GetWeaponComponentFromActorInfo(ActorInfo);
	if (!WeaponComponent)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ETtWeaponSlot SlotToReload = WeaponComponent->GetCurrentWeaponSlot();
	const UTtWeaponData* WeaponDataFromEvent = nullptr;

	if (TriggerEventData)
	{
		WeaponDataFromEvent = Cast<UTtWeaponData>(TriggerEventData->OptionalObject);
		if (WeaponDataFromEvent)
		{
			SlotToReload = FindWeaponSlotByData(WeaponComponent, WeaponDataFromEvent, SlotToReload);
		}
	}

	int32 AmmoAfterReload = INDEX_NONE;
	if (TriggerEventData && TriggerEventData->EventMagnitude > 0.f)
	{
		AmmoAfterReload = FMath::RoundToInt(TriggerEventData->EventMagnitude);
	}

	if (AmmoAfterReload == INDEX_NONE && WeaponDataFromEvent)
	{
		AmmoAfterReload = WeaponDataFromEvent->MagazineSize;
	}

	if (AmmoAfterReload == INDEX_NONE)
	{
		if (UTtWeaponData* const* WeaponData = WeaponComponent->GetWeaponSlotByWeaponData().Find(SlotToReload))
		{
			AmmoAfterReload = *WeaponData ? (*WeaponData)->MagazineSize : 0;
		}
		else
		{
			AmmoAfterReload = 0;
		}
	}

	PendingWeaponComponent = WeaponComponent;
	PendingWeaponSlot = SlotToReload;
	PendingAmmoAfterReload = FMath::Max(AmmoAfterReload, 0);

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Reload started. WeaponSlot=%d TargetAmmo=%d"),
		*GetNameSafe(WeaponComponent->GetOwner()),
		static_cast<int32>(PendingWeaponSlot),
		PendingAmmoAfterReload);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UTtReloadAbility::CommitAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	FGameplayTagContainer* OptionalRelevantTags)
{
	UTtWeaponComponent* WeaponComponent = PendingWeaponComponent.Get();
	if (!WeaponComponent)
	{
		return false;
	}

	if (!Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
	{
		return false;
	}

	TMap<ETtWeaponSlot, int32> WeaponSlotByAmmo = WeaponComponent->GetWeaponSlotByAmmo();
	WeaponSlotByAmmo.FindOrAdd(PendingWeaponSlot) = PendingAmmoAfterReload;
	WeaponComponent->SetWeaponSlotByAmmo(WeaponSlotByAmmo);

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Reload completed. WeaponSlot=%d Ammo=%d"),
		*GetNameSafe(WeaponComponent->GetOwner()),
		static_cast<int32>(PendingWeaponSlot),
		PendingAmmoAfterReload);

	return true;
}
