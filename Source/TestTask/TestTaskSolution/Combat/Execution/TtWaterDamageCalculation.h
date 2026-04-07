// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "TtWaterDamageCalculation.generated.h"

/**
 * Example MMC: converts incoming water damage into final value using target WaterResistance.
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
	FGameplayEffectAttributeCaptureDefinition WaterResistanceDef;
};
