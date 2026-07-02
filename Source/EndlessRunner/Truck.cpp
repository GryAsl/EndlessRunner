// Copyright Epic Games, Inc. All Rights Reserved.

#include "Truck.h"

ATruck::ATruck()
{
	SetActorScale3D(FVector(1.25f, 1.15f, 1.2f));
	DefaultSpeedKmh = 75.0f;
	DefaultThrottleValue = 0.65f;
}
