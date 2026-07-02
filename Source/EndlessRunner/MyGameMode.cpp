// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"
#include "MyPlayerController.h"
#include "TutorialHUD.h"

AMyGameMode::AMyGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = AMyPawn::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	HUDClass = ATutorialHUD::StaticClass();
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	RemainingTime = RunDuration;
	LowSpeedElapsed = 0.0f;

	if (bAutoStartHighwayRun)
	{
		StartHighwayRun();
	}
}

void AMyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (RunState != EHighwayRunState::Running)
	{
		return;
	}

	RemainingTime = FMath::Max(0.0f, RemainingTime - DeltaSeconds);
	if (RemainingTime <= 0.0f)
	{
		WinRun();
		return;
	}

	const AMyPawn* PlayerPawn = Cast<AMyPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!PlayerPawn)
	{
		return;
	}

	if (PlayerPawn->GetSpeedKmh() < MinimumSpeedKmh)
	{
		LowSpeedElapsed += DeltaSeconds;
		if (LowSpeedElapsed >= LowSpeedGraceSeconds)
		{
			LoseRun();
		}
	}
	else
	{
		LowSpeedElapsed = 0.0f;
	}
}

void AMyGameMode::StartHighwayRun()
{
	RunState = EHighwayRunState::Running;
	RemainingTime = RunDuration;
	LowSpeedElapsed = 0.0f;
}

void AMyGameMode::WinRun()
{
	if (RunState == EHighwayRunState::Running)
	{
		RunState = EHighwayRunState::Won;
	}
}

void AMyGameMode::LoseRun()
{
	if (RunState == EHighwayRunState::Running)
	{
		RunState = EHighwayRunState::Lost;
	}
}

float AMyGameMode::GetLowSpeedSecondsRemaining() const
{
	return FMath::Clamp(LowSpeedGraceSeconds - LowSpeedElapsed, 0.0f, LowSpeedGraceSeconds);
}
