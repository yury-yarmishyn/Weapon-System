// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "TtFireDamageCalculation.generated.h"

/**
 * Example calculation: converts incoming fire damage into final value using target FireResistance.
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
	FGameplayEffectAttributeCaptureDefinition FireResistanceDef;
};
