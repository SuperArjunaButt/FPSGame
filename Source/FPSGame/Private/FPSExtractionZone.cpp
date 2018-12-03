// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include <Components/BoxComponent.h>
#include <Components/DecalComponent.h>
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	ExtractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Comp"));
	ExtractionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ExtractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	ExtractionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ExtractionBox->SetBoxExtent(FVector(200.0f));
	ExtractionBox->SetHiddenInGame(false);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("ExtractionBoxDecal"));
	DecalComp->DecalSize = FVector(200.0f);
	DecalComp->SetupAttachment(RootComponent);

	RootComponent = ExtractionBox;

	ExtractionBox->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* OverlappedCharacter = Cast<AFPSCharacter>(OtherActor);

	if (OverlappedCharacter == nullptr)
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Overlapped with extraction zone"))

	if (OverlappedCharacter->bIsCarryingObjective)
	{
		//notify game mode that we disable input and end mission
		AFPSGameMode* gameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode)
		{
			gameMode->CompleteMission(OverlappedCharacter);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Game mode cast to AFPSGameMode failed!"))
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}

	

}

