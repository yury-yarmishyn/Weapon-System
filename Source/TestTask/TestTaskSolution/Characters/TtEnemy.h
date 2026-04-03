// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TtCharacter.h"
#include "TtEnemy.generated.h"

UCLASS()
class TESTTASK_API ATtEnemy : public ATtCharacter
{
	GENERATED_BODY()

public:
	ATtEnemy();

protected:
	virtual void BeginPlay() override;
};
