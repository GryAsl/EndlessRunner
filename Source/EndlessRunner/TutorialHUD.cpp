// Copyright Epic Games, Inc. All Rights Reserved.

#include "TutorialHUD.h"

#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "MyGameMode.h"
#include "MyPawn.h"

void ATutorialHUD::DrawHUD()
{
	Super::DrawHUD();

	const AMyPawn* MyPawn = PlayerOwner ? Cast<AMyPawn>(PlayerOwner->GetPawn()) : nullptr;
	const AMyGameMode* HighwayGameMode = GetWorld() ? Cast<AMyGameMode>(GetWorld()->GetAuthGameMode()) : nullptr;
	const float SpeedKmh = MyPawn ? MyPawn->GetSpeedKmh() : 0.0f;
	const float DistanceMeters = MyPawn ? MyPawn->GetDistanceTravelledMeters() : 0.0f;
	const float TargetThrottle = MyPawn ? MyPawn->GetTargetThrottle() * 100.0f : 0.0f;
	const float EffectiveThrottle = MyPawn ? MyPawn->GetEffectiveThrottle() * 100.0f : 0.0f;
	const float NitroPercent = MyPawn ? MyPawn->GetNitroPercent() * 100.0f : 0.0f;
	const int32 Health = MyPawn ? MyPawn->GetHealth() : 0;
	const bool bNitroActive = MyPawn && MyPawn->IsNitroActive();

	const float X = 40.0f;
	float Y = 40.0f;
	const float Scale = 1.35f;
	const FColor TextColor = FColor::White;
	const FColor AccentColor = bNitroActive ? FColor::Cyan : FColor::Silver;

	DrawText(HighwayGameMode ? TEXT("HIGHWAY ESCAPE") : TEXT("HIGHWAY ESCAPE - TUTORIAL"), TextColor, X, Y, GEngine->GetMediumFont(), Scale);
	Y += 42.0f;
	DrawText(TEXT("W / Up: Raise throttle"), TextColor, X, Y, GEngine->GetSmallFont(), Scale);
	Y += 28.0f;
	DrawText(TEXT("S / Down: Lower throttle"), TextColor, X, Y, GEngine->GetSmallFont(), Scale);
	Y += 28.0f;
	DrawText(TEXT("A / D: Steer"), TextColor, X, Y, GEngine->GetSmallFont(), Scale);
	Y += 28.0f;
	DrawText(TEXT("Shift: Nitro"), TextColor, X, Y, GEngine->GetSmallFont(), Scale);
	Y += 42.0f;
	DrawText(FString::Printf(TEXT("Speed: %.0f km/h"), SpeedKmh), TextColor, X, Y, GEngine->GetSmallFont(), Scale);
	Y += 28.0f;
	DrawText(FString::Printf(TEXT("Throttle target: %.0f%%"), TargetThrottle), TextColor, X, Y, GEngine->GetSmallFont(), Scale);
	Y += 28.0f;
	DrawText(FString::Printf(TEXT("Effective throttle: %.0f%%"), EffectiveThrottle), AccentColor, X, Y, GEngine->GetSmallFont(), Scale);
	Y += 28.0f;
	DrawText(FString::Printf(TEXT("Nitro: %.0f%%"), NitroPercent), AccentColor, X, Y, GEngine->GetSmallFont(), Scale);
	Y += 42.0f;
	DrawText(FString::Printf(TEXT("Health: %d"), Health), Health > 30 ? TextColor : FColor::Red, X, Y, GEngine->GetSmallFont(), Scale);
	Y += 28.0f;
	DrawText(FString::Printf(TEXT("Distance: %.0f m"), DistanceMeters), TextColor, X, Y, GEngine->GetSmallFont(), Scale);
	Y += 28.0f;

	if (!HighwayGameMode)
	{
		DrawText(TEXT("Drive into the exit zone to start the highway escape."), FColor::Yellow, X, Y, GEngine->GetSmallFont(), Scale);
		return;
	}

	const EHighwayRunState RunState = HighwayGameMode->GetRunState();
	if (RunState == EHighwayRunState::Running)
	{
		DrawText(FString::Printf(TEXT("Time: %.0f"), HighwayGameMode->GetRemainingTime()), FColor::Yellow, X, Y, GEngine->GetSmallFont(), Scale);
		Y += 28.0f;

		if (SpeedKmh < HighwayGameMode->GetMinimumSpeedKmh())
		{
			DrawText(FString::Printf(TEXT("LOW SPEED - %.1f seconds left"), HighwayGameMode->GetLowSpeedSecondsRemaining()), FColor::Red, X, Y, GEngine->GetSmallFont(), Scale);
		}
	}
	else if (RunState == EHighwayRunState::Won)
	{
		DrawText(TEXT("ESCAPE SUCCESSFUL - You survived the highway chase."), FColor::Green, X, Y, GEngine->GetSmallFont(), Scale);
		Y += 28.0f;
		DrawText(FString::Printf(TEXT("Distance survived: %.0f m"), DistanceMeters), FColor::Green, X, Y, GEngine->GetSmallFont(), Scale);
	}
	else if (RunState == EHighwayRunState::Lost)
	{
		const TCHAR* LoseMessage = TEXT("ESCAPE FAILED");
		if (HighwayGameMode->GetLoseReason() == EHighwayLoseReason::LowSpeed)
		{
			LoseMessage = TEXT("ESCAPE FAILED - You stayed too slow for too long.");
		}
		else if (HighwayGameMode->GetLoseReason() == EHighwayLoseReason::NoHealth)
		{
			LoseMessage = TEXT("ESCAPE FAILED - Vehicle health depleted.");
		}

		DrawText(LoseMessage, FColor::Red, X, Y, GEngine->GetSmallFont(), Scale);
		Y += 28.0f;
		DrawText(FString::Printf(TEXT("Distance survived: %.0f m"), DistanceMeters), FColor::Red, X, Y, GEngine->GetSmallFont(), Scale);
	}
}
