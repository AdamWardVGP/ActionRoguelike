// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPhysicsProjectile.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AActPhysicsProjectile::AActPhysicsProjectile()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComponent");
	ForceComp->SetupAttachment(RootComponent);
	ForceComp->ForceStrength = 1000.f;
	ForceComp->Radius = 1000.f;

	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void AActPhysicsProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AActPhysicsProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

