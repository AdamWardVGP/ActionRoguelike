// Fill out your copyright notice in the Description page of Project Settings.


#include "ActHealthPowerup.h"

#include "ActAttributeComponent.h"
#include "ActPlayerState.h"


AActHealthPowerup::AActHealthPowerup()
{
	CreditCost = 50.f;
}

void AActHealthPowerup::Interact_Implementation(APawn* InstigatorPawn)
{
	if(InstigatorPawn)
	{
		UActAttributeComponent* InstigatorAttributes = 
			Cast<UActAttributeComponent>(InstigatorPawn->GetComponentByClass(UActAttributeComponent::StaticClass()));

		AActPlayerState* PlayerState = Cast<AActPlayerState>(InstigatorPawn->GetPlayerState());

		//Modify must be called last as it actually spends the credits, but only if possible
		if(InstigatorAttributes && !InstigatorAttributes->IsAtMaxHealth() && PlayerState->ModifyCredits(this, -1 * CreditCost))
		{
			OnPickup();
			InstigatorAttributes->ApplyHealthChange(this, 100.f);
		}
	}
}
