// Fill out your copyright notice in the Description page of Project Settings.


#include "ActActionEffect.h"

#include "GameFramework/GameStateBase.h"

UActActionEffect::UActActionEffect()
{
	bAutoStart = true;
}

void UActActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if(Duration > 0.0f)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "StopAction", Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, TimerDelegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, TimerDelegate, Period, true);
	}
}

void UActActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{

}

void UActActionEffect::StopAction_Implementation(AActor* Instigator)
{
	//Incase we run into a race condition where duration handle ends action just before period is about to make it's final tick
	//ensure we call it before removal is triggered.
	if(GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect_Implementation(Instigator);
	}

	Super::StopAction_Implementation(Instigator);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UActActionComponent* ActionComponent = GetOwningComponent();
	if(ActionComponent)
	{
		ActionComponent->RemoveAction(this);
	}
}

float UActActionEffect::getTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if(GS)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}

	return Duration;
}