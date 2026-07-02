// Copyright Epic Games, Inc. All Rights Reserved.

#include "TutorialGameMode.h"

#include "MyPawn.h"
#include "MyPlayerController.h"
#include "TutorialHUD.h"

ATutorialGameMode::ATutorialGameMode()
{
	DefaultPawnClass = AMyPawn::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	HUDClass = ATutorialHUD::StaticClass();
}
