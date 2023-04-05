// Fill out your copyright notice in the Description page of Project Settings.


#include "ActMagicProj.h"
#include "Components/SphereComponent.h"
#include "ActAttributeComponent.h"
#include "ActGameplayFunctionLibrary.h"


AActMagicProj::AActMagicProj()
{
	ProjectileDamage = 20.f;
}

void AActMagicProj::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// More consistent to bind here compared to Constructor which may fail to bind if Blueprint was created before adding this binding (or when using hotreload)
	// PostInitializeComponent is the preferred way of binding any events.
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AActMagicProj::OnActorOverlap);
}

void AActMagicProj::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, Instigator: %s"), *GetNameSafe(OtherActor), *GetNameSafe(GetInstigator()));
		if(UActGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, ProjectileDamage, SweepResult))
		{
			UE_LOG(LogTemp, Warning, TEXT("Calling explode"));
			Explode();
		}
	}
}
