// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAction.h"
#include "ActActionComponent.h"
#include "Net/UnrealNetwork.h"

void UActAction::Initialize(UActActionComponent* NewActionComp)
{
	OwnerActionComponent = NewActionComp;
}

void UActAction::StartAction_Implementation(AActor* Instigator)
{
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UActActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	TimeStarted = GetWorld()->TimeSeconds;

	Comp->OnActionStarted.Broadcast(Comp, this);
}

void UActAction::StopAction_Implementation(AActor* Instigator)
{
	//LogOnScreen(this, FString::Printf(TEXT("Stopping: %s"), *ActionName.ToString()), FColor::White);

	UActActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	Comp->OnActionStopped.Broadcast(Comp, this);
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
	return OwnerActionComponent;
}

void UActAction::OnRep_RepData()
{
	if(RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool UActAction::IsRunning() const
{
	return RepData.bIsRunning;
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

	DOREPLIFETIME(UActAction, RepData);
	DOREPLIFETIME(UActAction, OwnerActionComponent);
}

bool UActAction::IsSupportedForNetworking() const
{
	return true;
}
