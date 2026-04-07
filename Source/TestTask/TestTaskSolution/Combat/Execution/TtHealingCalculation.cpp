// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Combat/Execution/TtHealingCalculation.h"

#include "GameplayEffectExtension.h"
#include "GameplayTagContainer.h"
#include "Core/TtGameplayTags.h"
#include "TestTaskSolution/Combat/Attributes/TtAttributeSet.h"

namespace TtHealingMMCHelpers
{
	static float GetSetByCallerOrLevel(const FGameplayEffectSpec& Spec, const FGameplayTag DataTag, const float Fallback)
	{
		if (!DataTag.IsValid())
		{
			return Fallback;
		}

		return Spec.GetSetByCallerMagnitude(DataTag, false, Fallback);
	}
}

UTtHealingCalculation::UTtHealingCalculation()
{
	HealthDef = FGameplayEffectAttributeCaptureDefinition(
		UTtAttributeSet::GetHealthAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	MaxHealthDef = FGameplayEffectAttributeCaptureDefinition(
		UTtAttributeSet::GetMaxHealthAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	RelevantAttributesToCapture.Add(HealthDef);
	RelevantAttributesToCapture.Add(MaxHealthDef);
}

float UTtHealingCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float CurrentHealth = 0.f;
	float MaxHealth = 0.f;
	GetCapturedAttributeMagnitude(HealthDef, Spec, EvaluationParameters, CurrentHealth);
	GetCapturedAttributeMagnitude(MaxHealthDef, Spec, EvaluationParameters, MaxHealth);

	const float RawHealing = TtHealingMMCHelpers::GetSetByCallerOrLevel(
		Spec,
		TtGameplayTags::TAG_Data_Healing,
		Spec.GetLevel());
	const float MissingHealth = FMath::Max(0.f, MaxHealth - CurrentHealth);
	return FMath::Max(0.f, FMath::Min(RawHealing, MissingHealth));
}
