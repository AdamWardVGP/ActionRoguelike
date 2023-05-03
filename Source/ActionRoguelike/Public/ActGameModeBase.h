// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ActGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class UActSaveGame;
class UDataTable;
class UActMonsterData;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		Weight = 1.f;
		SpawnCost = 5.f;
		KillReward = 20.f;
	}


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;

	/* Relative chance to pick this monster */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	/* Points required by gamemode to spawn this unit. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	/* Amount of credits awarded to killer of this unit. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;

};


UCLASS()
class ACTIONROGUELIKE_API AActGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AActGameModeBase();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

protected:

	FString SlotName;

	UPROPERTY()
	UActSaveGame* CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UFUNCTION()
	void OnSpawnAIQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);


	UFUNCTION()
	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);


	UFUNCTION()
	void SpawnBotTimerElapsed();

	FTimerHandle TimerHandle_SpawnBots;

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);


	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	UEnvQuery* SpawnPowerupQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TSubclassOf<AActor> HealthPickup;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TSubclassOf<AActor> CreditPickup;

	UFUNCTION()
	void OnSpawnPowerupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnPickups();

public:

	UFUNCTION(Exec)
	void KillAllBots();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

};
