// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TtGameplayAbility.h"
#include "TtEquipWeaponAbility.generated.h"

class UTtWeaponData;

UCLASS()
class TESTTASK_API UTtEquipWeaponAbility : public UTtGameplayAbility
{
	GENERATED_BODY()

public:
	UTtEquipWeaponAbility();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual bool CommitAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) override;

private:
	TWeakObjectPtr<UTtWeaponComponent> PendingWeaponComponent;
	TWeakObjectPtr<UTtWeaponData> PendingWeaponData;
	ETtWeaponSlot PendingWeaponSlot = ETtWeaponSlot::Slot1;
};
