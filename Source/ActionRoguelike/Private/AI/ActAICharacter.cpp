// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActAICharacter.h"

#include "ActActionComponent.h"
#include "ActAttributeComponent.h"
#include "ActWorldUserWidget.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/ActAIController.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AActAICharacter::AActAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AttributeComp = CreateDefaultSubobject<UActAttributeComponent>("AttributeComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TimeToHitParamName = "TimeToHit";

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	ActionComp = CreateDefaultSubobject<UActActionComponent>("ActionComponent");
}

void AActAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AActAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AActAICharacter::OnHealthChanged);
}

void AActAICharacter::OnPawnSeen(APawn* Pawn)
{
	setTargetActor(Pawn);
}

void AActAICharacter::setTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void AActAICharacter::OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta < 0)
	{
		if(InstigatorActor != this)
		{
			setTargetActor(InstigatorActor);
		}

		if(ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UActWorldUserWidget>(GetWorld(), HealthbarWidgetClass);
			ActiveHealthBar->AttachedActor = this;
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		//died
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

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			//Set lifespan
			SetLifeSpan(10.0f);
		}
	}
}
