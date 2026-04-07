// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/PlayerController/TtPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "TestTask.h"

void ATtPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!IsLocalPlayerController())
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!Subsystem)
	{
		UE_LOG(LogTestTask, Error, TEXT("[%s] SetupInputComponent failed: EnhancedInput subsystem is null"), *GetNameSafe(this));
		return;
	}

	int32 AddedContexts = 0;
	for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
	{
		if (!CurrentContext)
		{
			continue;
		}

		Subsystem->AddMappingContext(CurrentContext, 0);
		++AddedContexts;
	}

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] SetupInputComponent completed. MappingContexts=%d"),
		*GetNameSafe(this),
		AddedContexts);

	if (AddedContexts == 0)
	{
		UE_LOG(LogTestTask, Warning, TEXT("[%s] No input mapping contexts were added"), *GetNameSafe(this));
	}
}
