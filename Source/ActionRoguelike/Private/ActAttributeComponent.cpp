// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAttributeComponent.h"

UActAttributeComponent::UActAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100.f;
	Health = MaxHealth;
}

bool UActAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{

	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);

	return ActualDelta != 0;
}

bool UActAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UActAttributeComponent::IsAtMaxHealth() const
{
	return Health == MaxHealth;
}


