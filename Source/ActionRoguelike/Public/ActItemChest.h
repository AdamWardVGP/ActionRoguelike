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

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ContainerComp;

	UPROPERTY(VisibleAnywhere)
	float TargetPitch;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};