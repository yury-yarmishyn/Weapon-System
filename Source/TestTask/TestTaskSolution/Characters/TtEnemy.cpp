// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Characters/TtEnemy.h"
#include "TestTaskSolution/Combat/AbilityComponent/TtAbilitySystemComponent.h"

ATtEnemy::ATtEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UTtAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void ATtEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitializeCharacterOwnedAbilitySystem();
}

void ATtEnemy::InitializeCharacterOwnedAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->InitializeAbilitySystemComponent(this, this);

	if (!HasAuthority())
	{
		return;
	}

	AbilitySystemComponent->InitializeAttributeSet();
}
