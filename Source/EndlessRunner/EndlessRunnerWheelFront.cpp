// Copyright Epic Games, Inc. All Rights Reserved.

#include "EndlessRunnerWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UEndlessRunnerWheelFront::UEndlessRunnerWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}