// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActMagicProjectile.h"
#include "ActDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AActDashProjectile : public AActMagicProjectile
{
	GENERATED_BODY()

public:

	AActDashProjectile();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FTimerHandle TimerHandle_Projectile;

	void DurationElapsed();

};
