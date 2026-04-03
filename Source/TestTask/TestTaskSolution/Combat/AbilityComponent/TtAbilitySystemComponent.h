// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TtAbilitySystemComponent.generated.h"

class UAttributeSet;
class UGameplayEffect;
class UTtAttributeSet;

UCLASS()
class TESTTASK_API UTtAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UTtAbilitySystemComponent();
	
	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TSubclassOf<UAttributeSet> AttributeSetClass;

	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TSubclassOf<UGameplayEffect> InitializeAttributesEffectClass;

	UPROPERTY()
	const UTtAttributeSet* AttributeSet = nullptr;
	
public:
	void InitializeAbilitySystemComponent(AActor* InOwnerActor, AActor* InAvatarActor);
	void InitializeAttributeSet();
	const UTtAttributeSet* GetAttributeSet() const;
};
