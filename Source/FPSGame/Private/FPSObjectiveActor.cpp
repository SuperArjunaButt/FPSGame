// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjectiveActor.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>
#include "FPSCharacter.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{


	ObjectiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	ObjectiveMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ObjectiveMesh;

	ObjectiveCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	ObjectiveCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ObjectiveCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	ObjectiveCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ObjectiveCollision->SetupAttachment(ObjectiveMesh);

	SetReplicates(true);

}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	//PlayEffects();
}

void AFPSObjectiveActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, pickupParticles, GetActorLocation());
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	PlayEffects();

	if (Role == ROLE_Authority)
	{
		AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
		if (MyCharacter)
		{
			MyCharacter->bIsCarryingObjective = true;
			Destroy();
		}
	}

}

