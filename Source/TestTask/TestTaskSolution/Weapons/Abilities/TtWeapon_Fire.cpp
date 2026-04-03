// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Weapons/Abilities/TtWeapon_Fire.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayTagContainer.h"
#include "TestTask.h"
#include "TestTaskSolution/Core/TtGameplayTags.h"

UTtWeapon_Fire::UTtWeapon_Fire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	FTtGameplayTags::InitializeNativeGameplayTags();
	FireEventTag = FTtGameplayTags::Get().Event_Weapon_Fire;

	if (FireEventTag.IsValid())
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		TriggerData.TriggerTag = FireEventTag;
		AbilityTriggers.Add(TriggerData);
	}
}

void UTtWeapon_Fire::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	UE_LOG(
		LogTestTask,
		Log,
		TEXT("GA_Weapon_Fire activated for %s."),
		*GetNameSafe(ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr));

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
