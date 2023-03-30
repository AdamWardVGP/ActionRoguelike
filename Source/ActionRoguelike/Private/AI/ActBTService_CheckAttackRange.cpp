// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UActBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//Find a distance between AI pawn and target actor + line of sight checks

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComponent))
	{
		//TODO Move "targetactor" to a const value
		AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
		if(TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if(ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if(ensure(AIPawn))
				{
					float Distance = FVector::Dist(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					bool bWithinRange = Distance < 1500.f;
					bool bHasLineOfSight = false;

					if(bWithinRange)
					{
						bHasLineOfSight = MyController->LineOfSightTo(TargetActor);
					}
					BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, bHasLineOfSight);
				}
			}
		}
	}

}
