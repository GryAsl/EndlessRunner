// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCCar.generated.h"

class AMyPawn;
class UBoxComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;

UCLASS()
class ANPCCar : public AActor
{
	GENERATED_BODY()

public:
	ANPCCar();

	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	void InitializeNPC(const FVector& InMovementDirection, float InSpeedKmh, float InThrottleValue);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "NPC Car")
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = "NPC Car")
	TObjectPtr<UStaticMeshComponent> CarMesh;

	UPROPERTY(VisibleAnywhere, Category = "NPC Car")
	TObjectPtr<UStaticMeshComponent> WheelFrontLeft;

	UPROPERTY(VisibleAnywhere, Category = "NPC Car")
	TObjectPtr<UStaticMeshComponent> WheelFrontRight;

	UPROPERTY(VisibleAnywhere, Category = "NPC Car")
	TObjectPtr<UStaticMeshComponent> WheelBackLeft;

	UPROPERTY(VisibleAnywhere, Category = "NPC Car")
	TObjectPtr<UStaticMeshComponent> WheelBackRight;

	UPROPERTY(EditAnywhere, Category = "NPC Car")
	float DefaultSpeedKmh = 90.0f;

	UPROPERTY(EditAnywhere, Category = "NPC Car")
	float DefaultThrottleValue = 0.75f;

	UPROPERTY(EditAnywhere, Category = "NPC Car")
	float LifeSeconds = 36.0f;

	UPROPERTY(EditAnywhere, Category = "NPC Car")
	float PlayerHitRadius = 260.0f;

	UPROPERTY(EditAnywhere, Category = "NPC Car|Penalty")
	float PlayerHitPenaltyDuration = 1.25f;

	UPROPERTY(EditAnywhere, Category = "NPC Car|Penalty")
	float PlayerHitThrottle = 0.0f;

	UPROPERTY(EditAnywhere, Category = "NPC Car|Penalty")
	bool bDestroyOnPlayerHit = false;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void TryHitPlayer(AActor* OtherActor, UPrimitiveComponent* OtherComponent);
	void BecomePhysicsActor();

	FVector MovementDirection = FVector::ForwardVector;
	float SpeedKmh = 90.0f;
	float ThrottleValue = 0.75f;
	float AliveSeconds = 0.0f;
	bool bHitPlayer = false;
	bool bPhysicsActor = false;
};
