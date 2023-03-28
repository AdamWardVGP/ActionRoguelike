// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActProjectileBase.h"
#include "ActDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AActDashProjectile : public AActProjectileBase
{
	GENERATED_BODY()

public:

	AActDashProjectile();

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FTimerHandle TimerHandle_Projectile;

	//Base class uses BlueprintNative event, must override the _Implementation rather than Explode()
	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;
};
