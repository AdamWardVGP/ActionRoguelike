// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActBTTask_Heal.h"

#include "ActAttributeComponent.h"
#include "AIController.h"

EBTNodeResult::Type UActBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

	UActAttributeComponent* Attributes = UActAttributeComponent::GetAttributes(OwnerPawn);
	if(ensure(Attributes))
	{
		Attributes->ApplyHealthChange(OwnerPawn, Attributes->GetMaxHealth());
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
