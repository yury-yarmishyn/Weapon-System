// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Combat/AbilityComponent/TtAbilitySystemComponent.h"
#include "TestTask.h"
#include "TestTaskSolution/Combat/Attributes/TtAttributeSet.h"

UTtAbilitySystemComponent::UTtAbilitySystemComponent()
{
	AttributeSetClass = UTtAttributeSet::StaticClass();
}

void UTtAbilitySystemComponent::InitializeAbilitySystemComponent(AActor* InOwnerActor, AActor* InAvatarActor)
{
	if (!InOwnerActor)
	{
		UE_LOG(LogTestTask, Error, TEXT("InitializeAbilitySystemComponent: InOwnerActor is null"));
		return;
	}
	
	if (!InAvatarActor)
	{
		UE_LOG(LogTestTask, Error, TEXT("InitializeAbilitySystemComponent: InAvatarActor is null"));
		return;
	}
	
	InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	UE_LOG(
		LogTestTask,
		Log,
		TEXT("ASC initialized. Owner=%s Avatar=%s"),
		*GetNameSafe(InOwnerActor),
		*GetNameSafe(InAvatarActor));
}

void UTtAbilitySystemComponent::InitializeAttributeSet()
{
	if (AttributeSet)
	{
		UE_LOG(
			LogTestTask,
			Verbose,
			TEXT("InitializeAttributeSet skipped: AttributeSet already initialized for %s"),
			*GetNameSafe(GetOwner()));
		return;
	}

	if (!AttributeSetClass)
	{
		UE_LOG(LogTestTask, Error, TEXT("InitializeAttributeSet: AttributeSetClass is null"));
		return;
	}

	AActor* OwningActor = GetOwner();
	if (!OwningActor)
	{
		UE_LOG(LogTestTask, Error, TEXT("InitializeAttributeSet: Owning actor is null"));
		return;
	}

	UAttributeSet* CreatedAttributeSet = NewObject<UAttributeSet>(OwningActor, AttributeSetClass);
	if (!CreatedAttributeSet)
	{
		UE_LOG(LogTestTask, Error, TEXT("InitializeAttributeSet: Failed to create AttributeSet"));
		return;
	}
	
	AddAttributeSetSubobject(CreatedAttributeSet);
	AttributeSet = Cast<UTtAttributeSet>(CreatedAttributeSet);
	UE_LOG(
		LogTestTask,
		Log,
		TEXT("InitializeAttributeSet: Created AttributeSet %s for %s"),
		*GetNameSafe(AttributeSet),
		*GetNameSafe(OwningActor));

	if (InitializeAttributesEffectClass)
	{
		FGameplayEffectContextHandle Context = MakeEffectContext();
		Context.AddSourceObject(GetAvatarActor() ? GetAvatarActor() : OwningActor);

		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(InitializeAttributesEffectClass, 1.f, Context);
		if (SpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			UE_LOG(
				LogTestTask,
				Log,
				TEXT("InitializeAttributeSet: Applied init effect %s to %s"),
				*GetNameSafe(InitializeAttributesEffectClass),
				*GetNameSafe(OwningActor));
		}
		else
		{
			UE_LOG(LogTestTask, Error, TEXT("InitializeAttributeSet: Failed to create GE Spec"));
		}
	}
	else
	{
		UE_LOG(
			LogTestTask,
			Warning,
			TEXT("InitializeAttributeSet: InitializeAttributesEffectClass is not set for %s"),
			*GetNameSafe(OwningActor));
	}
}

const UTtAttributeSet* UTtAbilitySystemComponent::GetAttributeSet() const
{
	return AttributeSet;
}
