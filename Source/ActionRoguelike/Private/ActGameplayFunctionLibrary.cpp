// Fill out your copyright notice in the Description page of Project Settings.


#include "ActGameplayFunctionLibrary.h"

#include "ActAttributeComponent.h"


bool UActGameplayFunctionLibrary::ApplyDamage(AActor* DamageSource, AActor* TargetActor, float DamageAmount)
{
	if (UActAttributeComponent* ActAttributeComponent = UActAttributeComponent::GetAttributes(TargetActor))
	{
		return ActAttributeComponent->ApplyHealthChange(DamageSource, -1 * DamageAmount);
	}
	return false;
}

bool UActGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageSource, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if(ApplyDamage(DamageSource, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if(HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName))
		{

			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComponent->AddImpulseAtLocation(Direction * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}

	return false;
}
