// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TtPlayerController.generated.h"

class UInputMappingContext;
class APawn;

UCLASS(config="Game")
class TESTTASK_API ATtPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;
	
protected:
	virtual void SetupInputComponent() override;
};
