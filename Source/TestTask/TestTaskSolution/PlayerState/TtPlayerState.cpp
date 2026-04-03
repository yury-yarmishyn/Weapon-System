// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/PlayerState/TtPlayerState.h"
#include "TestTaskSolution/Combat/Attributes/TtAttributeSet.h"
#include "TestTaskSolution/Combat/Effects/TtInitializeAttributes.h"
#include "TestTaskSolution/Core/TtAbilitySystemComponent.h"

ATtPlayerState::ATtPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTtAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetClass = UTtAttributeSet::StaticClass();
	InitialAttributesEffectClass = UTtInitializeAttributes::StaticClass();

	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* ATtPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UTtAbilitySystemComponent* ATtPlayerState::GetTtAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

const UTtAttributeSet* ATtPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void ATtPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!AbilitySystemComponent->GetOwnerActor() || !AbilitySystemComponent->GetAvatarActor())
	{
		AActor* AvatarActor = GetPawn();
		if (!AvatarActor)
		{
			AvatarActor = this;
		}

		AbilitySystemComponent->InitializeAbilitySystemComponent(this, AvatarActor);
	}

	AttributeSet = AbilitySystemComponent->GetSet<UTtAttributeSet>();

	if (!HasAuthority() || bAttributesInitialized)
	{
		return;
	}

	AbilitySystemComponent->InitializeAttributeSet(AttributeSetClass, InitialAttributesEffectClass);
	AttributeSet = AbilitySystemComponent->GetSet<UTtAttributeSet>();
	bAttributesInitialized = true;
}
