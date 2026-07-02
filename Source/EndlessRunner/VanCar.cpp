// Copyright Epic Games, Inc. All Rights Reserved.

#include "VanCar.h"

AVanCar::AVanCar()
{
	SetActorScale3D(FVector(1.05f, 1.05f, 1.05f));
	DefaultSpeedKmh = 90.0f;
	DefaultThrottleValue = 0.75f;
}
