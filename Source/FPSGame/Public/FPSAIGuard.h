// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Navigation/PathFollowingComponent.h>
#include <AITypes.h>
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;
class ATargetPoint;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Sensing)
	UPawnSensingComponent* AISensor;

	UFUNCTION()
	void OnSeePlayer(APawn* SeenPawn);

	UFUNCTION()
	void OnHear(APawn* InstigatorPawn, const FVector& Location, float Volume);

	UPROPERTY(EditAnywhere, Category = AI)
	float patrolTolerance = 50.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = AI)
	TArray<ATargetPoint*> TargetPoints;

	UFUNCTION()
	void SetGuardState(EAIState GState);

	UFUNCTION(BlueprintImplementableEvent, Category = AI)
	void OnStateChanged(EAIState newState);

	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EAIState GuardState;

	UFUNCTION()
	void OnRep_GuardState();

private:
	
	FRotator originalRotation;

	FTimerHandle TimerHandle_ResetOrientation;

	UFUNCTION()
	void ResetOrientation();



	int currentTargetPoint;

	void MoveToNextPatrolPoint();

};
