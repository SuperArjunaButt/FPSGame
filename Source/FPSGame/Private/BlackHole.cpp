// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackHole.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Gravitational Pull"));
	AttractionSphere->SetSphereRadius(500);

	DestructionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Destroy On Collide"));
	DestructionSphere->SetSphereRadius(55);

	BlagoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BLAGOLE Mesh"));
	BlagoleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(BlagoleMesh);

	DestructionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::DestroyObj);

}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	//Get all actors with physics enabled


	
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Compute vector from object affected to DA BLAGOLE
	AttractionSphere->GetOverlappingComponents(OverlappingComps);
	for (int32 i = 0; i < OverlappingComps.Num(); i++)
	{
		UPrimitiveComponent* pc = OverlappingComps[i];
		if (pc && pc->IsSimulatingPhysics())
		{
			const float SphereRadius = AttractionSphere->GetScaledSphereRadius();
			const float attractionForce = -2000.0f;		//Negative value so it pulls objects toward the center rather than push away
			//AddForce() in that direction
			pc->AddRadialForce(GetActorLocation(), SphereRadius, attractionForce, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

void ABlackHole::DestroyObj(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}