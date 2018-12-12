// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include <Perception/PawnSensingComponent.h>
#include <DrawDebugHelpers.h>
#include <Engine/TargetPoint.h>
#include "FPSGameMode.h"
#include <Kismet/KismetSystemLibrary.h>
#include <AIController.h>
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <UnrealNetwork.h>

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AISensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("AI Sensor"));

	AISensor->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnSeePlayer);
	GuardState = EAIState::Idle;
	currentTargetPoint = 0;
}



// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	ResetOrientation();
	AISensor->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnHear);
	if(TargetPoints.Num() > 0) UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), TargetPoints[currentTargetPoint]);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GuardState == EAIState::Idle && TargetPoints.Num() > 0)
	{
		//ATargetPoint* currentTarget = TargetPoints[currentTargetPoint];
		//AAIController* aic = Cast<AAIController>(GetController());
		//aic->MoveTo(currentTarget);

		FVector dist = GetActorLocation() - TargetPoints[currentTargetPoint]->GetActorLocation();
		if (dist.Size() < patrolTolerance)
		{
			MoveToNextPatrolPoint();
		}
		UE_LOG(LogTemp, Warning, TEXT("%f"), dist.Size())
	}
	
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	currentTargetPoint++;
	currentTargetPoint = currentTargetPoint % TargetPoints.Num();
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), TargetPoints[currentTargetPoint]);
}

void AFPSAIGuard::SetGuardState(EAIState GState)
{
	if (GuardState == GState)
	{
		return;
	}
	else
	{
		GuardState = GState;
		OnRep_GuardState();

	}
}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}




void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
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
		SetGuardState(EAIState::Alerted);
		gameMode->CompleteMission(SeenPawn, false);
	}
	//DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 64.0f, 32, FColor::Red, false, 10.0f);
}

void AFPSAIGuard::OnHear(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

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
	SetGuardState(EAIState::Suspicious);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f, false);
	DrawDebugSphere(GetWorld(), Location, 64.0f, 32, FColor::Blue, false, 10.0f);


}

void AFPSAIGuard::ResetOrientation()
{
	SetActorRotation(originalRotation);
	
	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	SetGuardState(EAIState::Idle);
}
