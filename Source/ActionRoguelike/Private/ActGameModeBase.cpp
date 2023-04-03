// Fill out your copyright notice in the Description page of Project Settings.


#include "ActGameModeBase.h"

#include "ActAttributeComponent.h"
#include "EngineUtils.h"
#include "AI/ActAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "DrawDebugHelpers.h"

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
	//I personally don't like this approach of iterating to find all the bots, we should auto-register them somewhere.
	int32 NumOfAliveBots = 0;
	for (TActorIterator<AActAICharacter> It(GetWorld()); It; ++It)
	{
		AActAICharacter* Bot = *It;
		UActAttributeComponent* BotAttrs = UActAttributeComponent::GetAttributes(Bot);
		if (ensure(BotAttrs) && BotAttrs->IsAlive())
		{
			NumOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."))

	float MaxBotCount = 10.f;

	if(DifficultyCurve)
	{
		//spawn more bots as time increases
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."))
		return;
	}

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
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator, SpawnParams);

		DrawDebugSphere(GetWorld(), Locations[0], 50.f, 20, FColor::Blue, false, 60.0f);
		//UE_LOG(LogTemp, Warning, TEXT("Actor spawned: %p"), SpawnActor);
	}
}
