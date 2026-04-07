// Fill out your copyright notice in the Description page of Project Settings.

#include "TestTaskSolution/Combat/GameplayAbilities/TtEquipWeaponAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
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
	const UTtWeaponData* RequestedWeaponData = nullptr;
	if (TriggerEventData)
	{
		const UTtWeaponData* WeaponData = Cast<const UTtWeaponData>(TriggerEventData->OptionalObject);
		if (WeaponData)
		{
			RequestedWeaponData = WeaponData;
			NewWeaponSlot = FindWeaponSlotByData(WeaponComponent, WeaponData, NewWeaponSlot);
		}
	}

	if (!RequestedWeaponData)
	{
		UE_LOG(
			LogTestTask,
			Warning,
			TEXT("[%s] EquipWeaponAbility rejected: missing weapon data in OptionalObject"),
			*GetNameSafe(WeaponComponent->GetOwner()));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	PendingWeaponComponent = WeaponComponent;
	PendingWeaponData = const_cast<UTtWeaponData*>(RequestedWeaponData);
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

	const UTtWeaponData* EquippedWeaponData = PendingWeaponData.Get();

	UAbilitySystemComponent* AbilitySystemComponent = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
	if (!AbilitySystemComponent)
	{
		AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(WeaponComponent->GetOwner());
	}

	bool bBasicDamageEffectApplied = false;
	const float WeaponBasicDamage = EquippedWeaponData ? FMath::Max(0.f, EquippedWeaponData->BasicDamage) : 0.f;
	if (AbilitySystemComponent && EquippedWeaponData && EquippedWeaponData->DamageEffectClass)
	{
		const FGameplayTag BasicDamageDataTag = EquippedWeaponData->BasicDamageDataTag.IsValid()
			? EquippedWeaponData->BasicDamageDataTag
			: TtGameplayTags::TAG_Data_Damage;

		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			EquippedWeaponData->DamageEffectClass,
			1.f,
			EffectContext);
		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(BasicDamageDataTag, WeaponBasicDamage);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			bBasicDamageEffectApplied = true;
		}
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
		TEXT("[%s] Weapon equipped. WeaponSlot=%d BasicDamage=%.2f BasicDamageGEApplied=%s"),
		*GetNameSafe(WeaponComponent->GetOwner()),
		static_cast<int32>(PendingWeaponSlot),
		WeaponBasicDamage,
		bBasicDamageEffectApplied ? TEXT("true") : TEXT("false"));

	return true;
}
