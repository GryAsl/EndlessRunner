// Copyright Epic Games, Inc. All Rights Reserved.

#include "OilTrap.h"

#include "Components/StaticMeshComponent.h"
#include "MyPawn.h"

AOilTrap::AOilTrap()
{
	if (VisualMesh)
	{
		VisualMesh->SetRelativeScale3D(FVector(1.4f, 1.4f, 0.08f));
	}
}

void AOilTrap::OnCollectedByPlayer(AMyPawn* PlayerPawn)
{
	if (PlayerPawn)
	{
		PlayerPawn->ApplyTrafficHitPenalty(PenaltyDuration, ForcedThrottle);
	}
}
