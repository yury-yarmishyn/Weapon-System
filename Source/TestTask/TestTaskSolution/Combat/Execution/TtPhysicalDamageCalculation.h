// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "TtPhysicalDamageCalculation.generated.h"

/**
 * Example MMC: converts incoming physical damage into final value using target PhysicalResistance.
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
	FGameplayEffectAttributeCaptureDefinition PhysicalResistanceDef;
};
