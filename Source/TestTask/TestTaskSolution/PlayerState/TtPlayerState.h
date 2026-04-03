// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "TtPlayerState.generated.h"

class UAbilitySystemComponent;
class UTtAbilitySystemComponent;
class UTtAttributeSet;

/**
 * Persistent gameplay state holder.
 * Owns ASC.
 */
UCLASS()
class TESTTASK_API ATtPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATtPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UTtAbilitySystemComponent* GetTtAbilitySystemComponent() const;
	const UTtAttributeSet* GetAttributeSet() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UTtAbilitySystemComponent> AbilitySystemComponent;
};
