// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActBTTask_RangedAttack.h"

#include "ActAttributeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


UActBTTask_RangedAttack::UActBTTask_RangedAttack()
{
	MaxBulletSpread = 2.0f;
}

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

		if(!UActAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
		FVector VectorToTarget = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator ProjRotator = VectorToTarget.Rotation();

		ProjRotator.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
		ProjRotator.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = MyPawn;

		AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, ProjRotator, SpawnParams);

		return SpawnedProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
