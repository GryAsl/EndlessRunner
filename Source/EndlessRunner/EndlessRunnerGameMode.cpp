// Copyright Epic Games, Inc. All Rights Reserved.

#include "EndlessRunnerGameMode.h"
#include "EndlessRunnerPlayerController.h"

AEndlessRunnerGameMode::AEndlessRunnerGameMode()
{
	PlayerControllerClass = AEndlessRunnerPlayerController::StaticClass();
}
