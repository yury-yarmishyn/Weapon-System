// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Combat/Execution/TtPhysicalDamageCalculation.h"

#include "GameplayEffectExtension.h"
#include "GameplayTagContainer.h"
#include "Core/TtGameplayTags.h"
#include "TestTaskSolution/Combat/Attributes/TtAttributeSet.h"

namespace TtMMCHelpers
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

UTtPhysicalDamageCalculation::UTtPhysicalDamageCalculation()
{
	PhysicalResistanceDef = FGameplayEffectAttributeCaptureDefinition(
		UTtAttributeSet::GetPhysicalResistanceAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	RelevantAttributesToCapture.Add(PhysicalResistanceDef);
}

float UTtPhysicalDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float PhysicalResistance = 0.f;
	GetCapturedAttributeMagnitude(PhysicalResistanceDef, Spec, EvaluationParameters, PhysicalResistance);
	PhysicalResistance = FMath::Clamp(PhysicalResistance, 0.f, 100.f);

	const float RawDamage = TtMMCHelpers::GetSetByCallerOrLevel(
		Spec,
		TtGameplayTags::TAG_Data_Damage_Physical,
		Spec.GetLevel());
	return FMath::Max(0.f, RawDamage * (1.f - (PhysicalResistance / 100.f)));
}
