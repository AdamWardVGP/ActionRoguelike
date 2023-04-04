// Fill out your copyright notice in the Description page of Project Settings.


#include "ActInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "ActGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("act.InteractionDebugDraw"), false, TEXT("Enable debug geometry for interaction components"), ECVF_Cheat);

// Sets default values for this component's properties
UActInteractionComponent::UActInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UActInteractionComponent::PrimaryInteract()
{

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotator;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotator);

	const FVector End = EyeLocation + (EyeRotator.Vector() * 1000.f);

	//This is essentially a raycast, so may be difficult for activating smaller objects
	//FHitResult Hit;
	//GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	float Radius = 30.f;

	//Sweep will instead scan across the vector with the given shape
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	TArray<FHitResult> Hits;

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	bool debugEnabled = CVarDebugDrawInteraction.GetValueOnGameThread();

	for (FHitResult Hit: Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<UActGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				IActGameplayInterface::Execute_Interact(HitActor, MyPawn);

				if (debugEnabled)
				{
					DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, FColor::Green, false, 5.f, 0, 3.f);
				}

				break;
			}
		}
	}

	if(debugEnabled)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 5.f, 0, 3.f);
	}

}