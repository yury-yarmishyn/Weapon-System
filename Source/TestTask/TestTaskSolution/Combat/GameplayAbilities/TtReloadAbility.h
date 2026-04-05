// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TtGameplayAbility.h"
#include "TtReloadAbility.generated.h"

UCLASS()
class TESTTASK_API UTtReloadAbility : public UTtGameplayAbility
{
	GENERATED_BODY()

public:
	UTtReloadAbility();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
