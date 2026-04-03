// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Characters/TtCharacter.h"
#include "Combat/AbilityComponent/TtAbilitySystemComponent.h"
#include "TestTaskSolution/PlayerState/TtPlayerState.h"

ATtCharacter::ATtCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

UAbilitySystemComponent* ATtCharacter::GetAbilitySystemComponent() const
{
	if (const ATtPlayerState* TtPlayerState = GetPlayerState<ATtPlayerState>())
	{
		return TtPlayerState->GetAbilitySystemComponent();
	}

	return AbilitySystemComponent;
}
