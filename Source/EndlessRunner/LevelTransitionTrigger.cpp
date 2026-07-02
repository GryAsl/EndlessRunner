// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelTransitionTrigger.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"

ALevelTransitionTrigger::ALevelTransitionTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);

	TriggerBox->SetBoxExtent(FVector(250.0f, 250.0f, 150.0f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->SetGenerateOverlapEvents(true);
}

void ALevelTransitionTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionTrigger::HandleBeginOverlap);
}

void ALevelTransitionTrigger::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bTriggered || !OtherActor)
	{
		return;
	}

	if (bOnlyPlayerPawn && !Cast<AMyPawn>(OtherActor))
	{
		return;
	}

	if (TargetLevelName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelTransitionTrigger has no TargetLevelName set."));
		return;
	}

	bTriggered = true;

	if (bDisableAfterUse)
	{
		TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	UGameplayStatics::OpenLevel(this, TargetLevelName);
}
