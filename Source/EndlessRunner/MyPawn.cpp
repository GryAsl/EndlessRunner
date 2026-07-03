// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyPawn.h"

#include "Animation/AnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputCoreTypes.h"
#include "Engine/Engine.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	ChassisMain = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis Main"));
	ChassisMain->SetupAttachment(GetMesh());
	ChassisMain->SetCollisionProfileName(FName("NoCollision"));

	ChassisGlass = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis Glass"));
	ChassisGlass->SetupAttachment(GetMesh());
	ChassisGlass->SetCollisionProfileName(FName("NoCollision"));

	WheelFrontLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel Front Left"));
	WheelFrontLeft->SetupAttachment(GetMesh(), FName("Phys_Wheel_FL"));
	WheelFrontLeft->SetCollisionProfileName(FName("NoCollision"));
	WheelFrontLeft->SetRelativeLocation(FVector(5.0f, 0.0f, 0.0f));
	WheelFrontLeft->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));

	WheelFrontRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel Front Right"));
	WheelFrontRight->SetupAttachment(GetMesh(), FName("Phys_Wheel_FR"));
	WheelFrontRight->SetCollisionProfileName(FName("NoCollision"));
	WheelFrontRight->SetRelativeLocation(FVector(-5.0f, 0.0f, 0.0f));
	WheelFrontRight->SetRelativeRotation(FRotator(90.0f, -90.0f, 0.0f));

	WheelBackLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel Back Left"));
	WheelBackLeft->SetupAttachment(GetMesh(), FName("Phys_Wheel_BL"));
	WheelBackLeft->SetCollisionProfileName(FName("NoCollision"));
	WheelBackLeft->SetRelativeLocation(FVector(5.0f, 0.0f, 0.0f));
	WheelBackLeft->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));

	WheelBackRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel Back Right"));
	WheelBackRight->SetupAttachment(GetMesh(), FName("Phys_Wheel_BR"));
	WheelBackRight->SetCollisionProfileName(FName("NoCollision"));
	WheelBackRight->SetRelativeLocation(FVector(-5.0f, 0.0f, 0.0f));
	WheelBackRight->SetRelativeRotation(FRotator(90.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SportsCarMesh(TEXT("/Game/Vehicles/SportsCar/SKM_SportsCar.SKM_SportsCar"));
	if (SportsCarMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SportsCarMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ChassisMesh(TEXT("/Game/Vehicles/SportsCar/SM_SportsCar.SM_SportsCar"));
	if (ChassisMesh.Succeeded())
	{
		ChassisMain->SetStaticMesh(ChassisMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> GlassMesh(TEXT("/Game/Vehicles/SportsCar/SM_SportsCar_Glass.SM_SportsCar_Glass"));
	if (GlassMesh.Succeeded())
	{
		ChassisGlass->SetStaticMesh(GlassMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WheelMesh(TEXT("/Game/Vehicles/SportsCar/SM_SportsCar_Wheel.SM_SportsCar_Wheel"));
	if (WheelMesh.Succeeded())
	{
		WheelFrontLeft->SetStaticMesh(WheelMesh.Object);
		WheelFrontRight->SetStaticMesh(WheelMesh.Object);
		WheelBackLeft->SetStaticMesh(WheelMesh.Object);
		WheelBackRight->SetStaticMesh(WheelMesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> SportsCarAnimClass(TEXT("/Game/Vehicles/SportsCar/ABP_SportsCar"));
	if (SportsCarAnimClass.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(SportsCarAnimClass.Class);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> TorqueCurveAsset(TEXT("/Game/VehicleTemplate/Blueprints/SportsCar/FC_SportsCar_Torque.FC_SportsCar_Torque"));
	if (TorqueCurveAsset.Succeeded())
	{
		GetChaosVehicleMovement()->EngineSetup.TorqueCurve.ExternalCurve = TorqueCurveAsset.Object;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> CarUIWidgetClass(TEXT("/Game/Blueprints/Widgets/CarUI"));
	if (CarUIWidgetClass.Succeeded())
	{
		CarUIClass = CarUIWidgetClass.Class;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SteeringActionAsset(TEXT("/Game/VehicleTemplate/Input/Actions/IA_Steering.IA_Steering"));
	static ConstructorHelpers::FObjectFinder<UInputAction> ThrottleActionAsset(TEXT("/Game/VehicleTemplate/Input/Actions/IA_Throttle.IA_Throttle"));
	static ConstructorHelpers::FObjectFinder<UInputAction> BrakeActionAsset(TEXT("/Game/VehicleTemplate/Input/Actions/IA_Brake.IA_Brake"));
	static ConstructorHelpers::FObjectFinder<UInputAction> HandbrakeActionAsset(TEXT("/Game/VehicleTemplate/Input/Actions/IA_Handbrake.IA_Handbrake"));
	static ConstructorHelpers::FObjectFinder<UInputAction> LookAroundActionAsset(TEXT("/Game/VehicleTemplate/Input/Actions/IA_LookAround.IA_LookAround"));
	static ConstructorHelpers::FObjectFinder<UInputAction> ToggleCameraActionAsset(TEXT("/Game/VehicleTemplate/Input/Actions/IA_ToggleCamera.IA_ToggleCamera"));
	static ConstructorHelpers::FObjectFinder<UInputAction> ResetVehicleActionAsset(TEXT("/Game/VehicleTemplate/Input/Actions/IA_Reset.IA_Reset"));

	if (SteeringActionAsset.Succeeded())
	{
		SteeringAction = SteeringActionAsset.Object;
	}
	if (ThrottleActionAsset.Succeeded())
	{
		ThrottleAction = ThrottleActionAsset.Object;
	}
	if (BrakeActionAsset.Succeeded())
	{
		BrakeAction = BrakeActionAsset.Object;
	}
	if (HandbrakeActionAsset.Succeeded())
	{
		HandbrakeAction = HandbrakeActionAsset.Object;
	}
	if (LookAroundActionAsset.Succeeded())
	{
		LookAroundAction = LookAroundActionAsset.Object;
	}
	if (ToggleCameraActionAsset.Succeeded())
	{
		ToggleCameraAction = ToggleCameraActionAsset.Object;
	}
	if (ResetVehicleActionAsset.Succeeded())
	{
		ResetVehicleAction = ResetVehicleActionAsset.Object;
	}

	CurrentTargetThrottle = InitialThrottle;
	BaseMaxTorque = GetChaosVehicleMovement()->EngineSetup.MaxTorque;
	BaseMaxRPM = GetChaosVehicleMovement()->EngineSetup.MaxRPM;
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	LastDistanceSampleLocation = GetActorLocation();
	DistanceTravelledCm = 0.0f;
	CreateCarUI();
	ApplyDrivingInputMode();
}

void AMyPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CarUIInstance)
	{
		CarUIInstance->RemoveFromParent();
		CarUIInstance = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void AMyPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CreateCarUI();
	ApplyDrivingInputMode();
}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AMyPawn::HandleSteeringInput);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &AMyPawn::HandleSteeringInput);

		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AMyPawn::CruiseThrottleInput);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AMyPawn::StopCruiseAdjust);

		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AMyPawn::CruiseBrakeInput);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &AMyPawn::StopCruiseAdjust);

		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &AMyPawn::HandleHandbrakeStart);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &AMyPawn::HandleHandbrakeStop);

		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &AMyPawn::HandleLookInput);
		EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Triggered, this, &AMyPawn::HandleToggleCameraInput);
		EnhancedInputComponent->BindAction(ResetVehicleAction, ETriggerEvent::Triggered, this, &AMyPawn::HandleResetVehicleInput);
	}

	PlayerInputComponent->BindKey(EKeys::LeftShift, IE_Pressed, this, &AMyPawn::StartNitro);
	PlayerInputComponent->BindKey(EKeys::LeftShift, IE_Released, this, &AMyPawn::StopNitro);
	PlayerInputComponent->BindKey(EKeys::RightShift, IE_Pressed, this, &AMyPawn::StartNitro);
	PlayerInputComponent->BindKey(EKeys::RightShift, IE_Released, this, &AMyPawn::StopNitro);
}

void AMyPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector CurrentLocation = GetActorLocation();
	if (!LastDistanceSampleLocation.IsZero())
	{
		DistanceTravelledCm += FVector::Dist2D(CurrentLocation, LastDistanceSampleLocation);
	}
	LastDistanceSampleLocation = CurrentLocation;

	const bool bCanRunNitro = bNitroRequested && NitroCharge > 0.0f;
	bNitroActive = bCanRunNitro;

	if (TrafficPenaltyRemaining > 0.0f)
	{
		TrafficPenaltyRemaining = FMath::Max(0.0f, TrafficPenaltyRemaining - DeltaSeconds);
		CurrentTargetThrottle = FMath::Min(CurrentTargetThrottle, MinimumThrottle);
		CancelNitro();
	}
	else
	{
		const float NitroCruiseAdjust = bNitroActive ? 1.0f : 0.0f;
		const float CombinedCruiseAdjust = CruiseAdjustInput < 0.0f ? CruiseAdjustInput : FMath::Max(CruiseAdjustInput, NitroCruiseAdjust);
		const float AdjustRate = ThrottleAdjustRate * (CombinedCruiseAdjust < 0.0f ? BrakeThrottleAdjustMultiplier : 1.0f);
		CurrentTargetThrottle = FMath::Clamp(CurrentTargetThrottle + CombinedCruiseAdjust * AdjustRate * DeltaSeconds, MinimumThrottle, MaximumThrottle);
	}

	const float EffectiveThrottle = TrafficPenaltyRemaining > 0.0f ? TrafficPenaltyThrottle : GetEffectiveThrottle();
	const float BrakeInput = FMath::Max(CruiseAdjustInput < 0.0f ? CruiseBrakeInputStrength : 0.0f, bHandbrakeHeld ? HandbrakeBrakeInputStrength : 0.0f);
	GetChaosVehicleMovement()->SetThrottleInput(FMath::Clamp(EffectiveThrottle, 0.0f, 1.0f));
	GetChaosVehicleMovement()->SetBrakeInput(BrakeInput);
	GetChaosVehicleMovement()->EngineSetup.MaxTorque = bNitroActive ? BaseMaxTorque * NitroTorqueMultiplier : BaseMaxTorque;
	GetChaosVehicleMovement()->EngineSetup.MaxRPM = bNitroActive ? BaseMaxRPM * NitroMaxRPMMultiplier : BaseMaxRPM;

	if (bNitroActive)
	{
		ConsumeNitro(NitroDrainRate * DeltaSeconds);
		if (NitroCharge <= 0.0f)
		{
			bNitroActive = false;
			bNitroRequested = false;
		}
	}
	else if (bAllowNitroRecharge)
	{
		RefillNitro(NitroRechargeRate * DeltaSeconds);
	}

	const float TargetFOV = bNitroActive ? NitroCameraFOV : NormalCameraFOV;
	if (UCameraComponent* FrontCameraComponent = GetFollowCamera())
	{
		FrontCameraComponent->SetFieldOfView(FMath::FInterpTo(FrontCameraComponent->FieldOfView, TargetFOV, DeltaSeconds, CameraFOVInterpSpeed));
	}
	if (UCameraComponent* BackCameraComponent = GetBackCamera())
	{
		BackCameraComponent->SetFieldOfView(FMath::FInterpTo(BackCameraComponent->FieldOfView, TargetFOV, DeltaSeconds, CameraFOVInterpSpeed));
	}
}

