// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPlayerState.h"

#include "ActSaveGame.h"
#include "Net/UnrealNetwork.h"

AActPlayerState::AActPlayerState()
{

}

bool AActPlayerState::ModifyCredits(AActor* ModifySource, int32 Amount)
{
	if(Amount < 0.f && FMath::Abs(Amount) > Credits)
	{
		//Purchase attempt failed
		return false;
	}

	if(HasAuthority())
	{
		Credits += Amount;

		if (Amount != 0.0f)
		{
			OnCreditsChanged.Broadcast(this, Credits, Amount);
		}
	}

	return true;

}

void AActPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

int32 AActPlayerState::GetCreditTotal() const
{
	return Credits;
}

void AActPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AActPlayerState, Credits);
}

void AActPlayerState::LoadPlayerState_Implementation(UActSaveGame* SaveObject)
{
	if (SaveObject)
	{
		ModifyCredits(nullptr, SaveObject->Credits);
	}
}

void AActPlayerState::SavePlayerState_Implementation(UActSaveGame* SaveObject)
{
	if(SaveObject)
	{
		SaveObject->Credits = Credits;
	}

}