// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActAICharacter.h"

#include "ActAttributeComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/ActAIController.h"
#include "BrainComponent.h"

AActAICharacter::AActAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AttributeComp = CreateDefaultSubobject<UActAttributeComponent>("AttributeComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void AActAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AActAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AActAICharacter::OnHealthChanged);
}

void AActAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		UBlackboardComponent* BlackboardComp = AIC->GetBlackboardComponent();
		BlackboardComp->SetValueAsObject("TargetActor", Pawn);

		//DrawDebugString(GetWorld(), GetActorLocation(), "Player Spotted", nullptr, FColor::White, 4.f, true);
	}
}

void AActAICharacter::OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta < 0)
	{
		if(NewHealth <= 0.0f)
		{
			//Stop BT
			AActAIController* AIC = Cast<AActAIController>(GetController());
			if(AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Entity Killed");
			}

			//Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			//Set lifespan
			SetLifeSpan(10.0f);
		}
	}
}
