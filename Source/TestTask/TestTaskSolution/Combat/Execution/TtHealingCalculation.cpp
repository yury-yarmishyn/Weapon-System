// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Combat/Execution/TtHealingCalculation.h"

#include "GameplayEffectExtension.h"
#include "TestTaskSolution/Combat/Attributes/TtAttributeSet.h"

UTtHealingCalculation::UTtHealingCalculation()
{
	DamageDef = FGameplayEffectAttributeCaptureDefinition(
		UTtAttributeSet::GetDamageAttribute(),
		EGameplayEffectAttributeCaptureSource::Source,
		false);

	RelevantAttributesToCapture.Add(DamageDef);
}

float UTtHealingCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float HealingValue = 0.f;
	GetCapturedAttributeMagnitude(DamageDef, Spec, EvaluationParameters, HealingValue);

	const float HealingRaw = FMath::Max(0.f, HealingValue);
	return HealingRaw;
}
