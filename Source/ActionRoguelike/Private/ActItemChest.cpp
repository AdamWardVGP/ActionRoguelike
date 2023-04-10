// Fill out your copyright notice in the Description page of Project Settings.


#include "ActItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

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

	SetReplicates(true);
}

void AActItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	//Interact is happening on the server, and RepNotify only updates clients
	//Server will need to update itself.
	OnRep_LidOpened();
}

void AActItemChest::OnRep_LidOpened()
{
	const float CurrentPitch = bLidOpened ? TargetPitch : 0.f;
	LidComp->SetRelativeRotation(FRotator(CurrentPitch, 0.f, 0.f));
}

void AActItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Always send to all clients whenever changed on the server.
	DOREPLIFETIME(AActItemChest, bLidOpened);
}
