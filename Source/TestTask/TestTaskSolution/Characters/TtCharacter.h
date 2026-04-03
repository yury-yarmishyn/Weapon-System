// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "TestTaskSolution/Weapons/Interfaces/TtWeaponInterface.h"
#include "TtCharacter.generated.h"

class UAbilitySystemComponent;
class UInputComponent;
class UGameplayEffect;
class USkeletalMeshComponent;
class UAttributeSet;
class UTtAbilitySystemComponent;
class UTtAttributeSet;
class UTtWeaponComponent;

/**
 * Player pawn representation.
 * Owns visual WeaponHandler and gameplay WeaponComponent.
 */
UCLASS()
class TESTTASK_API ATtCharacter : public ACharacter, public IAbilitySystemInterface, public ITtWeaponInterface
{
	GENERATED_BODY()

public:
	ATtCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual USkeletalMeshComponent* GetWeaponHandler() const override;
	virtual UTtWeaponComponent* GetWeaponComponent() const override;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void InitializeCharacterOwnedAbilitySystem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS")
	TObjectPtr<UTtAbilitySystemComponent> CharacterAbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category="GAS")
	const UTtAttributeSet* CharacterAttributeSet = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TSubclassOf<UAttributeSet> AttributeSetClass;

	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TSubclassOf<UGameplayEffect> InitialAttributesEffectClass;
};
