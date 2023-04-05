// Fill out your copyright notice in the Description page of Project Settings.


#include "ActGameModeBase.h"

#include "ActAttributeComponent.h"
#include "ActCharacter.h"
#include "ActPlayerState.h"
#include "EngineUtils.h"
#include "AI/ActAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("act.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

AActGameModeBase::AActGameModeBase()
{
	SpawnTimerInterval = 2.f;
}

void AActGameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnPickups();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, 
		&AActGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}


void AActGameModeBase::SpawnBotTimerElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning skipped via cvar 'CVarSpawnBots'"))
		return;
	}

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

void AActGameModeBase::KillAllBots()
{
	for (TActorIterator<AActAICharacter> It(GetWorld()); It; ++It)
	{
		AActAICharacter* Bot = *It;
		UActAttributeComponent* BotAttrs = UActAttributeComponent::GetAttributes(Bot);
		if (ensure(BotAttrs) && BotAttrs->IsAlive())
		{
			BotAttrs->Kill(this); //Maybe pass in player for kill credit in the future?
		}
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

		//DrawDebugSphere(GetWorld(), Locations[0], 50.f, 20, FColor::Blue, false, 60.0f);
		//UE_LOG(LogTemp, Warning, TEXT("Actor spawned: %p"), SpawnActor);
	}
}


void AActGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	if(AActCharacter* PlayerCharacterVictim = Cast<AActCharacter>(VictimActor))
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", PlayerCharacterVictim->GetController());


		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	} else if (AActAICharacter* BotCharacter = Cast<AActAICharacter>(VictimActor))
	{
		if(AActCharacter* PlayerCharacterKiller = Cast<AActCharacter>(Killer))
		{
			AActPlayerState* PlayerState = Cast<AActPlayerState>(PlayerCharacterKiller->GetPlayerState());
			PlayerState->ModifyCredits(this, 20);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("OnActorKilled: Victim %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void AActGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if(Controller)
	{
		//Removes character from the controller, it doesn't destroy it. But when we Restart we'll get a fresh player character
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void AActGameModeBase::SpawnPickups()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInst = UEnvQueryManager::RunEQSQuery(
		this, SpawnPowerupQuery, this, EEnvQueryRunMode::AllMatching, nullptr);

	if (ensure(QueryInst))
	{
		QueryInst->GetOnQueryFinishedEvent().AddDynamic(this, &AActGameModeBase::OnSpawnPowerupQueryCompleted);
	}
}


void AActGameModeBase::OnSpawnPowerupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn powerup EQS Query Failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.Num() > 0)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		for(int i = 0; i < Locations.Num(); i++)
		{
			if(i % 4 == 0)
			{
				AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(CreditPickup, Locations[i], FRotator::ZeroRotator, SpawnParams);
				//DrawDebugSphere(GetWorld(), Locations[i], 50.f, 20, FColor::Orange, false, 60.0f);
			}
			else if (i % 7 == 0)
			{
				AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(HealthPickup, Locations[i], FRotator::ZeroRotator, SpawnParams);
				//DrawDebugSphere(GetWorld(), Locations[i], 50.f, 20, FColor::Cyan, false, 60.0f);
			}
		}
	}
}

