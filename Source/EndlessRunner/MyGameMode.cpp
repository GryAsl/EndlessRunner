// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyGameMode.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"
#include "MyPlayerController.h"
#include "TutorialHUD.h"
#include "UObject/ConstructorHelpers.h"

AMyGameMode::AMyGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = AMyPawn::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	HUDClass = ATutorialHUD::StaticClass();

	static ConstructorHelpers::FClassFinder<UUserWidget> WinMenuWidgetClass(TEXT("/Game/Blueprints/Widgets/WinMenu"));
	if (WinMenuWidgetClass.Succeeded())
	{
		WinMenuClass = WinMenuWidgetClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LoseMenuWidgetClass(TEXT("/Game/Blueprints/Widgets/LoseMenu"));
	if (LoseMenuWidgetClass.Succeeded())
	{
		LoseMenuClass = LoseMenuWidgetClass.Class;
	}
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	RemainingTime = RunDuration;
	LowSpeedElapsed = 0.0f;
	LoseReason = EHighwayLoseReason::None;

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

	const AMyPawn* PlayerPawn = Cast<AMyPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!PlayerPawn)
	{
		return;
	}

	if (PlayerPawn->GetHealth() <= 0)
	{
		LoseRun(EHighwayLoseReason::NoHealth);
		return;
	}

	if (PlayerPawn->GetSpeedKmh() < MinimumSpeedKmh)
	{
		LowSpeedElapsed += DeltaSeconds;
		if (LowSpeedElapsed >= LowSpeedGraceSeconds)
		{
			LoseRun(EHighwayLoseReason::LowSpeed);
			return;
		}
	}
	else
	{
		LowSpeedElapsed = 0.0f;
	}

	RemainingTime = FMath::Max(0.0f, RemainingTime - DeltaSeconds);
	if (RemainingTime <= 0.0f)
	{
		WinRun();
	}
}

void AMyGameMode::StartHighwayRun()
{
	RunState = EHighwayRunState::Running;
	LoseReason = EHighwayLoseReason::None;
	RemainingTime = RunDuration;
	LowSpeedElapsed = 0.0f;
}

void AMyGameMode::WinRun()
{
	if (RunState == EHighwayRunState::Running)
	{
		RunState = EHighwayRunState::Won;
		LoseReason = EHighwayLoseReason::None;
		ShowEndMenu(WinMenuClass);
	}
}

void AMyGameMode::LoseRun(EHighwayLoseReason Reason)
{
	if (RunState == EHighwayRunState::Running)
	{
		RunState = EHighwayRunState::Lost;
		LoseReason = Reason;
		ShowEndMenu(LoseMenuClass);
	}
}

float AMyGameMode::GetLowSpeedSecondsRemaining() const
{
	return FMath::Clamp(LowSpeedGraceSeconds - LowSpeedElapsed, 0.0f, LowSpeedGraceSeconds);
}

void AMyGameMode::ShowEndMenu(TSubclassOf<UUserWidget> MenuClass)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController || !MenuClass)
	{
		return;
	}

	if (ActiveEndMenu)
	{
		ActiveEndMenu->RemoveFromParent();
		ActiveEndMenu = nullptr;
	}

	ActiveEndMenu = CreateWidget<UUserWidget>(PlayerController, MenuClass);
	if (!ActiveEndMenu)
	{
		return;
	}

	ActiveEndMenu->AddToViewport();

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(ActiveEndMenu->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetIgnoreMoveInput(true);
	PlayerController->SetIgnoreLookInput(true);
}
