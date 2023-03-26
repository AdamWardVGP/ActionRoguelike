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

	return true;
}

