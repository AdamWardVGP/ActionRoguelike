// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAttributeComponent.h"

UActAttributeComponent::UActAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100.f;
}

bool UActAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

bool UActAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

