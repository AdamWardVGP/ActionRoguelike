// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ActPowerupBase.generated.h"

class UActInteractionComponent;
class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API AActPowerupBase : public AActor, public IActGameplayInterface
{
	GENERATED_BODY()
	
public:	
	AActPowerupBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle TimerHandle_RespawnDelay;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UActInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RespawnDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CreditCost;

	UFUNCTION()
	void OnPickup();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnPickup();

	UFUNCTION()
	void OnRespawn();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnRespawn();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
};
