// Copyright Epic Games, Inc. All Rights Reserved.

#include "LaneSpawner.h"

#include "Engine/World.h"
#include "HighwayInteractable.h"
#include "Kismet/GameplayStatics.h"
#include "NitroPickup.h"
#include "NPCCar.h"
#include "OilTrap.h"
#include "SmallCar.h"
#include "TimerManager.h"
#include "Truck.h"
#include "VanCar.h"

ALaneSpawner::ALaneSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	CarClasses.Add(ASmallCar::StaticClass());
	CarClasses.Add(AVanCar::StaticClass());
	CarClasses.Add(ATruck::StaticClass());
	InteractableClasses.Add(ANitroPickup::StaticClass());
	InteractableClasses.Add(AOilTrap::StaticClass());
}

void ALaneSpawner::BeginPlay()
{
	Super::BeginPlay();

	ScheduleNextCarSpawn();
	ScheduleNextInteractableSpawn();
}

void ALaneSpawner::ScheduleNextCarSpawn()
{
	if (!GetWorld())
	{
		return;
	}

	const float Delay = FMath::FRandRange(CarSpawnIntervalMin, FMath::Max(CarSpawnIntervalMin, CarSpawnIntervalMax));
	GetWorldTimerManager().SetTimer(CarSpawnTimerHandle, this, &ALaneSpawner::SpawnCar, Delay, false);
}

void ALaneSpawner::ScheduleNextInteractableSpawn()
{
	if (!GetWorld())
	{
		return;
	}

	const float Delay = FMath::FRandRange(InteractableSpawnIntervalMin, FMath::Max(InteractableSpawnIntervalMin, InteractableSpawnIntervalMax));
	GetWorldTimerManager().SetTimer(InteractableSpawnTimerHandle, this, &ALaneSpawner::SpawnInteractable, Delay, false);
}

void ALaneSpawner::SpawnCar()
{
	if (GetWorld() && CarClasses.Num() > 0 && IsSpawnAreaClearForClass(ANPCCar::StaticClass(), SpawnClearRadius))
	{
		const int32 ClassIndex = FMath::RandRange(0, CarClasses.Num() - 1);
		TSubclassOf<ANPCCar> ChosenClass = CarClasses[ClassIndex];

		if (ChosenClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			const FVector Direction = GetMovementDirection();
			const float SpeedKmh = FMath::FRandRange(CarSpeedKmhMin, FMath::Max(CarSpeedKmhMin, CarSpeedKmhMax));
			const float Throttle = FMath::FRandRange(CarThrottleMin, FMath::Max(CarThrottleMin, CarThrottleMax));

			if (ANPCCar* SpawnedCar = GetWorld()->SpawnActor<ANPCCar>(ChosenClass, GetSpawnLocation(), Direction.Rotation(), SpawnParams))
			{
				SpawnedCar->InitializeNPC(Direction, SpeedKmh, Throttle);
			}
		}
	}

	ScheduleNextCarSpawn();
}

void ALaneSpawner::SpawnInteractable()
{
	const bool bCarNearby = bSkipInteractableIfCarNearby && !IsSpawnAreaClearForClass(ANPCCar::StaticClass(), SpawnClearRadius);
	if (GetWorld() && !bCarNearby && InteractableClasses.Num() > 0 && IsSpawnAreaClearForClass(AHighwayInteractable::StaticClass(), SpawnClearRadius * 0.65f))
	{
		const int32 ClassIndex = FMath::RandRange(0, InteractableClasses.Num() - 1);
		TSubclassOf<AHighwayInteractable> ChosenClass = InteractableClasses[ClassIndex];

		if (ChosenClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<AHighwayInteractable>(ChosenClass, GetSpawnLocation(), GetActorRotation(), SpawnParams);
		}
	}

	ScheduleNextInteractableSpawn();
}

FVector ALaneSpawner::GetSpawnLocation() const
{
	return GetActorLocation() + GetMovementDirection() * SpawnForwardOffset + FVector::UpVector * SpawnUpOffset;
}

FVector ALaneSpawner::GetMovementDirection() const
{
	const FVector Direction = bUseReverseDirection ? -GetActorForwardVector() : GetActorForwardVector();
	return Direction.GetSafeNormal();
}

bool ALaneSpawner::IsSpawnAreaClearForClass(TSubclassOf<AActor> ActorClass, float Radius) const
{
	if (!GetWorld() || !ActorClass)
	{
		return true;
	}

	TArray<AActor*> ExistingActors;
	UGameplayStatics::GetAllActorsOfClass(this, ActorClass, ExistingActors);

	const FVector SpawnLocation = GetSpawnLocation();
	for (AActor* ExistingActor : ExistingActors)
	{
		if (IsValid(ExistingActor) && FVector::DistSquared(ExistingActor->GetActorLocation(), SpawnLocation) <= FMath::Square(Radius))
		{
			return false;
		}
	}

	return true;
}
