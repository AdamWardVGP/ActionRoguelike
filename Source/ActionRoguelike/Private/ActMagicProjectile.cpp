// Fill out your copyright notice in the Description page of Project Settings.


#include "ActMagicProjectile.h"

#include "ActAttributeComponent.h"

void AActMagicProjectile::Explode_Implementation()
{
	if (APawn* HitObject = GetInstigator())
	{
		UActAttributeComponent* AttributeComponent =
			Cast<UActAttributeComponent>(HitObject->GetComponentByClass(UActAttributeComponent::StaticClass()));

		if (AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(-20.f);

			Destroy();
		}
	}
}
