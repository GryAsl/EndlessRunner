// Copyright Epic Games, Inc. All Rights Reserved.


#include "EndlessRunnerSportsWheelRear.h"

UEndlessRunnerSportsWheelRear::UEndlessRunnerSportsWheelRear()
{
	WheelRadius = 40.f;
	WheelWidth = 40.0f;
	FrictionForceMultiplier = 4.4f;
	SlipThreshold = 100.0f;
	SkidThreshold = 100.0f;
	MaxSteerAngle = 0.0f;
	MaxBrakeTorque = 10000.0f;
	MaxHandBrakeTorque = 14000.0f;
}
