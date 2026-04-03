// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Characters/TtCharacter.h"
#include "AbilitySystemComponent.h"
#include "TestTaskSolution/Combat/Attributes/TtAttributeSet.h"
#include "TestTaskSolution/Combat/Effects/TtInitializeAttributes.h"
#include "TestTaskSolution/Core/TtAbilitySystemComponent.h"
#include "TestTaskSolution/PlayerState/TtPlayerState.h"

ATtCharacter::ATtCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	AttributeSetClass = UTtAttributeSet::StaticClass();
	InitialAttributesEffectClass = UTtInitializeAttributes::StaticClass();
}

UAbilitySystemComponent* ATtCharacter::GetAbilitySystemComponent() const
{
	if (const ATtPlayerState* TtPlayerState = GetPlayerState<ATtPlayerState>())
	{
		return TtPlayerState->GetAbilitySystemComponent();
	}

	return CharacterAbilitySystemComponent;
}

USkeletalMeshComponent* ATtCharacter::GetWeaponHandler() const
{
	return nullptr;
}

UTtWeaponComponent* ATtCharacter::GetWeaponComponent() const
{
	return nullptr;
}

void ATtCharacter::InitializeCharacterOwnedAbilitySystem()
{
	if (!CharacterAbilitySystemComponent)
	{
		return;
	}

	CharacterAbilitySystemComponent->InitializeAbilitySystemComponent(this, this);
	CharacterAttributeSet = CharacterAbilitySystemComponent->GetSet<UTtAttributeSet>();

	if (!HasAuthority())
	{
		return;
	}

	CharacterAbilitySystemComponent->InitializeAttributeSet(AttributeSetClass, InitialAttributesEffectClass);
	CharacterAttributeSet = CharacterAbilitySystemComponent->GetSet<UTtAttributeSet>();
}

void ATtCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
