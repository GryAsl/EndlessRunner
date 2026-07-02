// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EndlessRunnerSportsCar.h"
#include "PoliceCar.generated.h"

class AMyPawn;
class UStaticMeshComponent;

UCLASS()
class APoliceCar : public AEndlessRunnerSportsCar
{
	GENERATED_BODY()

public:
	APoliceCar();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Police|Visuals", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ChassisMain;

	UPROPERTY(VisibleAnywhere, Category = "Police|Visuals", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ChassisGlass;

	UPROPERTY(VisibleAnywhere, Category = "Police|Visuals", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> WheelFrontLeft;

	UPROPERTY(VisibleAnywhere, Category = "Police|Visuals", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> WheelFrontRight;

	UPROPERTY(VisibleAnywhere, Category = "Police|Visuals", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> WheelBackLeft;

	UPROPERTY(VisibleAnywhere, Category = "Police|Visuals", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> WheelBackRight;

	UPROPERTY(EditAnywhere, Category = "Police|AI")
	float ChaseThrottle = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Police|AI")
	float SteeringSensitivity = 0.004f;

	UPROPERTY(EditAnywhere, Category = "Police|AI")
	float MaxSteering = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Police|Hit")
	float PlayerHitRadius = 320.0f;

	UPROPERTY(EditAnywhere, Category = "Police|Hit")
	float PlayerHitPenaltyDuration = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Police|Hit")
	float PlayerHitThrottle = 0.0f;

private:
	AMyPawn* GetTargetPlayer() const;
	void ApplyPlayerHitIfClose(AMyPawn* PlayerPawn);

	bool bHasHitPlayer = false;
};
