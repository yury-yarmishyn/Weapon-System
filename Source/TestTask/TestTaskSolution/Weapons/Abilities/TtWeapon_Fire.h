// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "TtWeapon_Fire.generated.h"

/**
 * Minimal fire ability activated by gameplay event.
 */
UCLASS()
class TESTTASK_API UTtWeapon_Fire : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UTtWeapon_Fire();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

private:
	FGameplayTag FireEventTag;
};
