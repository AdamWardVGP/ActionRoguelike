// Fill out your copyright notice in the Description page of Project Settings.


#include "ActActionComponent.h"
#include "ActAction.h"
#include "ActionRoguelike/ActionRoguelike.h"

UActActionComponent::UActActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


void UActActionComponent::BeginPlay()
{
	Super::BeginPlay();
	for(TSubclassOf<UActAction> ActionClass: DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
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
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			Action->IsRunning() ? TEXT("true") : TEXT("false"),
			*GetNameSafe(GetOuter()));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void UActActionComponent::AddAction(AActor* Instigator, TSubclassOf<UActAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UActAction* NewAction = NewObject<UActAction>(this, ActionClass);
	if(ensure(NewAction))
	{
		Actions.Add(NewAction);

		if(NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

bool UActActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
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

			//If Client call server RPC.
			//Server (authority) will replicate to ther clients
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
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}

void UActActionComponent::RemoveAction(UActAction* ActActionEffect)
{
	if (!ensure(ActActionEffect && !ActActionEffect->IsRunning()))
	{
		return;
	}

	if(!Actions.Contains(ActActionEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("Action effect %s requested removal but was not present on %s"), *GetNameSafe(ActActionEffect), *GetNameSafe(GetOwner()))
	}

	Actions.Remove(ActActionEffect);
}

void UActActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}
