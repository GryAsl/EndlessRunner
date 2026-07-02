// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoliceSpawner.generated.h"

class USceneComponent;

UCLASS()
class APoliceSpawner : public AActor
{
	GENERATED_BODY()

public:
	APoliceSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Police", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, Category = "Police")
	bool bSpawnOnBeginPlay = true;

	UPROPERTY(EditAnywhere, Category = "Police")
	float InitialSpawnDelay = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Police")
	float SpawnForwardOffset = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Police")
	float SpawnUpOffset = 80.0f;

private:
	void SpawnPolice();

	FTimerHandle SpawnTimerHandle;
};
