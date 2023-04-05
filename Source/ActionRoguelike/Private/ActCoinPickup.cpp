// Fill out your copyright notice in the Description page of Project Settings.


#include "ActCoinPickup.h"

#include "ActPlayerState.h"

AActCoinPickup::AActCoinPickup()
{
	CoinTotal = 10;
}

void AActCoinPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	AActPlayerState* PlayerState = Cast<AActPlayerState>(InstigatorPawn->GetPlayerState());
	if(PlayerState)
	{
		PlayerState->ModifyCredits(this, CoinTotal);
	}
}
