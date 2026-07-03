// Copyright Epic Games, Inc. All Rights Reserved.

#include "PoliceCar.h"

#include "Animation/AnimInstance.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/Engine.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"
#include "UObject/ConstructorHelpers.h"

APoliceCar::APoliceCar()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	ChassisMain = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis Main"));
	ChassisMain->SetupAttachment(GetMesh());
	ChassisMain->SetCollisionProfileName(FName("NoCollision"));

	ChassisGlass = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis Glass"));
	ChassisGlass->SetupAttachment(GetMesh());
	ChassisGlass->SetCollisionProfileName(FName("NoCollision"));

	HitTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Hit Trigger"));
	HitTrigger->SetupAttachment(GetMesh());
	HitTrigger->SetSphereRadius(PlayerHitRadius);
	HitTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitTrigger->SetCollisionObjectType(ECC_WorldDynamic);
	HitTrigger->SetCollisionResponseToAllChannels(ECR_Overlap);
	HitTrigger->SetGenerateOverlapEvents(true);

	WheelFrontLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel Front Left"));
	WheelFrontLeft->SetupAttachment(GetMesh());
	WheelFrontLeft->SetCollisionProfileName(FName("NoCollision"));
	WheelFrontLeft->SetRelativeLocation(FVector::ZeroVector);
	WheelFrontLeft->SetRelativeRotation(FRotator::ZeroRotator);

	WheelFrontRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel Front Right"));
	WheelFrontRight->SetupAttachment(GetMesh());
	WheelFrontRight->SetCollisionProfileName(FName("NoCollision"));
	WheelFrontRight->SetRelativeLocation(FVector::ZeroVector);
	WheelFrontRight->SetRelativeRotation(FRotator::ZeroRotator);

	WheelBackLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel Back Left"));
	WheelBackLeft->SetupAttachment(GetMesh());
	WheelBackLeft->SetCollisionProfileName(FName("NoCollision"));
	WheelBackLeft->SetRelativeLocation(FVector::ZeroVector);
	WheelBackLeft->SetRelativeRotation(FRotator::ZeroRotator);

	WheelBackRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel Back Right"));
	WheelBackRight->SetupAttachment(GetMesh());
	WheelBackRight->SetCollisionProfileName(FName("NoCollision"));
	WheelBackRight->SetRelativeLocation(FVector::ZeroVector);
	WheelBackRight->SetRelativeRotation(FRotator::ZeroRotator);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SportsCarMesh(TEXT("/Game/Vehicles/SportsCar/SKM_SportsCar.SKM_SportsCar"));
	if (SportsCarMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SportsCarMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ChassisMesh(TEXT("/Game/CitySampleVehicles/vehicle13_Car/Mesh/SM_vehCar_vehicle13_No_Wheel.SM_vehCar_vehicle13_No_Wheel"));
	if (ChassisMesh.Succeeded())
	{
		ChassisMain->SetStaticMesh(ChassisMesh.Object);
	}

	ChassisGlass->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrontLeftWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle13_Car/Mesh/SM_Wheel_Front_L_vehCar_vehicle13.SM_Wheel_Front_L_vehCar_vehicle13"));
	if (FrontLeftWheelMesh.Succeeded())
	{
		WheelFrontLeft->SetStaticMesh(FrontLeftWheelMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrontRightWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle13_Car/Mesh/SM_Wheel_Front_R_vehCar_vehicle13.SM_Wheel_Front_R_vehCar_vehicle13"));
	if (FrontRightWheelMesh.Succeeded())
	{
		WheelFrontRight->SetStaticMesh(FrontRightWheelMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RearLeftWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle13_Car/Mesh/SM_Wheel_Rear_L_vehCar_vehicle13.SM_Wheel_Rear_L_vehCar_vehicle13"));
	if (RearLeftWheelMesh.Succeeded())
	{
		WheelBackLeft->SetStaticMesh(RearLeftWheelMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RearRightWheelMesh(TEXT("/Game/CitySampleVehicles/vehicle13_Car/Mesh/SM_Wheel_Rear_R_vehCar_vehicle13.SM_Wheel_Rear_R_vehCar_vehicle13"));
	if (RearRightWheelMesh.Succeeded())
	{
		WheelBackRight->SetStaticMesh(RearRightWheelMesh.Object);
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

	GetChaosVehicleMovement()->EngineSetup.MaxTorque *= EngineTorqueMultiplier;
	GetChaosVehicleMovement()->EngineSetup.MaxRPM *= MaxRPMMultiplier;
	GetChaosVehicleMovement()->TransmissionSetup.FinalRatio *= FinalRatioMultiplier;

	GetMesh()->SetNotifyRigidBodyCollision(true);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetChaosVehicleMovement()->SetRequiresControllerForInputs(false);
}

void APoliceCar::BeginPlay()
{
	Super::BeginPlay();

	if (!GetController())
	{
		SpawnDefaultController();
	}

	GetChaosVehicleMovement()->SetRequiresControllerForInputs(false);
	GetChaosVehicleMovement()->Activate(true);
	GetMesh()->WakeAllRigidBodies();

	if (HitTrigger)
	{
		HitTrigger->SetSphereRadius(PlayerHitRadius);
		HitTrigger->OnComponentBeginOverlap.AddDynamic(this, &APoliceCar::HandleHitTriggerOverlap);
	}
}

void APoliceCar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bHasHitPlayer)
	{
		GetChaosVehicleMovement()->SetThrottleInput(0.0f);
		GetChaosVehicleMovement()->SetSteeringInput(0.0f);
		GetChaosVehicleMovement()->SetBrakeInput(1.0f);
		return;
	}

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

	if (FVector::DistSquared2D(PlayerPawn->GetActorLocation(), GetActorLocation()) <= FMath::Square(PlayerHitRadius))
	{
		ApplyPlayerHit(PlayerPawn);
	}
}

AMyPawn* APoliceCar::GetTargetPlayer() const
{
	if (TargetPawn.IsValid())
	{
		return TargetPawn.Get();
	}

	return Cast<AMyPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APoliceCar::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	TryApplyPlayerHit(Other, OtherComp);
}

void APoliceCar::SetTargetPawn(AMyPawn* NewTargetPawn)
{
	TargetPawn = NewTargetPawn;
}

void APoliceCar::TryApplyPlayerHit(AActor* OtherActor, UPrimitiveComponent* OtherComponent)
{
	AMyPawn* PlayerPawn = Cast<AMyPawn>(OtherActor);
	if (!PlayerPawn && OtherComponent)
	{
		PlayerPawn = Cast<AMyPawn>(OtherComponent->GetOwner());
	}

	ApplyPlayerHit(PlayerPawn);
}

void APoliceCar::HandleHitTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TryApplyPlayerHit(OtherActor, OtherComponent);
}

void APoliceCar::ApplyPlayerHit(AMyPawn* PlayerPawn)
{
	if (bHasHitPlayer || !PlayerPawn)
	{
		return;
	}

	bHasHitPlayer = true;
	PlayerPawn->ApplyDamage(PlayerHitDamage);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("Police hit player"));
	}
}
