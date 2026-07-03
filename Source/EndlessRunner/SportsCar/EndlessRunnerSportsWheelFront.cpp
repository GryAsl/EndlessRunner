// Copyright Epic Games, Inc. All Rights Reserved.


#include "EndlessRunnerSportsWheelFront.h"

UEndlessRunnerSportsWheelFront::UEndlessRunnerSportsWheelFront()
{
	WheelRadius = 39.0f;
	WheelWidth = 35.0f;
	FrictionForceMultiplier = 3.4f;
	MaxSteerAngle = 70.0f;

	MaxBrakeTorque = 10000.0f;
	MaxHandBrakeTorque = 12000.0f;
}
