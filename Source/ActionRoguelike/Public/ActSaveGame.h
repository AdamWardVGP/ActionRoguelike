// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ActSaveGame.generated.h"

USTRUCT()
struct FSavedTransform
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString ActorName;

	UPROPERTY()
	FTransform Transform;
};

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UActSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FSavedTransform> SavedActorPositions;
};
