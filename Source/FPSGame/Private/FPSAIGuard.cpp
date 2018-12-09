// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include <Perception/PawnSensingComponent.h>
#include <DrawDebugHelpers.h>
#include "FPSGameMode.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AISensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("AI Sensor"));

	AISensor->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnSeePlayer);

}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	ResetOrientation();
	AISensor->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnHear);

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
	AFPSGameMode* gameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode)
	{
		gameMode->CompleteMission(SeenPawn, false);
	}
	//DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 64.0f, 32, FColor::Red, false, 10.0f);
}

void AFPSAIGuard::OnHear(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
	
	if (!InstigatorPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnHear called but InstigatorPawn is null"))
		return;
	}

	FVector dirVector = Location - GetActorLocation();
	dirVector.Normalize();

	FRotator rotation = FRotationMatrix::MakeFromX(dirVector).Rotator();
	rotation.Pitch = 0.0f;
	rotation.Roll = 0.0f;

	SetActorRotation(rotation);

	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f, false);
	DrawDebugSphere(GetWorld(), Location, 64.0f, 32, FColor::Blue, false, 10.0f);
}

void AFPSAIGuard::ResetOrientation()
{
	SetActorRotation(originalRotation);
}
