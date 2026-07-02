// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HighwayInteractable.generated.h"

class AMyPawn;
class USphereComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;

UCLASS()
class AHighwayInteractable : public AActor
{
	GENERATED_BODY()

public:
	AHighwayInteractable();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnCollectedByPlayer(AMyPawn* PlayerPawn);

	UPROPERTY(VisibleAnywhere, Category = "Interactable")
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere, Category = "Interactable")
	TObjectPtr<UStaticMeshComponent> VisualMesh;

	UPROPERTY(EditAnywhere, Category = "Interactable")
	bool bUseDistanceFallback = true;

	UPROPERTY(EditAnywhere, Category = "Interactable")
	float ShrinkDuration = 0.35f;

	UPROPERTY(EditAnywhere, Category = "Interactable")
	float RotationSpeedDegrees = 90.0f;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void TryCollectFromActor(AActor* OtherActor, UPrimitiveComponent* OtherComponent);
	void CollectFromPawn(AMyPawn* PlayerPawn);

	bool bCollected = false;
	float ShrinkElapsed = 0.0f;
	FVector InitialScale = FVector::OneVector;
};
