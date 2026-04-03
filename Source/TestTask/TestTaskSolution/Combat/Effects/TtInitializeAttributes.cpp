// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Combat/Effects/TtInitializeAttributes.h"
#include "TestTaskSolution/Combat/Attributes/TtAttributeSet.h"

namespace
{
	FGameplayModifierInfo MakeOverrideModifier(const FGameplayAttribute& Attribute, const float Value)
	{
		FGameplayModifierInfo Info;
		Info.Attribute = Attribute;
		Info.ModifierOp = EGameplayModOp::Override;
		Info.ModifierMagnitude = FScalableFloat(Value);
		return Info;
	}
}

UTtInitializeAttributes::UTtInitializeAttributes()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	Modifiers.Add(MakeOverrideModifier(UTtAttributeSet::GetHealthAttribute(), 100.0f));
	Modifiers.Add(MakeOverrideModifier(UTtAttributeSet::GetMaxHealthAttribute(), 100.0f));
	Modifiers.Add(MakeOverrideModifier(UTtAttributeSet::GetPhysicalResistanceAttribute(), 0.0f));
	Modifiers.Add(MakeOverrideModifier(UTtAttributeSet::GetFireResistanceAttribute(), 0.0f));
	Modifiers.Add(MakeOverrideModifier(UTtAttributeSet::GetWaterResistanceAttribute(), 0.0f));
	Modifiers.Add(MakeOverrideModifier(UTtAttributeSet::GetDamageAttribute(), 0.0f));
}

