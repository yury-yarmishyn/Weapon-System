// Fill out your copyright notice in the Description page of Project Settings.

#include "TestTaskSolution/Combat/GameplayAbilities/TtEquipAmmoAbility.h"

#include "Core/TtGameplayTags.h"
#include "TestTask.h"
#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"
#include "TestTaskSolution/Weapons/Data/TtAmmoData.h"

UTtEquipAmmoAbility::UTtEquipAmmoAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	const FGameplayTag TriggerTag = TtGameplayTags::TAG_Event_Ammo_Equip;

	if (TriggerTag.IsValid())
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		TriggerData.TriggerTag = TriggerTag;
		AbilityTriggers.Add(TriggerData);
	}
}

void UTtEquipAmmoAbility::ActivateAbility(
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

	ETtAmmoSlot NewAmmoSlot = WeaponComponent->GetCurrentAmmoSlot();
	if (TriggerEventData)
	{
		const UTtAmmoData* AmmoData = Cast<UTtAmmoData>(TriggerEventData->OptionalObject);
		if (AmmoData)
		{
			NewAmmoSlot = FindAmmoSlotByData(WeaponComponent, AmmoData, NewAmmoSlot);
		}
	}

	PendingWeaponComponent = WeaponComponent;
	PendingWeaponSlot = WeaponComponent->GetCurrentWeaponSlot();
	PendingAmmoSlot = NewAmmoSlot;

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UTtEquipAmmoAbility::CommitAbility(
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

	WeaponComponent->SetCurrentAmmoSlot(PendingAmmoSlot);
	WeaponComponent->SetAmmoSlotForWeapon(PendingWeaponSlot, PendingAmmoSlot);

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Ammo equipped. WeaponSlot=%d AmmoSlot=%d"),
		*GetNameSafe(WeaponComponent->GetOwner()),
		static_cast<int32>(PendingWeaponSlot),
		static_cast<int32>(PendingAmmoSlot));

	return true;
}
