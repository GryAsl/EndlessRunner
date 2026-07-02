// Copyright Epic Games, Inc. All Rights Reserved.

#include "PoliceSpawner.h"

#include "Engine/World.h"
#include "PoliceCar.h"
#include "TimerManager.h"

APoliceSpawner::APoliceSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	PoliceClass = APoliceCar::StaticClass();
}

void APoliceSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (bSpawnOnBeginPlay && GetWorld())
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &APoliceSpawner::SpawnPolice, FMath::Max(0.0f, InitialSpawnDelay), false);
	}
}

void APoliceSpawner::SpawnPolice()
{
	if (!GetWorld() || !PoliceClass)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<APoliceCar>(PoliceClass, GetActorLocation(), GetActorRotation(), SpawnParams);
}
