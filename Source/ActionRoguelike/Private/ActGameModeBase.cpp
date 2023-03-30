// Fill out your copyright notice in the Description page of Project Settings.


#include "ActGameModeBase.h"

#include "EnvironmentQuery/EnvQueryManager.h"

AActGameModeBase::AActGameModeBase()
{
	SpawnTimerInterval = 2.f;
}

void AActGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, 
		&AActGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}


void AActGameModeBase::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInst = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if(ensure(QueryInst))
	{
		QueryInst->GetOnQueryFinishedEvent().AddDynamic(this, &AActGameModeBase::OnSpawnAIQueryCompleted);
	}
}

void AActGameModeBase::OnSpawnAIQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if(Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}
