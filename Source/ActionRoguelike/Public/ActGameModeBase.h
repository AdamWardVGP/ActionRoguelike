// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ActGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

UCLASS()
class ACTIONROGUELIKE_API AActGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AActGameModeBase();


	virtual void StartPlay() override;

protected:

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

};
