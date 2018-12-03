// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include <Perception/PawnSensingComponent.h>
#include <DrawDebugHelpers.h>

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AISensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("AI Sensor"));

	AISensor->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnSeePlayer);
	//AISensor->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnHear);

}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSAIGuard::OnSeePlayer(APawn* SeenPawn)
{
	if (!SeenPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnSeePlayer called but SeenPawn is null"))
		return;
	}
	
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 64.0f, 32, FColor::Red, false, 10.0f);
}

void AFPSAIGuard::OnHear(APawn* InstigatorPawn, const FVector& Location, float Volume)
{

}

