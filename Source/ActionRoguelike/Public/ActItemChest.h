// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ActItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API AActItemChest : public AActor, public IActGameplayInterface 
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
public:	
	// Sets default values for this actor's properties
	AActItemChest();


protected:

	//Replicated - changes the property on other clients but doesn't tr
	//ReplicatedUsing - allows us to listen to property changes ("RepNotify" in blueprint)
	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened")
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ContainerComp;

	UPROPERTY(VisibleAnywhere)
	float TargetPitch;

};
