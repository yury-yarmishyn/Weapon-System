// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "TtWaterDamageCalculation.generated.h"

/**
 * Resolves water damage from SetByCaller and applies weighted resistances.
 */
UCLASS()
class TESTTASK_API UTtWaterDamageCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UTtWaterDamageCalculation();

protected:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition DamageDef;
	FGameplayEffectAttributeCaptureDefinition MaxHealthDef;
	FGameplayEffectAttributeCaptureDefinition PhysicalResistanceDef;
	FGameplayEffectAttributeCaptureDefinition FireResistanceDef;
	FGameplayEffectAttributeCaptureDefinition WaterResistanceDef;
};
