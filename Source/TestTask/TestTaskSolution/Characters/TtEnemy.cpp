// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Characters/TtEnemy.h"
#include "TestTaskSolution/Core/TtAbilitySystemComponent.h"

ATtEnemy::ATtEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	CharacterAbilitySystemComponent = CreateDefaultSubobject<UTtAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	CharacterAbilitySystemComponent->SetIsReplicated(true);
	CharacterAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void ATtEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitializeCharacterOwnedAbilitySystem();
}
