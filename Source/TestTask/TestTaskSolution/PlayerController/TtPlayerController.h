// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TestTaskPlayerController.h"
#include "TtPlayerController.generated.h"

class UInputMappingContext;

/**
 * TestTaskSolution player controller.
 * Input mapping contexts are initialized explicitly from Blueprint.
 */
UCLASS(config="Game")
class TESTTASK_API ATtPlayerController : public ATestTaskPlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Input")
	void InitializeInputMappingContext(UInputMappingContext* MappingContext, int32 Priority = 0);
};