float AMyPawn::GetSpeedKmh() const
{
	return FMath::Abs(GetChaosVehicleMovement()->GetForwardSpeed()) * 0.036f;
}

float AMyPawn::GetEffectiveThrottle() const
{
	return bNitroActive ? FMath::Min(1.0f, CurrentTargetThrottle + NitroThrottleBonus) : FMath::Min(MaximumThrottle, CurrentTargetThrottle);
}

float AMyPawn::GetNitroPercent() const
{
	return NitroMaxCharge > 0.0f ? FMath::Clamp(NitroCharge / NitroMaxCharge, 0.0f, 1.0f) : 0.0f;
}

float AMyPawn::GetNitroCharge() const
{
	return NitroCharge;
}

float AMyPawn::GetNitroMaxCharge() const
{
	return NitroMaxCharge;
}

bool AMyPawn::IsNitroActive() const
{
	return bNitroActive;
}

int32 AMyPawn::GetHealth() const
{
	return Health;
}

float AMyPawn::GetHealthPercent() const
{
	return MaxHealth > 0 ? FMath::Clamp(static_cast<float>(Health) / static_cast<float>(MaxHealth), 0.0f, 1.0f) : 0.0f;
}

void AMyPawn::CreateCarUI()
{
	if (CarUIInstance || !CarUIClass)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		APlayerController* LocalPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (LocalPlayerController && LocalPlayerController->GetPawn() == this)
		{
			PlayerController = LocalPlayerController;
		}
	}

	if (!PlayerController)
	{
		return;
	}

	CarUIInstance = CreateWidget<UUserWidget>(PlayerController, CarUIClass);
	if (CarUIInstance)
	{
		CarUIInstance->AddToViewport();
	}
}

