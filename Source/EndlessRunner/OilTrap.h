// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HighwayInteractable.h"
#include "OilTrap.generated.h"

class AMyPawn;

UCLASS()
class AOilTrap : public AHighwayInteractable
{
	GENERATED_BODY()

public:
	AOilTrap();

protected:
	virtual void OnCollectedByPlayer(AMyPawn* PlayerPawn) override;

	UPROPERTY(EditAnywhere, Category = "Trap")
	float PenaltyDuration = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Trap")
	float ForcedThrottle = 0.0f;
};
