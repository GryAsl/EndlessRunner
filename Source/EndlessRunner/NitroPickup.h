// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HighwayInteractable.h"
#include "NitroPickup.generated.h"

class AMyPawn;

UCLASS()
class ANitroPickup : public AHighwayInteractable
{
	GENERATED_BODY()

public:
	ANitroPickup();

protected:
	virtual void OnCollectedByPlayer(AMyPawn* PlayerPawn) override;
};
