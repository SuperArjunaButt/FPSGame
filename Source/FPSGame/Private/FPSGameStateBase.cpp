// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameStateBase.h"
#include <Engine/World.h>
#include "FPSPlayerController.h"


void AFPSGameStateBase::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	for (FConstPlayerControllerIterator pcitr = GetWorld()->GetPlayerControllerIterator(); pcitr; pcitr++)
	{
		AFPSPlayerController* pc = Cast<AFPSPlayerController>(pcitr->Get());
		if (pc && pc->IsLocalController())
		{
			pc->OnMissionCompleted(InstigatorPawn, bMissionSuccess);
			
			//Disable input
			APawn* MyPawn = pc->GetPawn(); 
			if (MyPawn)
			{
				MyPawn->DisableInput(nullptr);
			}
		}
	}
}
