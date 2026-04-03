// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "TtInitializeAttributes.generated.h"

/**
 * Startup GE used to initialize the main attribute set.
 */
UCLASS()
class TESTTASK_API UTtInitializeAttributes : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UTtInitializeAttributes();
};

