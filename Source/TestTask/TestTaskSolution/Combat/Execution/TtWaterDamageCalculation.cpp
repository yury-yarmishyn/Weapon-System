// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Combat/Execution/TtWaterDamageCalculation.h"

#include "GameplayEffectExtension.h"
#include "GameplayTagContainer.h"
#include "Core/TtGameplayTags.h"
#include "TestTaskSolution/Combat/Attributes/TtAttributeSet.h"

namespace TtWaterMMCHelpers
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

UTtWaterDamageCalculation::UTtWaterDamageCalculation()
{
	WaterResistanceDef = FGameplayEffectAttributeCaptureDefinition(
		UTtAttributeSet::GetWaterResistanceAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	RelevantAttributesToCapture.Add(WaterResistanceDef);
}

float UTtWaterDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float WaterResistance = 0.f;
	GetCapturedAttributeMagnitude(WaterResistanceDef, Spec, EvaluationParameters, WaterResistance);
	WaterResistance = FMath::Clamp(WaterResistance, 0.f, 100.f);

	const float RawDamage = TtWaterMMCHelpers::GetSetByCallerOrLevel(
		Spec,
		TtGameplayTags::TAG_Data_Damage_Water,
		Spec.GetLevel());
	return FMath::Max(0.f, RawDamage * (1.f - (WaterResistance / 100.f)));
}
