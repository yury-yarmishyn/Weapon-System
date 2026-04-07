// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Combat/Execution/TtPhysicalDamageCalculation.h"

#include "GameplayEffectExtension.h"
#include "TestTaskSolution/Combat/Attributes/TtAttributeSet.h"

UTtPhysicalDamageCalculation::UTtPhysicalDamageCalculation()
{
	DamageDef = FGameplayEffectAttributeCaptureDefinition(
		UTtAttributeSet::GetDamageAttribute(),
		EGameplayEffectAttributeCaptureSource::Source,
		false);

	MaxHealthDef = FGameplayEffectAttributeCaptureDefinition(
		UTtAttributeSet::GetMaxHealthAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	PhysicalResistanceDef = FGameplayEffectAttributeCaptureDefinition(
		UTtAttributeSet::GetPhysicalResistanceAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	FireResistanceDef = FGameplayEffectAttributeCaptureDefinition(
		UTtAttributeSet::GetFireResistanceAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	WaterResistanceDef = FGameplayEffectAttributeCaptureDefinition(
		UTtAttributeSet::GetWaterResistanceAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	RelevantAttributesToCapture.Add(DamageDef);
	RelevantAttributesToCapture.Add(MaxHealthDef);
	RelevantAttributesToCapture.Add(PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(FireResistanceDef);
	RelevantAttributesToCapture.Add(WaterResistanceDef);
}

float UTtPhysicalDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float PhysicalResistance = 0.f;
	GetCapturedAttributeMagnitude(PhysicalResistanceDef, Spec, EvaluationParameters, PhysicalResistance);

	float FireResistance = 0.f;
	GetCapturedAttributeMagnitude(FireResistanceDef, Spec, EvaluationParameters, FireResistance);

	float WaterResistance = 0.f;
	GetCapturedAttributeMagnitude(WaterResistanceDef, Spec, EvaluationParameters, WaterResistance);

	float BaseDamageMultiplier = 0.f;
	GetCapturedAttributeMagnitude(DamageDef, Spec, EvaluationParameters, BaseDamageMultiplier);

	float MaxHealth = 0.f;
	GetCapturedAttributeMagnitude(MaxHealthDef, Spec, EvaluationParameters, MaxHealth);

	const float WeightedResistance =
		FMath::Max(0.f, PhysicalResistance) * 1.0f +
		FMath::Max(0.f, FireResistance) * 0.5f +
		FMath::Max(0.f, WaterResistance) * 0.5f;

	const float ResistanceScale = FMath::Max(1.f, MaxHealth);
	const float DamageMultiplier = ResistanceScale / (ResistanceScale + WeightedResistance);

	return FMath::Max(0.f, BaseDamageMultiplier) * DamageMultiplier;
}
