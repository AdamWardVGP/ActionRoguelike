// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActMonsterData.generated.h"

class UActAction;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UActMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Params")
	TSubclassOf<AActor> MonsterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Params")
	TArray<TSubclassOf<UActAction>> Actions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Params")
	UTexture2D* Icon;


	
};
