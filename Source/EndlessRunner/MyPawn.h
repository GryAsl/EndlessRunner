// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EndlessRunnerSportsCar.h"
#include "MyPawn.generated.h"

class UStaticMeshComponent;
struct FInputActionValue;

/**
 * C++ gameplay-focused player vehicle for Highway Escape.
 */
UCLASS()
class AMyPawn : public AEndlessRunnerSportsCar
{
	GENERATED_BODY()

public:
	AMyPawn();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

	float GetSpeedKmh() const;
	float GetTargetThrottle() const { return CurrentTargetThrottle; }
	float GetEffectiveThrottle() const;
	float GetDistanceTravelledMeters() const { return DistanceTravelledCm * 0.01f; }
	float GetNitroPercent() const;
	bool IsNitroActive() const { return bNitroActive; }

	void RefillNitro(float Amount);
	void FillNitro();
	bool ConsumeNitro(float Amount);
	void CancelNitro();
	void ApplyTrafficHitPenalty(float Duration, float ForcedThrottle);

protected:
	UPROPERTY(EditAnywhere, Category = "Highway|Cruise")
	float MinimumThrottle = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Highway|Cruise")
	float MaximumThrottle = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Highway|Cruise")
	float InitialThrottle = 0.35f;

	UPROPERTY(EditAnywhere, Category = "Highway|Cruise")
	float ThrottleAdjustRate = 0.75f;

	UPROPERTY(EditAnywhere, Category = "Highway|Nitro")
	float NitroMaxCharge = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Highway|Nitro")
	float NitroCharge = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Highway|Nitro")
	float NitroDrainRate = 35.0f;

	UPROPERTY(EditAnywhere, Category = "Highway|Nitro")
	float NitroRechargeRate = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Highway|Nitro")
	float NitroThrottleBonus = 0.35f;

	UPROPERTY(EditAnywhere, Category = "Highway|Nitro")
	float NitroTorqueMultiplier = 1.6f;

	UPROPERTY(EditAnywhere, Category = "Highway|Nitro")
	float NitroMaxRPMMultiplier = 1.2f;

	UPROPERTY(EditAnywhere, Category = "Highway|Camera")
	float NormalCameraFOV = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Highway|Camera")
	float NitroCameraFOV = 105.0f;

	UPROPERTY(EditAnywhere, Category = "Highway|Camera")
	float CameraFOVInterpSpeed = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Highway|Nitro")
	bool bAllowNitroRecharge = true;

	UPROPERTY(EditAnywhere, Category = "Highway|Nitro")
	float MinimumNitroToStart = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Highway|Traffic")
	float DefaultTrafficPenaltyDuration = 1.25f;

	void StartNitro();
	void StopNitro();

	void HandleSteeringInput(const FInputActionValue& Value);
	void HandleHandbrakeStart(const FInputActionValue& Value);
	void HandleHandbrakeStop(const FInputActionValue& Value);
	void HandleLookInput(const FInputActionValue& Value);
	void HandleToggleCameraInput(const FInputActionValue& Value);
	void HandleResetVehicleInput(const FInputActionValue& Value);
	void CruiseThrottleInput(const FInputActionValue& Value);
	void CruiseBrakeInput(const FInputActionValue& Value);
	void StopCruiseAdjust(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere, Category = "Highway|Visuals", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ChassisMain;

	UPROPERTY(VisibleAnywhere, Category = "Highway|Visuals", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ChassisGlass;

	UPROPERTY(VisibleAnywhere, Category = "Highway|Visuals", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> WheelFrontLeft;

	UPROPERTY(VisibleAnywhere, Category = "Highway|Visuals", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> WheelFrontRight;

	UPROPERTY(VisibleAnywhere, Category = "Highway|Visuals", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> WheelBackLeft;

	UPROPERTY(VisibleAnywhere, Category = "Highway|Visuals", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> WheelBackRight;

	bool bNitroRequested = false;
	bool bNitroActive = false;
	float CurrentTargetThrottle = 0.35f;
	float CruiseAdjustInput = 0.0f;
	float BaseMaxTorque = 750.0f;
	float BaseMaxRPM = 7000.0f;
	float TrafficPenaltyRemaining = 0.0f;
	float TrafficPenaltyThrottle = 0.0f;
	FVector LastDistanceSampleLocation = FVector::ZeroVector;
	float DistanceTravelledCm = 0.0f;
};
