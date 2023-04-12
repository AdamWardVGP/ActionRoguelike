// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPlayerState.h"

#include "Net/UnrealNetwork.h"

AActPlayerState::AActPlayerState()
{

}

bool AActPlayerState::ModifyCredits(AActor* ModifySource, float Amount)
{
	if(Amount < 0.f && FMath::Abs(Amount) > Credits)
	{
		//Purchase attempt failed
		return false;
	}

	Credits += Amount;


	if (Amount != 0.0f)
	{
		OnCreditsChanged.Broadcast(this, Credits, Amount);
	}

	return true;

}

//void AActPlayerState::MulticastCreditsChanged_Implementation(AActor* InstigatorActor, float NewCredits, float Delta)
//{
//	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
//}

void AActPlayerState::OnRep_Credits(float OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

float AActPlayerState::GetCreditTotal() const
{
	return Credits;
}

void AActPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AActPlayerState, Credits);
}
