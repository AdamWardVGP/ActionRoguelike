// Fill out your copyright notice in the Description page of Project Settings.


#include "ActItemChest.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AActItemChest::AActItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ContainerComp = CreateDefaultSubobject<UStaticMeshComponent>("ContainerMesh");
	SetRootComponent(ContainerComp);

	LidComp = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidComp->SetupAttachment(ContainerComp);

	TargetPitch = 110.f;
}

void AActItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidComp->SetRelativeRotation(FRotator(TargetPitch, 0.f, 0.f));
}

