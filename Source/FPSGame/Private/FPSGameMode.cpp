// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include <Camera/PlayerCameraManager.h>
#include <Kismet/GameplayStatics.h>

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn)
{
	//disable input and end mission, tally up and score or similar state with the instigating actor

	if (InstigatorPawn)
	{
		if (SpectatingViewpointClass)
		{
			InstigatorPawn->DisableInput(nullptr);
			OnMissionCompleted(InstigatorPawn);

			APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
			//Change viewpoint if any valid actor found
			if (PC)
			{

				TArray<AActor*> ReturnedActors;
				UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);
				if (ReturnedActors.Num() > 0)
				{
					AActor* MissionCompleteViewpoint = ReturnedActors[0];
					PC->SetViewTargetWithBlend(MissionCompleteViewpoint, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spectating Viewpoint Actor not found! Please check game mode class and add valid subclass.  Cannot change spectating view target"))
		}
	}
}