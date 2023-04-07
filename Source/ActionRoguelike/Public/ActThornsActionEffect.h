// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActActionEffect.h"
#include "ActThornsActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UActThornsActionEffect : public UActActionEffect
{
	GENERATED_BODY()

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;



public:

	UActThornsActionEffect();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float ThornsDamage;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth, float Delta);
};
