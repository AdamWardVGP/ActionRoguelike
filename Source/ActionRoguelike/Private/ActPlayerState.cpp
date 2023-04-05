// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPlayerState.h"

bool AActPlayerState::ModifyCredits(AActor* ModifySource, float Amount)
{
	if(Amount < 0.f && Amount > Credits)
	{
		//Purchase attempt failed
		return false;
	}

	Credits += Amount;

	OnCreditsChanged.Broadcast(ModifySource, this, Credits, Amount);

	return true;

}

float AActPlayerState::GetCreditTotal() const
{
	return Credits;
}
