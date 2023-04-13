// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPlayerController.h"

void AActPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}
