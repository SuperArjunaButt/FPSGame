// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchPad.h"
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>
#include <Components/StaticMeshComponent.h>
#include <Components/PrimitiveComponent.h>
#include <Engine/StaticMeshActor.h>

// Sets default values
ALaunchPad::ALaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LaunchBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Launch Box"));


	launchCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Launch Platform"));

	launchPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Launch Plane"));


	LaunchBox->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::HandleOverlap);
	SetRootComponent(LaunchBox);
}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("In ALaunchPad::HandleOverlap"))
	FRotator launchRotation = GetActorRotation();
	launchRotation.Pitch += launchPitch;
	FVector launchVelocity = launchRotation.Vector() * launchMagnitude;

	
	
	//spawn emitter
	UGameplayStatics::SpawnEmitterAtLocation(this, launchParticles, GetActorLocation());

	////launch character
	ACharacter* chr = Cast<ACharacter>(OtherActor);
	if (chr)
	{
		chr->LaunchCharacter(launchVelocity, true, true);
	}
	else
	{
		AStaticMeshActor* asma = Cast<AStaticMeshActor>(OtherActor);
		if (asma)
		{
			UPrimitiveComponent* pc = Cast<UPrimitiveComponent>(asma->GetComponentByClass(UPrimitiveComponent::StaticClass()));
			if (pc && pc->IsSimulatingPhysics())
			{
				
				pc->AddImpulse(launchVelocity, NAME_None, true);
			}
		}
	}
}
