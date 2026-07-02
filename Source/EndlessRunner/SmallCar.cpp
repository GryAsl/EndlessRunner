// Copyright Epic Games, Inc. All Rights Reserved.

#include "SmallCar.h"

ASmallCar::ASmallCar()
{
	SetActorScale3D(FVector(0.85f, 0.85f, 0.85f));
	DefaultSpeedKmh = 105.0f;
	DefaultThrottleValue = 0.85f;
}
