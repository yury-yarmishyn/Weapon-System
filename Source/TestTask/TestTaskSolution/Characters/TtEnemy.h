// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TtCharacter.h"
#include "TtEnemy.generated.h"

struct FGameplayTag;
class UWidgetComponent;

UCLASS()
class TESTTASK_API ATtEnemy : public ATtCharacter
{
	GENERATED_BODY()

public:
	ATtEnemy();

protected:
	virtual void InitializeAbilitySystem() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void BindBurningTagEvents();
	void HandleBurningTagChanged(FGameplayTag InTag, int32 NewCount);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;

	FDelegateHandle BurningTagChangedHandle;
};
