// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActBTService_CheckLowHealth.h"

#include "ActAttributeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UActBTService_CheckLowHealth::UActBTService_CheckLowHealth()
{
	HealthPercentageThreshold = 0.5f;
}

void UActBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	const bool Result = UActAttributeComponent::IsLowHealth(OwnerComp.GetAIOwner()->GetPawn(), HealthPercentageThreshold);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsLowHealthKey.SelectedKeyName, Result);
}
