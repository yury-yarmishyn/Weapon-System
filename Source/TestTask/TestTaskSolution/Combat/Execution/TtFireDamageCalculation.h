// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "TtFireDamageCalculation.generated.h"

/**
 * Resolves fire damage from SetByCaller and applies weighted resistances.
 */
UCLASS()
class TESTTASK_API UTtFireDamageCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UTtFireDamageCalculation();

protected:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition DamageDef;
	FGameplayEffectAttributeCaptureDefinition MaxHealthDef;
	FGameplayEffectAttributeCaptureDefinition PhysicalResistanceDef;
	FGameplayEffectAttributeCaptureDefinition FireResistanceDef;
	FGameplayEffectAttributeCaptureDefinition WaterResistanceDef;
};
