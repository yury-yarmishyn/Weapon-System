// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/PlayerState/TtPlayerState.h"
#include "TestTaskSolution/Combat/AbilityComponent/TtAbilitySystemComponent.h"

ATtPlayerState::ATtPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTtAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

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
	return AbilitySystemComponent ? AbilitySystemComponent->GetAttributeSet() : nullptr;
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

	if (!HasAuthority())
	{
		return;
	}

	AbilitySystemComponent->InitializeAttributeSet();
}
