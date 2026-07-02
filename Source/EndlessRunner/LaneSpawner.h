// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaneSpawner.generated.h"

class AHighwayInteractable;
class ANPCCar;

UCLASS()
class ALaneSpawner : public AActor
{
	GENERATED_BODY()

public:
	ALaneSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane")
	bool bUseReverseDirection = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane")
	float SpawnForwardOffset = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane")
	float SpawnUpOffset = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane|Cars")
	TArray<TSubclassOf<ANPCCar>> CarClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane|Cars")
	float CarSpawnIntervalMin = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane|Cars")
	float CarSpawnIntervalMax = 3.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane|Cars")
	float CarSpeedKmhMin = 70.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane|Cars")
	float CarSpeedKmhMax = 130.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane|Cars")
	float CarThrottleMin = 0.55f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane|Cars")
	float CarThrottleMax = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane|Interactables")
	TArray<TSubclassOf<AHighwayInteractable>> InteractableClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane|Interactables")
	float InteractableSpawnIntervalMin = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane|Interactables")
	float InteractableSpawnIntervalMax = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane|Safety")
	float SpawnClearRadius = 450.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane|Safety")
	bool bSkipInteractableIfCarNearby = true;

protected:
	virtual void BeginPlay() override;

private:
	void ScheduleNextCarSpawn();
	void ScheduleNextInteractableSpawn();
	void SpawnCar();
	void SpawnInteractable();

	FVector GetSpawnLocation() const;
	FVector GetMovementDirection() const;
	bool IsSpawnAreaClearForClass(TSubclassOf<AActor> ActorClass, float Radius) const;

	FTimerHandle CarSpawnTimerHandle;
	FTimerHandle InteractableSpawnTimerHandle;
};
