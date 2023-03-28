// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActProjectileBase.h"
#include "ActPhysicsProjectile.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API AActPhysicsProjectile : public AActProjectileBase
{
	GENERATED_BODY()
	
public:

	AActPhysicsProjectile();

	UPROPERTY(BlueprintReadWrite)
	URadialForceComponent* ForceComp;

	UPROPERTY(BlueprintReadWrite)
	float ProjectileDuration;

};
