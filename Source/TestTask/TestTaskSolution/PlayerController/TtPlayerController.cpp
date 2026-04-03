// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/PlayerController/TtPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "TestTask.h"

void ATtPlayerController::InitializeInputMappingContext(UInputMappingContext* MappingContext, int32 Priority)
{
	if (!IsLocalPlayerController())
	{
		return;
	}

	if (!MappingContext)
	{
		UE_LOG(LogTestTask, Warning, TEXT("ATtPlayerController: MappingContext is null for %s"), *GetNameSafe(this));
		return;
	}

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		UE_LOG(LogTestTask, Warning, TEXT("ATtPlayerController: LocalPlayer is null for %s"), *GetNameSafe(this));
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->AddMappingContext(MappingContext, Priority);
		return;
	}

	UE_LOG(LogTestTask, Warning, TEXT("ATtPlayerController: EnhancedInput subsystem not found for %s"), *GetNameSafe(this));
}
