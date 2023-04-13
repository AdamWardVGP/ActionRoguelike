// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ActPlayerState.generated.h"

class UActSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AActPlayerState*, OwnerPlayerState, int32, NewCredits, int32, Delta);

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
	int32 Credits;

	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

public:

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	bool ModifyCredits(AActor* ModifySource, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	int32 GetCreditTotal() const;

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UActSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(UActSaveGame* SaveObject);
	
};
