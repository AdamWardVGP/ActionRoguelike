// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAction.h"
#include "ActActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void UActAction::Initialize(UActActionComponent* NewActionComp)
{
	OwnerActionComponent = NewActionComp;
}

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

	//ensureAlways(bIsRunning);

	//UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this))
	LogOnScreen(this, FString::Printf(TEXT("Stopping: %s"), *ActionName.ToString()), FColor::White);

	UActActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* UActAction::GetWorld() const
{
	//Outer is set when creating actions via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if(Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

UActActionComponent* UActAction::GetOwningComponent() const
{
	//This is an option but we don't want to iterate through components to find it every time we need this.
	//AActor* Actor = Cast<AActor>(GetOuter());
	//return Cast<UActActionComponent>(Actor->GetComponentByClass(UActActionComponent::StaticClass()));

	return OwnerActionComponent;
}

void UActAction::OnRep_IsRunning()
{
	if(bIsRunning)
	{
		//TODO we'll need to pass around instigator
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
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

void UActAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UActAction, bIsRunning);
	DOREPLIFETIME(UActAction, OwnerActionComponent);
}

bool UActAction::IsSupportedForNetworking() const
{
	return true;
}
