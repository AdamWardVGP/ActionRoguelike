// Fill out your copyright notice in the Description page of Project Settings.


#include "ActInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "ActGameplayInterface.h"
#include "ActWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("act.InteractionDebugDraw"), false, TEXT("Enable debug geometry for interaction components"), ECVF_Cheat);

// Sets default values for this component's properties
UActInteractionComponent::UActInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.f;
	TraceDistance = 500.f;
	CollisionChannel = ECollisionChannel::ECC_WorldDynamic;
}


void UActInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Ensure only if we're on the machine considered the owner of the pawn should we run this block
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if(MyPawn->IsLocallyControlled())
	{
		//Could do this less frequently than every frame with a timer
		FindBestInteractable();
	}
}

void UActInteractionComponent::FindBestInteractable()
{

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotator;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotator);

	const FVector End = EyeLocation + (EyeRotator.Vector() * TraceDistance);

	//Sweep will instead scan across the vector with the given shape
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	TArray<FHitResult> Hits;

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	bool debugEnabled = CVarDebugDrawInteraction.GetValueOnGameThread();

	//Clear ref before trying to fill
	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<UActGameplayInterface>())
			{
				FocusedActor = HitActor;

				if (debugEnabled)
				{
					DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, FColor::Green, false, 5.f, 0, 3.f);
				}

				break;
			}
		}
	}

	if(FocusedActor)
	{
		if(DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UActWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}



	if (debugEnabled)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 5.f, 0, 3.f);
	}
}

void UActInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void UActInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());
	IActGameplayInterface::Execute_Interact(InFocus, MyPawn);
}
