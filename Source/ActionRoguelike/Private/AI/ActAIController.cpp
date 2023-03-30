// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActAIController.h"

#include "Kismet/GameplayStatics.h"

void AActAIController::BeginPlay()
{
	Super::BeginPlay();

	if(ensureMsgf(BehaviorTree, TEXT("Behavior tree is nullptr! please assign BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
