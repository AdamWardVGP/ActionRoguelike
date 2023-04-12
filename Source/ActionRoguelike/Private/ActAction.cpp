// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAction.h"
#include "ActActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"

void UActAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this))
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UActActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UActAction::StopAction_Implementation(AActor* Instigator)
{

	ensureAlways(bIsRunning);

	//UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this))
	LogOnScreen(this, FString::Printf(TEXT("Stopping: %s"), *ActionName.ToString()), FColor::White);

	UActActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
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

UActActionComponent* UActAction::GetOwningComponent() const
{
	return Cast<UActActionComponent>(GetOuter());
}

bool UActAction::IsRunning() const
{
	return bIsRunning;
}

bool UActAction::CanStart_Implementation(AActor* Instigator)
{
	if(IsRunning())
	{
		return false;
	}

	UActActionComponent* Comp = GetOwningComponent();
	return !Comp->ActiveGameplayTags.HasAny(BlockedTags);
}
