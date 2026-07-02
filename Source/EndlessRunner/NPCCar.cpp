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
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionBox->SetGenerateOverlapEvents(true);

	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
	CarMesh->SetupAttachment(CollisionBox);
	CarMesh->SetCollisionProfileName(FName("NoCollision"));

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

	if (bDestroyOnPlayerHit)
	{
		Destroy();
	}
}
