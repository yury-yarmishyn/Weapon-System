// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "TtPhysicalDamageCalculation.generated.h"

/**
 * Resolves physical damage from SetByCaller and applies weighted resistances.
 */
UCLASS()
class TESTTASK_API UTtPhysicalDamageCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UTtPhysicalDamageCalculation();

protected:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition DamageDef;
	FGameplayEffectAttributeCaptureDefinition MaxHealthDef;
	FGameplayEffectAttributeCaptureDefinition PhysicalResistanceDef;
	FGameplayEffectAttributeCaptureDefinition FireResistanceDef;
	FGameplayEffectAttributeCaptureDefinition WaterResistanceDef;
};
