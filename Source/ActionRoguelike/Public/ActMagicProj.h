// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActProjectileBase.h"
#include "GameplayTagContainer.h"
#include "ActMagicProj.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AActMagicProj : public AActProjectileBase
{
	GENERATED_BODY()

public:
	AActMagicProj();

protected:
	virtual void PostInitializeComponents() override;
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float ProjectileDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;
};
