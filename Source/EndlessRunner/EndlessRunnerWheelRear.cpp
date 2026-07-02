// Copyright Epic Games, Inc. All Rights Reserved.

#include "EndlessRunnerWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UEndlessRunnerWheelRear::UEndlessRunnerWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}