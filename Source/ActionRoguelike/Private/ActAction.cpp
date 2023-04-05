// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAction.h"

void UActAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this))

}

void UActAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this))
}

UWorld* UActAction::GetWorld() const
{
	//Outer is set when creating actions via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if(Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}
