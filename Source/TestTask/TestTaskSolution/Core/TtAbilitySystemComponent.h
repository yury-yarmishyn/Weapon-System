// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TtAbilitySystemComponent.generated.h"

class UAttributeSet;
class UGameplayEffect;

/**
 * Project-specific ASC wrapper.
 * Kept as an extension point for startup behavior and logging.
 */
UCLASS()
class TESTTASK_API UTtAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TSubclassOf<UGameplayEffect> InitializeAttributesEffectClass;
	
public:
	void InitializeAbilitySystemComponent(AActor* InOwnerActor, AActor* InAvatarActor);
	void InitializeAttributeSet(TSubclassOf<UAttributeSet> InAttributeSetClass, TSubclassOf<UGameplayEffect> InitialEffect);
};
