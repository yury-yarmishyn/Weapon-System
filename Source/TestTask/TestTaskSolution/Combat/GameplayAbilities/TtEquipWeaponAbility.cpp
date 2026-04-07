// Fill out your copyright notice in the Description page of Project Settings.

#include "TestTaskSolution/Combat/GameplayAbilities/TtEquipWeaponAbility.h"

#include "Core/TtGameplayTags.h"
#include "TestTask.h"
#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"
#include "TestTaskSolution/Weapons/Data/TtWeaponData.h"
#include "TestTaskSolution/Weapons/Interfaces/TtWeaponInterface.h"
#include "Components/SkeletalMeshComponent.h"

UTtEquipWeaponAbility::UTtEquipWeaponAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	const FGameplayTag TriggerTag = TtGameplayTags::TAG_Event_Weapon_Equip;

	if (TriggerTag.IsValid())
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		TriggerData.TriggerTag = TriggerTag;
		AbilityTriggers.Add(TriggerData);
	}
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

	PendingWeaponComponent = WeaponComponent;
	PendingWeaponSlot = NewWeaponSlot;

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UTtEquipWeaponAbility::CommitAbility(
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

	WeaponComponent->SetCurrentWeaponSlot(PendingWeaponSlot);
	if (const ETtAmmoSlot* SavedAmmoSlot = WeaponComponent->GetWeaponSlotByAmmoSlot().Find(PendingWeaponSlot))
	{
		WeaponComponent->SetCurrentAmmoSlot(*SavedAmmoSlot);
	}

	const UTtWeaponData* EquippedWeaponData = nullptr;
	if (const UTtWeaponData* const* WeaponDataPtr = WeaponComponent->GetWeaponSlotByWeaponData().Find(PendingWeaponSlot))
	{
		EquippedWeaponData = *WeaponDataPtr;
	}

	if (ActorInfo)
	{
		AActor* AvatarActor = ActorInfo->AvatarActor.Get();
		if (!AvatarActor)
		{
			AvatarActor = ActorInfo->OwnerActor.Get();
		}

		if (AvatarActor && AvatarActor->GetClass()->ImplementsInterface(UTtWeaponInterface::StaticClass()))
		{
			if (USkeletalMeshComponent* WeaponMesh = ITtWeaponInterface::Execute_GetWeaponMesh(AvatarActor))
			{
				WeaponMesh->SetSkeletalMesh(EquippedWeaponData ? EquippedWeaponData->WeaponMesh : nullptr);
				WeaponMesh->SetAnimInstanceClass(EquippedWeaponData ? EquippedWeaponData->AnimInstanceClass : nullptr);
			}
		}
	}

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Weapon equipped. WeaponSlot=%d"),
		*GetNameSafe(WeaponComponent->GetOwner()),
		static_cast<int32>(PendingWeaponSlot));

	return true;
}
