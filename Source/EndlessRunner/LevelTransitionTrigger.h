// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransitionTrigger.generated.h"

class UBoxComponent;

UCLASS()
class ALevelTransitionTrigger : public AActor
{
	GENERATED_BODY()

public:
	ALevelTransitionTrigger();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Transition")
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(EditAnywhere, Category = "Transition")
	FName TargetLevelName = TEXT("L_Highway_Main");

	UPROPERTY(EditAnywhere, Category = "Transition")
	bool bOnlyPlayerPawn = true;

	UPROPERTY(EditAnywhere, Category = "Transition")
	bool bDisableAfterUse = true;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bTriggered = false;
};
