// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ActPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AActPlayerState*, OwnerPlayerState, float, NewCredits, float, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AActPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AActPlayerState();

protected:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing="OnRep_Credits", Category = "PlayerState")
	float Credits;

	UFUNCTION()
	void OnRep_Credits(float OldCredits);

public:

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	bool ModifyCredits(AActor* ModifySource, float Amount);

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	float GetCreditTotal() const;

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;


	
};
