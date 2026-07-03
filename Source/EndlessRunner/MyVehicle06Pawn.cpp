// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyVehicle06Pawn.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AMyVehicle06Pawn::AMyVehicle06Pawn()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMesh(TEXT("/Game/CitySampleVehicles/vehicle06_Car/Mesh/SM_vehCar_vehicle06_No_Wheel.SM_vehCar_vehicle06_No_Wheel"));
	if (BodyMesh.Succeeded())
	{
		GetChassisMainVisual()->SetStaticMesh(BodyMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> GlassMesh(TEXT("/Game/CitySampleVehicles/vehicle06_Car/Mesh/SM_All_Trans_vehCar_vehicle06.SM_All_Trans_vehCar_vehicle06"));
	if (GlassMesh.Succeeded())
	{
		GetChassisGlassVisual()->SetStaticMesh(GlassMesh.Object);
		GetChassisGlassVisual()->SetVisibility(true);
	}

	UStaticMeshComponent* FrontLeftWheel = GetWheelFrontLeftVisual();
	UStaticMeshComponent* FrontRightWheel = GetWheelFrontRightVisual();
	UStaticMeshComponent* BackLeftWheel = GetWheelBackLeftVisual();
	UStaticMeshComponent* BackRightWheel = GetWheelBackRightVisual();

	FrontLeftWheel->SetupAttachment(GetMesh());
	FrontLeftWheel->SetRelativeLocation(FVector::ZeroVector);
	FrontLeftWheel->SetRelativeRotation(FRotator::ZeroRotator);

	FrontRightWheel->SetupAttachment(GetMesh());
	FrontRightWheel->SetRelativeLocation(FVector::ZeroVector);
	FrontRightWheel->SetRelativeRotation(FRotator::ZeroRotator);

	BackLeftWheel->SetupAttachment(GetMesh());
	BackLeftWheel->SetRelativeLocation(FVector::ZeroVector);
	BackLeftWheel->SetRelativeRotation(FRotator::ZeroRotator);

	BackRightWheel->SetupAttachment(GetMesh());
	BackRightWheel->SetRelativeLocation(FVector::ZeroVector);
	BackRightWheel->SetRelativeRotation(FRotator::ZeroRotator);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrontLeftWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle06_Car/Mesh/SM_Wheel_Front_L_vehCar_vehicle06.SM_Wheel_Front_L_vehCar_vehicle06"));
	if (FrontLeftWheelMesh.Succeeded())
	{
		FrontLeftWheel->SetStaticMesh(FrontLeftWheelMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrontRightWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle06_Car/Mesh/SM_Wheel_Front_R_vehCar_vehicle06.SM_Wheel_Front_R_vehCar_vehicle06"));
	if (FrontRightWheelMesh.Succeeded())
	{
		FrontRightWheel->SetStaticMesh(FrontRightWheelMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RearLeftWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle06_Car/Mesh/SM_Wheel_Rear_L_vehCar_vehicle06.SM_Wheel_Rear_L_vehCar_vehicle06"));
	if (RearLeftWheelMesh.Succeeded())
	{
		BackLeftWheel->SetStaticMesh(RearLeftWheelMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RearRightWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle06_Car/Mesh/SM_Wheel_Rear_R_vehCar_vehicle06.SM_Wheel_Rear_R_vehCar_vehicle06"));
	if (RearRightWheelMesh.Succeeded())
	{
		BackRightWheel->SetStaticMesh(RearRightWheelMesh.Object);
	}
}
