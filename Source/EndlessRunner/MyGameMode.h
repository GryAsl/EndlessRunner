// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

UENUM()
enum class EHighwayRunState : uint8
{
	Waiting,
	Running,
	Won,
	Lost
};

UCLASS()
class AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameMode();

	virtual void Tick(float DeltaSeconds) override;

	void StartHighwayRun();
	void WinRun();
	void LoseRun();

	float GetRemainingTime() const { return RemainingTime; }
	float GetLowSpeedSecondsRemaining() const;
	EHighwayRunState GetRunState() const { return RunState; }
	float GetMinimumSpeedKmh() const { return MinimumSpeedKmh; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Highway|Rules")
	bool bAutoStartHighwayRun = true;

	UPROPERTY(EditAnywhere, Category = "Highway|Rules")
	float RunDuration = 120.0f;

	UPROPERTY(EditAnywhere, Category = "Highway|Rules")
	float MinimumSpeedKmh = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Highway|Rules")
	float LowSpeedGraceSeconds = 5.0f;

private:
	EHighwayRunState RunState = EHighwayRunState::Waiting;
	float RemainingTime = 120.0f;
	float LowSpeedElapsed = 0.0f;
};
