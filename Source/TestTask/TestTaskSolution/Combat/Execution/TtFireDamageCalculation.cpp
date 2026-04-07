// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Combat/Execution/TtFireDamageCalculation.h"

#include "GameplayEffectExtension.h"
#include "GameplayTagContainer.h"
#include "Core/TtGameplayTags.h"
#include "TestTaskSolution/Combat/Attributes/TtAttributeSet.h"

namespace TtFireMMCHelpers
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

UTtFireDamageCalculation::UTtFireDamageCalculation()
{
	FireResistanceDef = FGameplayEffectAttributeCaptureDefinition(
		UTtAttributeSet::GetFireResistanceAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	RelevantAttributesToCapture.Add(FireResistanceDef);
}

float UTtFireDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float FireResistance = 0.f;
	GetCapturedAttributeMagnitude(FireResistanceDef, Spec, EvaluationParameters, FireResistance);
	FireResistance = FMath::Clamp(FireResistance, 0.f, 100.f);

	const float RawDamage = TtFireMMCHelpers::GetSetByCallerOrLevel(
		Spec,
		TtGameplayTags::TAG_Data_Damage_Fire,
		Spec.GetLevel());
	return FMath::Max(0.f, RawDamage * (1.f - (FireResistance / 100.f)));
}
