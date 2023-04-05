// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActPowerupBase.h"
#include "ActCoinPickup.generated.h"


class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AActCoinPickup : public AActPowerupBase
{
	GENERATED_BODY()

public:
	AActCoinPickup();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(BlueprintReadOnly)
	float CoinTotal;
	
};