void AMyPawn::ApplyDrivingInputMode()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		APlayerController* LocalPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (LocalPlayerController && LocalPlayerController->GetPawn() == this)
		{
			PlayerController = LocalPlayerController;
		}
	}

	if (!PlayerController)
	{
		return;
	}

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(false);
	PlayerController->ResetIgnoreMoveInput();
	PlayerController->ResetIgnoreLookInput();
}

void AMyPawn::RefillNitro(float Amount)
{
	NitroCharge = FMath::Clamp(NitroCharge + FMath::Max(0.0f, Amount), 0.0f, NitroMaxCharge);
}

void AMyPawn::FillNitro()
{
	NitroCharge = NitroMaxCharge;
}

void AMyPawn::CancelNitro()
{
	bNitroRequested = false;
	bNitroActive = false;
}

void AMyPawn::ApplyTrafficHitPenalty(float Duration, float ForcedThrottle)
{
	TrafficPenaltyRemaining = FMath::Max(TrafficPenaltyRemaining, Duration > 0.0f ? Duration : DefaultTrafficPenaltyDuration);
	TrafficPenaltyThrottle = FMath::Clamp(ForcedThrottle, 0.0f, 1.0f);
	CurrentTargetThrottle = FMath::Min(CurrentTargetThrottle, MinimumThrottle);
	CancelNitro();
}

void AMyPawn::ApplyDamage(int32 DamageAmount)
{
	const int32 PreviousHealth = Health;
	Health = FMath::Clamp(Health - FMath::Max(0, DamageAmount), 0, MaxHealth);

	if (GEngine && PreviousHealth != Health)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, FString::Printf(TEXT("Health: %d"), Health));
	}
}

bool AMyPawn::ConsumeNitro(float Amount)
{
	if (NitroCharge <= 0.0f)
	{
		NitroCharge = 0.0f;
		return false;
	}

	NitroCharge = FMath::Clamp(NitroCharge - FMath::Max(0.0f, Amount), 0.0f, NitroMaxCharge);
	return true;
}

void AMyPawn::StartNitro()
{
	if (NitroCharge >= MinimumNitroToStart)
	{
		bNitroRequested = true;
	}
}

void AMyPawn::StopNitro()
{
	CancelNitro();
}

void AMyPawn::HandleSteeringInput(const FInputActionValue& Value)
{
	DoSteering(Value.Get<float>());
}

void AMyPawn::HandleHandbrakeStart(const FInputActionValue& Value)
{
	bHandbrakeHeld = true;
	DoHandbrakeStart();
}

void AMyPawn::HandleHandbrakeStop(const FInputActionValue& Value)
{
	bHandbrakeHeld = false;
	DoHandbrakeStop();
}

void AMyPawn::HandleLookInput(const FInputActionValue& Value)
{
	DoLookAround(Value.Get<float>());
}

void AMyPawn::HandleToggleCameraInput(const FInputActionValue& Value)
{
	DoToggleCamera();
}

void AMyPawn::HandleResetVehicleInput(const FInputActionValue& Value)
{
	DoResetVehicle();
}

void AMyPawn::CruiseThrottleInput(const FInputActionValue& Value)
{
	CruiseAdjustInput = FMath::Max(0.0f, Value.GetMagnitude());
}

void AMyPawn::CruiseBrakeInput(const FInputActionValue& Value)
{
	CruiseAdjustInput = -FMath::Max(0.0f, Value.GetMagnitude());
}

void AMyPawn::StopCruiseAdjust(const FInputActionValue& Value)
{
	CruiseAdjustInput = 0.0f;
}
