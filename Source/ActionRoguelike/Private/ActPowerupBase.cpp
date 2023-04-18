// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPowerupBase.h"

#include "ActInteractionComponent.h"
#include "Components/SphereComponent.h"

AActPowerupBase::AActPowerupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	RootComponent = SphereComp;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);

	InteractionComponent = CreateDefaultSubobject<UActInteractionComponent>("InteractionComponent");

	RespawnDelay = 10.f;
	CreditCost = 10.f;

	//if server spawns the actor then it will tell the client to spawn that actor too
	SetReplicates(true);
}

void AActPowerupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	OnPickup();
}

FText AActPowerupBase::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void AActPowerupBase::OnPickup()
{
	MulticastOnPickup();

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, this, &AActPowerupBase::OnRespawn, RespawnDelay);
}

void AActPowerupBase::MulticastOnPickup_Implementation()
{
	if (ensure(MeshComponent))
	{
		MeshComponent->SetVisibility(false, true);
		SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AActPowerupBase::OnRespawn()
{
	MulticastOnRespawn();
}

void AActPowerupBase::MulticastOnRespawn_Implementation()
{
	if (ensure(MeshComponent))
	{
		MeshComponent->SetVisibility(true, true);
		SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	}
}
