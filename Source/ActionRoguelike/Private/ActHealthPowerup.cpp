// Fill out your copyright notice in the Description page of Project Settings.


#include "ActHealthPowerup.h"

#include "ActAttributeComponent.h"

void AActHealthPowerup::Interact_Implementation(APawn* InstigatorPawn)
{
	if(InstigatorPawn)
	{
		UActAttributeComponent* InstigatorAttributes = 
			Cast<UActAttributeComponent>(InstigatorPawn->GetComponentByClass(UActAttributeComponent::StaticClass()));

		if(InstigatorAttributes && !InstigatorAttributes->IsAtMaxHealth())
		{
			OnPickup();
			InstigatorAttributes->ApplyHealthChange(100.f);
		}
	}
}
