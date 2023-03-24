// Fill out your copyright notice in the Description page of Project Settings.


#include "ActExplosiveBarrel.h"

#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AActExplosiveBarrel::AActExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	SetRootComponent(MeshComp);

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

	ForceComp->Radius = 500;
	ForceComp->ImpulseStrength = 500;
	ForceComp->ForceStrength = 500;
	ForceComp->bImpulseVelChange = true;

	// Optional, default constructor of component already adds 4 object types to affect, excluding WorldDynamic
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void AActExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//OnComponentHit is a struct
	MeshComp->OnComponentHit.AddDynamic(this, &AActExplosiveBarrel::OnActorHit);
}

void AActExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();
}

