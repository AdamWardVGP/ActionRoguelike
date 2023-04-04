// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAttributeComponent.h"

#include "ActGameModeBase.h"

UActAttributeComponent::UActAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100.f;
	Health = MaxHealth;
}

UActAttributeComponent* UActAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<UActAttributeComponent>(FromActor->GetComponentByClass(UActAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UActAttributeComponent::IsActorAlive(AActor* FromActor)
{
	if(UActAttributeComponent* ActAttributeComponent = GetAttributes(FromActor))
	{
		return ActAttributeComponent->IsAlive();
	}

	return false;
}

bool UActAttributeComponent::IsLowHealth(AActor* FromActor, float ThresholdPercent)
{
	if (UActAttributeComponent* ActAttributeComponent = GetAttributes(FromActor))
	{
		return (ActAttributeComponent->Health / ActAttributeComponent->MaxHealth) <= ThresholdPercent;
	}

	return false;
}

bool UActAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{

	if(!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);

	if(ActualDelta <0.0f && Health == 0.0f)
	{
		AActGameModeBase* GM = GetWorld()->GetAuthGameMode<AActGameModeBase>();
		if(GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return ActualDelta != 0;
}

float UActAttributeComponent::GetMaxHealth()
{
	return MaxHealth;
}

bool UActAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UActAttributeComponent::IsAtMaxHealth() const
{
	return Health == MaxHealth;
}

bool UActAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -1 * GetMaxHealth());
}


