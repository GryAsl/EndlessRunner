// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "UObject/ConstructorHelpers.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> VehicleMappingAsset(TEXT("/Game/VehicleTemplate/Input/IMC_Vehicle_Default.IMC_Vehicle_Default"));
	if (VehicleMappingAsset.Succeeded())
	{
		VehicleMappingContext = VehicleMappingAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MouseLookMappingAsset(TEXT("/Game/VehicleTemplate/Input/IMC_Vehicle_MouseLook.IMC_Vehicle_MouseLook"));
	if (MouseLookMappingAsset.Succeeded())
	{
		MouseLookMappingContext = MouseLookMappingAsset.Object;
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(false);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (VehicleMappingContext)
		{
			Subsystem->AddMappingContext(VehicleMappingContext, 0);
		}

		if (MouseLookMappingContext)
		{
			Subsystem->AddMappingContext(MouseLookMappingContext, 0);
		}
	}
}
