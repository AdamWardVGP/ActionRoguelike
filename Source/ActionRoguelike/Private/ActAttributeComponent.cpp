// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAttributeComponent.h"

UActAttributeComponent::UActAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100.f;
	Health = MaxHealth;
}

bool UActAttributeComponent::ApplyHealthChange(float Delta)
{
	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

bool UActAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

