// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TutorialHUD.generated.h"

UCLASS()
class ATutorialHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
};
