// Fill out your copyright notice in the Description page of Project Settings.


#include "ActTargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "ActAttributeComponent.h"

AActTargetDummy::AActTargetDummy()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);

	AttributeComponent = CreateDefaultSubobject<UActAttributeComponent>("AttributeComponent");
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AActTargetDummy::OnHealthChanged);
}

void AActTargetDummy::OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta < 0.0f)
	{
		MeshComponent->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}
