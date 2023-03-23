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

	Collider = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Collider->SetCapsuleHalfHeight(20.f);
	Collider->SetCapsuleRadius(15.f);
	MeshComp->SetupAttachment(Collider);

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AActExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
