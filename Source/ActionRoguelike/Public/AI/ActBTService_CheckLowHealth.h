// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ActBTService_CheckLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UActBTService_CheckLowHealth : public UBTService
{
	GENERATED_BODY()

public:
	UActBTService_CheckLowHealth();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float HealthPercentageThreshold;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector IsLowHealthKey;
};
