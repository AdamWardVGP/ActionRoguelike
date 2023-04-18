// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActPowerupBase.h"
#include "ActHealthPowerup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AActHealthPowerup : public AActPowerupBase
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

public:
	AActHealthPowerup();

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
};
