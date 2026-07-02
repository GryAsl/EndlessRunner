// Copyright Epic Games, Inc. All Rights Reserved.

#include "NitroPickup.h"

#include "MyPawn.h"

ANitroPickup::ANitroPickup()
{
}

void ANitroPickup::OnCollectedByPlayer(AMyPawn* PlayerPawn)
{
	if (PlayerPawn)
	{
		PlayerPawn->FillNitro();
	}
}
