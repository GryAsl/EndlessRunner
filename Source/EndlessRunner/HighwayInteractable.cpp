// Copyright Epic Games, Inc. All Rights Reserved.

#include "HighwayInteractable.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"
#include "MyPawn.h"
#include "UObject/ConstructorHelpers.h"

AHighwayInteractable::AHighwayInteractable()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(CollisionSphere);
	CollisionSphere->InitSphereRadius(90.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionSphere->SetGenerateOverlapEvents(true);

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(CollisionSphere);
	VisualMesh->SetCollisionProfileName(FName("NoCollision"));
	VisualMesh->SetRelativeScale3D(FVector(0.75f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (MeshAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MaterialAsset.Succeeded())
	{
		VisualMesh->SetMaterial(0, MaterialAsset.Object);
	}
}

void AHighwayInteractable::BeginPlay()
{
	Super::BeginPlay();

	InitialScale = GetActorScale3D();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AHighwayInteractable::HandleBeginOverlap);
}

void AHighwayInteractable::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AddActorLocalRotation(FRotator(0.0f, RotationSpeedDegrees * DeltaSeconds, 0.0f));

	if (!bCollected)
	{
		if (bUseDistanceFallback)
		{
			if (AMyPawn* PlayerPawn = Cast<AMyPawn>(UGameplayStatics::GetPlayerPawn(this, 0)))
			{
				if (FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation()) <= CollisionSphere->GetScaledSphereRadius())
				{
					CollectFromPawn(PlayerPawn);
				}
			}
		}

		return;
	}

	ShrinkElapsed += DeltaSeconds;
	const float Alpha = ShrinkDuration > 0.0f ? FMath::Clamp(ShrinkElapsed / ShrinkDuration, 0.0f, 1.0f) : 1.0f;
	SetActorScale3D(FMath::Lerp(InitialScale, FVector::ZeroVector, Alpha));

	if (Alpha >= 1.0f)
	{
		Destroy();
	}
}

void AHighwayInteractable::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TryCollectFromActor(OtherActor, OtherComponent);
}

void AHighwayInteractable::TryCollectFromActor(AActor* OtherActor, UPrimitiveComponent* OtherComponent)
{
	AMyPawn* PlayerPawn = Cast<AMyPawn>(OtherActor);
	if (!PlayerPawn && OtherComponent)
	{
		PlayerPawn = Cast<AMyPawn>(OtherComponent->GetOwner());
	}

	if (PlayerPawn)
	{
		CollectFromPawn(PlayerPawn);
	}
}

void AHighwayInteractable::CollectFromPawn(AMyPawn* PlayerPawn)
{
	if (bCollected || !PlayerPawn)
	{
		return;
	}

	OnCollectedByPlayer(PlayerPawn);
	bCollected = true;
	ShrinkElapsed = 0.0f;
	InitialScale = GetActorScale3D();
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHighwayInteractable::OnCollectedByPlayer(AMyPawn* PlayerPawn)
{
}
