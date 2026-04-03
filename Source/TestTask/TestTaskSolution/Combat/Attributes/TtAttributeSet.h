// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TtAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Main gameplay attributes for TestTaskSolution.
 */
UCLASS()
class TESTTASK_API UTtAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UTtAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTtAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTtAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PhysicalResistance, Category="Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UTtAttributeSet, PhysicalResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_FireResistance, Category="Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UTtAttributeSet, FireResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WaterResistance, Category="Attributes")
	FGameplayAttributeData WaterResistance;
	ATTRIBUTE_ACCESSORS(UTtAttributeSet, WaterResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Damage, Category="Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UTtAttributeSet, Damage)

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_WaterResistance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldValue);
};
