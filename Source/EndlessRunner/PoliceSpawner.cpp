// Copyright Epic Games, Inc. All Rights Reserved.

#include "PoliceSpawner.h"

#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"
#include "PoliceCar.h"
#include "TimerManager.h"

APoliceSpawner::APoliceSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void APoliceSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (bSpawnOnBeginPlay && GetWorld())
	{
		ScheduleNextSpawn(FMath::Max(0.0f, InitialSpawnDelay));
	}
}

void APoliceSpawner::ScheduleNextSpawn(float Delay)
{
	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &APoliceSpawner::SpawnPolice, FMath::Max(0.0f, Delay), false);
	}
}

void APoliceSpawner::SpawnPolice()
{
	if (!GetWorld())
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * SpawnForwardOffset + FVector::UpVector * SpawnUpOffset;

	if (APoliceCar* SpawnedPolice = GetWorld()->SpawnActor<APoliceCar>(APoliceCar::StaticClass(), SpawnLocation, GetActorRotation(), SpawnParams))
	{
		SpawnedPolice->SetTargetPawn(Cast<AMyPawn>(UGameplayStatics::GetPlayerPawn(this, 0)));
	}

	const float NextDelay = FMath::FRandRange(SpawnIntervalMin, FMath::Max(SpawnIntervalMin, SpawnIntervalMax));
	ScheduleNextSpawn(NextDelay);
}
