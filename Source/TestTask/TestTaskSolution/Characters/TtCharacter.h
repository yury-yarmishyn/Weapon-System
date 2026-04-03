// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "TtCharacter.generated.h"

class UAbilitySystemComponent;
class UTtAbilitySystemComponent;

/**
 * Base character with shared GAS setup.
 */
UCLASS()
class TESTTASK_API ATtCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATtCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS")
	TObjectPtr<UTtAbilitySystemComponent> AbilitySystemComponent;
};
