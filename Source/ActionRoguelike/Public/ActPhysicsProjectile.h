// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActMagicProjectile.h"
#include "GameFramework/Actor.h"
#include "ActPhysicsProjectile.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API AActPhysicsProjectile : public AActMagicProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActPhysicsProjectile();

	UPROPERTY(BlueprintReadWrite)
	URadialForceComponent* ForceComp;

	UPROPERTY(BlueprintReadWrite)
	float ProjectileDuration;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
