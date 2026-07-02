// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoliceSpawner.generated.h"

class APoliceCar;

UCLASS()
class APoliceSpawner : public AActor
{
	GENERATED_BODY()

public:
	APoliceSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Police")
	TSubclassOf<APoliceCar> PoliceClass;

	UPROPERTY(EditAnywhere, Category = "Police")
	bool bSpawnOnBeginPlay = true;

	UPROPERTY(EditAnywhere, Category = "Police")
	float InitialSpawnDelay = 2.0f;

private:
	void SpawnPolice();

	FTimerHandle SpawnTimerHandle;
};
