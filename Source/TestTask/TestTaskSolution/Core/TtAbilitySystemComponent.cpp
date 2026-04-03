// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Core/TtAbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "TestTask.h"
#include "GameFramework/PlayerState.h"

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

void UTtAbilitySystemComponent::InitializeAttributeSet(
	TSubclassOf<UAttributeSet> InAttributeSetClass,
	TSubclassOf<UGameplayEffect> InitialEffect)
{
	if (!InAttributeSetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("InitializeAttributeSet: AttributeSetClass is null"));
		return;
	}

	AActor* OwningActor = GetOwner();
	if (!OwningActor)
	{
		UE_LOG(LogTemp, Error, TEXT("InitializeAttributeSet: Owning actor is null"));
		return;
	}

	UAttributeSet* AttributeSet = NewObject<UAttributeSet>(OwningActor, InAttributeSetClass);
	if (!AttributeSet)
	{
		UE_LOG(LogTemp, Error, TEXT("InitializeAttributeSet: Failed to create AttributeSet"));
		return;
	}
	
	AddAttributeSetSubobject(AttributeSet);

	if (InitialEffect)
	{
		FGameplayEffectContextHandle Context = MakeEffectContext();
		Context.AddSourceObject(GetAvatarActor() ? GetAvatarActor() : OwningActor);

		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(InitialEffect, 1.f, Context);
		if (SpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("InitializeAttributeSet: Failed to create GE Spec"));
		}
	}
}
