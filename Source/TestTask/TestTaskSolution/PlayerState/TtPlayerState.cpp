// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/PlayerState/TtPlayerState.h"
#include "TestTaskSolution/Combat/AbilityComponent/TtAbilitySystemComponent.h"
#include "TestTask.h"

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
		UE_LOG(LogTestTask, Error, TEXT("[%s] BeginPlay: AbilitySystemComponent is null"), *GetNameSafe(this));
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
		UE_LOG(
			LogTestTask,
			Log,
			TEXT("[%s] BeginPlay: ASC actor info initialized. Avatar=%s"),
			*GetNameSafe(this),
			*GetNameSafe(AvatarActor));
	}

	if (!HasAuthority())
	{
		UE_LOG(LogTestTask, Verbose, TEXT("[%s] BeginPlay: skipping AttributeSet init on client"), *GetNameSafe(this));
		return;
	}

	AbilitySystemComponent->InitializeAttributeSet();
	UE_LOG(LogTestTask, Log, TEXT("[%s] BeginPlay: server AttributeSet initialization requested"), *GetNameSafe(this));
}
