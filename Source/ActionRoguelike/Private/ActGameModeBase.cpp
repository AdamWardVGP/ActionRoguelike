// Fill out your copyright notice in the Description page of Project Settings.


#include "ActGameModeBase.h"

#include "ActActionComponent.h"
#include "ActAttributeComponent.h"
#include "ActCharacter.h"
#include "ActGameplayInterface.h"
#include "ActMonsterData.h"
#include "ActPlayerState.h"
#include "ActSaveGame.h"
#include "EngineUtils.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "AI/ActAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("act.SpawnBots"), false, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

AActGameModeBase::AActGameModeBase()
{
	SpawnTimerInterval = 2.f;
	SlotName = "SaveGame1";
}

void AActGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("InitGame"));
	LoadSaveGame();
}

void AActGameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnPickups();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, 
		&AActGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void AActGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Executing before Super:: so that variable are initialized before 'beginplayingstate' is called in PlayerController
	AActPlayerState* PlayerState = NewPlayer->GetPlayerState<AActPlayerState>();
	if(PlayerState)
	{
		//this doesn't work with multiplayer, everyone loads the same state
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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
		//AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator, SpawnParams);

		if(MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("ActGameModeBase", Rows);

			// Get Random Enemy
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfValid();
			if(Manager)
			{
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AActGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);

				LogOnScreen(this, FString::Printf(TEXT("Loading monster asset: %s"), *SelectedRow->MonsterId.ToString()));

				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}
		}
	}
}

void AActGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this, FString::Printf(TEXT("Monster asset loaded")));

	UAssetManager* Manager = UAssetManager::GetIfValid();
	if(Manager)
	{
		UActMonsterData* MonsterData = Cast<UActMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
		if(MonsterData)
		{

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);

			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

				UActActionComponent* ActionComp = Cast<UActActionComponent>(NewBot->GetComponentByClass(UActActionComponent::StaticClass()));

				if (ActionComp)
				{
					for (TSubclassOf<UActAction> ActionClass : MonsterData->Actions)
					{
						ActionComp->AddAction(NewBot, ActionClass);
					}
				}
			}
		}
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
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for(int i = 0; i < Locations.Num(); i++)
		{
			if(i % 17 == 0)
			{
				AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(CreditPickup, Locations[i], FRotator::ZeroRotator, SpawnParams);
				//UE_LOG(LogTemp, Warning, TEXT("Pup spawn: %ls"), *GetNameSafe(SpawnActor))
				//DrawDebugSphere(GetWorld(), Locations[i], 50.f, 20, FColor::Orange, false, 60.0f);
			}
			else if (i % 21 == 0)
			{
				AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(HealthPickup, Locations[i], FRotator::ZeroRotator, SpawnParams);
				//UE_LOG(LogTemp, Warning, TEXT("Pup spawn: %ls"), *GetNameSafe(SpawnActor))
				//DrawDebugSphere(GetWorld(), Locations[i], 50.f, 20, FColor::Cyan, false, 60.0f);
			}
		}
	}
}


void AActGameModeBase::WriteSaveGame()
{
	for(int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AActPlayerState* PS = Cast<AActPlayerState>(GameState->PlayerArray[i]);
		if(PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; //Single player only at this point
		}
	}

	CurrentSaveGame->SavedActorData.Empty();

	for(FActorIterator It(GetWorld()); It; ++It)
	{

		AActor* Actor = *It;

		//Only interested in our custom gameplay actors
		if(!Actor->Implements<UActGameplayInterface>())
		{
			continue;
		}

		FSavedActorData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();


		FMemoryWriter MemWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		//Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		//Converts Actor's SaveGame UPROPERTY fields into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActorData.Add(ActorData);
	}


	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void AActGameModeBase::LoadSaveGame()
{
	if(UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UActSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if(CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."))
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Loaded SaveGame Data."))

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			
			AActor* Actor = *It;

			UE_LOG(LogTemp, Warning, TEXT("WorldActor %s"), *GetNameSafe(Actor));

			//Only interested in our custom gameplay actors
			if (!Actor->Implements<UActGameplayInterface>())
			{
				continue;
			}

			for (FSavedActorData ActorData : CurrentSaveGame->SavedActorData)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					//Find only variables with UPROPERTY(SaveGame)
					Ar.ArIsSaveGame = true;
					//Converts binary array back to actor's variables
					Actor->Serialize(Ar);

					IActGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}

		for (FSavedActorData ActorData : CurrentSaveGame->SavedActorData)
		{
			UE_LOG(LogTemp, Warning, TEXT("SavedActor %s"), *ActorData.ActorName);
		}


	}
	else
	{
		CurrentSaveGame = Cast<UActSaveGame>(UGameplayStatics::CreateSaveGameObject(UActSaveGame::StaticClass()));
		UE_LOG(LogTemp, Warning, TEXT("Created new SaveGame Data."))
	}
}
