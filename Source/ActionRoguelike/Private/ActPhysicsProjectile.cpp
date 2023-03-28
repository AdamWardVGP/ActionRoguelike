// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPhysicsProjectile.h"

#include "ActAttributeComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AActPhysicsProjectile::AActPhysicsProjectile()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComponent");
	ForceComp->SetupAttachment(RootComponent);
	ForceComp->ForceStrength = -1000000.f;
	ForceComp->Radius = 150.f;

	ForceComp->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}