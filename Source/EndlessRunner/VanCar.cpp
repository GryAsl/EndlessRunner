// Copyright Epic Games, Inc. All Rights Reserved.

#include "VanCar.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AVanCar::AVanCar()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMesh(TEXT("/Game/CitySampleVehicles/vehicle09_Van/Mesh/SM_vehVan_vehicle09_No_Wheel.SM_vehVan_vehicle09_No_Wheel"));
	if (BodyMesh.Succeeded())
	{
		CarMesh->SetStaticMesh(BodyMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrontLeftWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle09_Van/Mesh/SM_Wheel_Front_L_vehVan_vehicle09.SM_Wheel_Front_L_vehVan_vehicle09"));
	if (FrontLeftWheelMesh.Succeeded())
	{
		WheelFrontLeft->SetStaticMesh(FrontLeftWheelMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrontRightWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle09_Van/Mesh/SM_Wheel_Front_R_vehVan_vehicle09.SM_Wheel_Front_R_vehVan_vehicle09"));
	if (FrontRightWheelMesh.Succeeded())
	{
		WheelFrontRight->SetStaticMesh(FrontRightWheelMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RearLeftWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle09_Van/Mesh/SM_Wheel_Rear_L_vehVan_vehicle09.SM_Wheel_Rear_L_vehVan_vehicle09"));
	if (RearLeftWheelMesh.Succeeded())
	{
		WheelBackLeft->SetStaticMesh(RearLeftWheelMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RearRightWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle09_Van/Mesh/SM_Wheel_Rear_R_vehVan_vehicle09.SM_Wheel_Rear_R_vehVan_vehicle09"));
	if (RearRightWheelMesh.Succeeded())
	{
		WheelBackRight->SetStaticMesh(RearRightWheelMesh.Object);
	}

	SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
	DefaultSpeedKmh = 90.0f;
	DefaultThrottleValue = 0.75f;
}
