// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Core/TtGameMode.h"
#include "TestTaskSolution/Characters/TtPlayer.h"
#include "TestTaskSolution/PlayerController/TtPlayerController.h"
#include "TestTaskSolution/PlayerState/TtPlayerState.h"

ATtGameMode::ATtGameMode()
{
	DefaultPawnClass = ATtPlayer::StaticClass();
	PlayerControllerClass = ATtPlayerController::StaticClass();
	PlayerStateClass = ATtPlayerState::StaticClass();
}
