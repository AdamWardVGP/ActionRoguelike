// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActAction.h"
#include "ActAction_ProjectileAttack.generated.h"

class UAnimMontage;
class CastingEffect;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UActAction_ProjectileAttack : public UActAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	/* Particle system played during attack animation */
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatingCharacter);
	void LaunchProjectileTowardCrosshair(TSubclassOf<AActor> Projectile);

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

	UActAction_ProjectileAttack();
};
