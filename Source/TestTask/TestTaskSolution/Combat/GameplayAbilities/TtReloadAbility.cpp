// Fill out your copyright notice in the Description page of Project Settings.

#include "TestTaskSolution/Combat/GameplayAbilities/TtReloadAbility.h"

#include "TestTask.h"
#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"
#include "TestTaskSolution/Weapons/Data/TtWeaponData.h"

UTtReloadAbility::UTtReloadAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
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

	TMap<ETtWeaponSlot, int32> WeaponSlotByAmmo = WeaponComponent->GetWeaponSlotByAmmo();
	WeaponSlotByAmmo.FindOrAdd(SlotToReload) = FMath::Max(AmmoAfterReload, 0);

	UE_LOG(LogTestTask, Log, TEXT("Reload animation placeholder"));
	WeaponComponent->SetWeaponSlotByAmmo(WeaponSlotByAmmo);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
