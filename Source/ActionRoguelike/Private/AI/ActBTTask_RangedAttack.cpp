// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActBTTask_RangedAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


EBTNodeResult::Type UActBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* AIController = OwnerComp.GetAIOwner();

	if(ensure(AIController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(AIController->GetPawn());
		if(MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		AActor* TargetActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));

		if(TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
		FVector VectorToTarget = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator ProjRotator = VectorToTarget.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = MyPawn;

		AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, ProjRotator, SpawnParams);

		return SpawnedProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
