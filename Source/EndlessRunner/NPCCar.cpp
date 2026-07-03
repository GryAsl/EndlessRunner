// Copyright Epic Games, Inc. All Rights Reserved.

#include "NPCCar.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"
#include "UObject/ConstructorHelpers.h"

ANPCCar::ANPCCar()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetBoxExtent(FVector(230.0f, 105.0f, 75.0f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
	CollisionBox->SetNotifyRigidBodyCollision(true);
	CollisionBox->SetGenerateOverlapEvents(true);

	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
	CarMesh->SetupAttachment(CollisionBox);
	CarMesh->SetCollisionProfileName(FName("NoCollision"));

	WheelFrontLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelFrontLeft"));
	WheelFrontLeft->SetupAttachment(CollisionBox);
	WheelFrontLeft->SetCollisionProfileName(FName("NoCollision"));

	WheelFrontRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelFrontRight"));
	WheelFrontRight->SetupAttachment(CollisionBox);
	WheelFrontRight->SetCollisionProfileName(FName("NoCollision"));

	WheelBackLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelBackLeft"));
	WheelBackLeft->SetupAttachment(CollisionBox);
	WheelBackLeft->SetCollisionProfileName(FName("NoCollision"));

	WheelBackRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelBackRight"));
	WheelBackRight->SetupAttachment(CollisionBox);
	WheelBackRight->SetCollisionProfileName(FName("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Vehicles/SportsCar/SM_SportsCar.SM_SportsCar"));
	if (MeshAsset.Succeeded())
	{
		CarMesh->SetStaticMesh(MeshAsset.Object);
	}

	SpeedKmh = DefaultSpeedKmh;
	ThrottleValue = DefaultThrottleValue;
}

void ANPCCar::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ANPCCar::HandleBeginOverlap);
	MovementDirection = MovementDirection.GetSafeNormal();
	if (MovementDirection.IsNearlyZero())
	{
		MovementDirection = GetActorForwardVector();
	}
}

void ANPCCar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AliveSeconds += DeltaSeconds;
	if (AliveSeconds >= LifeSeconds)
	{
		Destroy();
		return;
	}

	if (bPhysicsActor)
	{
		return;
	}

	const float SpeedCmPerSecond = SpeedKmh * 27.777778f * FMath::Clamp(ThrottleValue, 0.0f, 1.0f);
	AddActorWorldOffset(MovementDirection * SpeedCmPerSecond * DeltaSeconds, true);

	if (!bHitPlayer)
	{
		if (AMyPawn* PlayerPawn = Cast<AMyPawn>(UGameplayStatics::GetPlayerPawn(this, 0)))
		{
			if (FVector::DistSquared(PlayerPawn->GetActorLocation(), GetActorLocation()) <= FMath::Square(PlayerHitRadius))
			{
				TryHitPlayer(PlayerPawn, nullptr);
			}
		}
	}
}

void ANPCCar::InitializeNPC(const FVector& InMovementDirection, float InSpeedKmh, float InThrottleValue)
{
	MovementDirection = InMovementDirection.GetSafeNormal();
	SpeedKmh = FMath::Max(0.0f, InSpeedKmh);
	ThrottleValue = FMath::Clamp(InThrottleValue, 0.0f, 1.0f);

	if (!MovementDirection.IsNearlyZero())
	{
		SetActorRotation(MovementDirection.Rotation());
	}
}

void ANPCCar::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TryHitPlayer(OtherActor, OtherComponent);
}

void ANPCCar::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	TryHitPlayer(Other, OtherComp);
}

void ANPCCar::TryHitPlayer(AActor* OtherActor, UPrimitiveComponent* OtherComponent)
{
	if (bHitPlayer)
	{
		return;
	}

	AMyPawn* PlayerPawn = Cast<AMyPawn>(OtherActor);
	if (!PlayerPawn && OtherComponent)
	{
		PlayerPawn = Cast<AMyPawn>(OtherComponent->GetOwner());
	}

	if (!PlayerPawn)
	{
		return;
	}

	bHitPlayer = true;
	PlayerPawn->ApplyTrafficHitPenalty(PlayerHitPenaltyDuration, PlayerHitThrottle);
	BecomePhysicsActor();

	if (bDestroyOnPlayerHit)
	{
		Destroy();
	}
}

void ANPCCar::BecomePhysicsActor()
{
	if (bPhysicsActor)
	{
		return;
	}

	bPhysicsActor = true;
	SpeedKmh = 0.0f;
	ThrottleValue = 0.0f;

	CollisionBox->SetSimulatePhysics(true);
	CollisionBox->SetEnableGravity(true);
	CollisionBox->WakeAllRigidBodies();
}
