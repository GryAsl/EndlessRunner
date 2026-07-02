// Copyright Epic Games, Inc. All Rights Reserved.

#include "PoliceCar.h"

#include "Animation/AnimInstance.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"
#include "UObject/ConstructorHelpers.h"

APoliceCar::APoliceCar()
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
}

void APoliceCar::BeginPlay()
{
	Super::BeginPlay();
}

void APoliceCar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AMyPawn* PlayerPawn = GetTargetPlayer();
	if (!PlayerPawn)
	{
		GetChaosVehicleMovement()->SetThrottleInput(0.0f);
		GetChaosVehicleMovement()->SetSteeringInput(0.0f);
		GetChaosVehicleMovement()->SetBrakeInput(0.0f);
		return;
	}

	const FVector LocalTarget = GetActorTransform().InverseTransformPosition(PlayerPawn->GetActorLocation());
	const float SteeringInput = FMath::Clamp(LocalTarget.Y * SteeringSensitivity, -MaxSteering, MaxSteering);

	GetChaosVehicleMovement()->SetThrottleInput(FMath::Clamp(ChaseThrottle, 0.0f, 1.0f));
	GetChaosVehicleMovement()->SetSteeringInput(SteeringInput);
	GetChaosVehicleMovement()->SetBrakeInput(0.0f);

	ApplyPlayerHitIfClose(PlayerPawn);
}

AMyPawn* APoliceCar::GetTargetPlayer() const
{
	return Cast<AMyPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APoliceCar::ApplyPlayerHitIfClose(AMyPawn* PlayerPawn)
{
	if (bHasHitPlayer || !PlayerPawn)
	{
		return;
	}

	if (FVector::DistSquared(PlayerPawn->GetActorLocation(), GetActorLocation()) > FMath::Square(PlayerHitRadius))
	{
		return;
	}

	bHasHitPlayer = true;
	PlayerPawn->ApplyTrafficHitPenalty(PlayerHitPenaltyDuration, PlayerHitThrottle);
}
