// Fill out your copyright notice in the Description page of Project Settings.


#include "ActActionComponent.h"
#include "ActAction.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_ACTSTATS);

UActActionComponent::UActActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


void UActActionComponent::BeginPlay()
{
	Super::BeginPlay();

	//Only let server set out actions
	if(GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UActAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UActActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	for(UActAction* Action : Actions)
	{
		const FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"),
			*GetNameSafe(GetOwner()),
			*GetNameSafe(Action));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void UActActionComponent::AddAction(AActor* Instigator, TSubclassOf<UActAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if(!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddaAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	UActAction* NewAction = NewObject<UActAction>(GetOwner(), ActionClass);
	NewAction->Initialize(this);

	if(ensure(NewAction))
	{
		Actions.Add(NewAction);

		if(NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UActActionComponent::RemoveAction(UActAction* ActActionEffect)
{
	if (!ensure(ActActionEffect && !ActActionEffect->IsRunning()))
	{
		return;
	}

	if (!Actions.Contains(ActActionEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("Action effect %s requested removal but was not present on %s"), *GetNameSafe(ActActionEffect), *GetNameSafe(GetOwner()))
	}

	Actions.Remove(ActActionEffect);
}

bool UActActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

	for(UActAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if(!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			// !GetOwner()->HasAuthority() means caller is a client.
			// Client calls Server RPC. (By convention Server RPC calls start with "Server<MethodName>" prefix
			// Server will then execute the logic in the given Server<MethodName>_Implimentation method
			// Server will still need to provide appropriate messaging back to clients to replicate the desired behavior
			if(!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

			//By starting actions ourselves immediately there's no delay to wait for server to receive
			//And then transmit the action back to us.
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;

}

bool UActActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UActAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName && Action->IsRunning())
		{

			if (!GetOwner()->HasAuthority())
			{
				ServerStopAction(Instigator, ActionName);
			}

			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}

void UActActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UActActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

void UActActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UActActionComponent, Actions);
}

bool UActActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	//When replicating unreal uses UActorChannel which can be thought of as the thread sending data between the server and client

	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for(UActAction* Action : Actions)
	{
		if(Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	//Notifies when changes in components occur so that the data can be replicated
	return WroteSomething;

}

