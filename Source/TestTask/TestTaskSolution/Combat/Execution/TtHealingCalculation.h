// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "TtHealingCalculation.generated.h"

/**
 * Example MMC: computes healing amount and clamps it to missing health.
 */
UCLASS()
class TESTTASK_API UTtHealingCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UTtHealingCalculation();

protected:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition HealthDef;
	FGameplayEffectAttributeCaptureDefinition MaxHealthDef;
};
