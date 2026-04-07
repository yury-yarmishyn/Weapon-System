// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Combat/Attributes/TtAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "TestTask.h"

UTtAttributeSet::UTtAttributeSet()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitPhysicalResistance(0.0f);
	InitFireResistance(0.0f);
	InitWaterResistance(0.0f);
	InitDamage(0.0f);
}

void UTtAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTtAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTtAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTtAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTtAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTtAttributeSet, WaterResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTtAttributeSet, Damage, COND_None, REPNOTIFY_Always);
}

void UTtAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FString EffectName = Data.EffectSpec.Def ? Data.EffectSpec.Def->GetName() : TEXT("UnknownEffect");
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float OldHealth = GetHealth();
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		const float NewHealth = GetHealth();
		const float EvaluatedDelta = Data.EvaluatedData.Magnitude;
		if (EvaluatedDelta < 0.f)
		{
			UE_LOG(
				LogTestTask,
				Log,
				TEXT("[%s] Damage applied: %.2f via %s"),
				*GetNameSafe(GetOwningActor()),
				FMath::Abs(EvaluatedDelta),
				*EffectName);
		}
		else if (EvaluatedDelta > 0.f)
		{
			UE_LOG(
				LogTestTask,
				Log,
				TEXT("[%s] Healing applied: %.2f via %s"),
				*GetNameSafe(GetOwningActor()),
				EvaluatedDelta,
				*EffectName);
		}

		if (EffectName.Contains(TEXT("GE_Burning")))
		{
			UE_LOG(LogTestTask, Log, TEXT("[%s] On Fire tick"), *GetNameSafe(GetOwningActor()));
		}
		else if (EffectName.Contains(TEXT("GE_FireHit")))
		{
			UE_LOG(LogTestTask, Log, TEXT("[%s] On Fire applied"), *GetNameSafe(GetOwningActor()));
		}
		else if (EffectName.Contains(TEXT("GE_WaterHit")))
		{
			UE_LOG(LogTestTask, Log, TEXT("[%s] On Fire removed"), *GetNameSafe(GetOwningActor()));
		}

		if (OldHealth > 0.f && NewHealth <= 0.f)
		{
			UE_LOG(LogTestTask, Warning, TEXT("[%s] Health depleted. Death reached"), *GetNameSafe(GetOwningActor()));
		}

		return;
	}

	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetMaxHealth(FMath::Max(0.0f, GetMaxHealth()));
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
}

void UTtAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTtAttributeSet, Health, OldValue);
}

void UTtAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTtAttributeSet, MaxHealth, OldValue);
}

void UTtAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTtAttributeSet, PhysicalResistance, OldValue);
}

void UTtAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTtAttributeSet, FireResistance, OldValue);
}

void UTtAttributeSet::OnRep_WaterResistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTtAttributeSet, WaterResistance, OldValue);
}

void UTtAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTtAttributeSet, Damage, OldValue);
}
