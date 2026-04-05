// Fill out your copyright notice in the Description page of Project Settings.

#include "TestTaskSolution/Combat/GameplayAbilities/TtEquipWeaponAbility.h"

#include "TestTask.h"
#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"
#include "TestTaskSolution/Weapons/Data/TtWeaponData.h"

UTtEquipWeaponAbility::UTtEquipWeaponAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UTtEquipWeaponAbility::ActivateAbility(
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

	ETtWeaponSlot NewWeaponSlot = WeaponComponent->GetCurrentWeaponSlot();
	if (TriggerEventData)
	{
		const UTtWeaponData* WeaponData = Cast<UTtWeaponData>(TriggerEventData->OptionalObject);
		if (WeaponData)
		{
			NewWeaponSlot = FindWeaponSlotByData(WeaponComponent, WeaponData, NewWeaponSlot);
		}
	}

	UE_LOG(LogTestTask, Log, TEXT("Equip weapon reload animation placeholder"));
	WeaponComponent->SetCurrentWeaponSlot(NewWeaponSlot);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
