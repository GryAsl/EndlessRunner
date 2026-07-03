// Copyright Epic Games, Inc. All Rights Reserved.

#include "SmallCar.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

ASmallCar::ASmallCar()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMesh(TEXT("/Game/CitySampleVehicles/vehicle02_Car/Mesh/SM_vehCar_vehicle02_No_Wheel.SM_vehCar_vehicle02_No_Wheel"));
	if (BodyMesh.Succeeded())
	{
		CarMesh->SetStaticMesh(BodyMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrontLeftWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle02_Car/Mesh/SM_Wheel_Front_L_vehCar_vehicle02.SM_Wheel_Front_L_vehCar_vehicle02"));
	if (FrontLeftWheelMesh.Succeeded())
	{
		WheelFrontLeft->SetStaticMesh(FrontLeftWheelMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrontRightWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle02_Car/Mesh/SM_Wheel_Front_R_vehCar_vehicle02.SM_Wheel_Front_R_vehCar_vehicle02"));
	if (FrontRightWheelMesh.Succeeded())
	{
		WheelFrontRight->SetStaticMesh(FrontRightWheelMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RearLeftWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle02_Car/Mesh/SM_Wheel_Rear_L_vehCar_vehicle02.SM_Wheel_Rear_L_vehCar_vehicle02"));
	if (RearLeftWheelMesh.Succeeded())
	{
		WheelBackLeft->SetStaticMesh(RearLeftWheelMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RearRightWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle02_Car/Mesh/SM_Wheel_Rear_R_vehCar_vehicle02.SM_Wheel_Rear_R_vehCar_vehicle02"));
	if (RearRightWheelMesh.Succeeded())
	{
		WheelBackRight->SetStaticMesh(RearRightWheelMesh.Object);
	}

	SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
	DefaultSpeedKmh = 105.0f;
	DefaultThrottleValue = 0.85f;
}
