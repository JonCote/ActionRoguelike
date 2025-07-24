// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPlayerController.h"

void AARLPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}
