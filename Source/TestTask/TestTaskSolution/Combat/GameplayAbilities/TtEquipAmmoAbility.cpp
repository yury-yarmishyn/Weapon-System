// Fill out your copyright notice in the Description page of Project Settings.

#include "TestTaskSolution/Combat/GameplayAbilities/TtEquipAmmoAbility.h"

#include "TestTask.h"
#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"
#include "TestTaskSolution/Weapons/Data/TtAmmoData.h"

UTtEquipAmmoAbility::UTtEquipAmmoAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
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

	UE_LOG(LogTestTask, Log, TEXT("Equip ammo animation placeholder"));
	WeaponComponent->SetCurrentAmmoSlot(NewAmmoSlot);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
