// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActProjectileBase.h"
#include "ActMagicProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AActMagicProjectile : public AActProjectileBase
{
	GENERATED_BODY()

	//Base class uses BlueprintNative event, must override the _Implementation rather than Explode()
	virtual void Explode_Implementation() override;
};
