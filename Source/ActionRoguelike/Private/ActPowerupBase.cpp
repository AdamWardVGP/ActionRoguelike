// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPowerupBase.h"

#include "ActInteractionComponent.h"

AActPowerupBase::AActPowerupBase()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	SetRootComponent(MeshComponent);

	InteractionComponent = CreateDefaultSubobject<UActInteractionComponent>("InteractionComponent");

	RespawnDelay = 5.f;
	CreditCost = 10.f;
}

void AActPowerupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	OnPickup();
}

void AActPowerupBase::OnPickup()
{
	if (ensure(MeshComponent))
	{
		MeshComponent->SetVisibility(false, true);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, this, &AActPowerupBase::OnRespawn, RespawnDelay);
}

void AActPowerupBase::OnRespawn()
{
	if(ensure(MeshComponent))
	{
		MeshComponent->SetVisibility(true, true);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	}
}