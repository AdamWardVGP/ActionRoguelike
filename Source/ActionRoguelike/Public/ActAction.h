// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActActionComponent.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "ActAction.generated.h"

class UWorld;

//Wrapping both fields in a struct allows us to know both fields are synchronized with one another
//Any payload arriving on a machine will get both values updated appropriately.
USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning;

	//Structs actually work opposite for replication
	//all these fields automatically are replicated.
	//Instead we have to mark it "NotReplicated"
	UPROPERTY()
	AActor* Instigator;
};


/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UActAction : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated)
	UActActionComponent* OwnerActionComponent;

	/* Tags automatically added to owning actor when activated, and removed when action ends */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Tags which prevent action from starting if the owner has any of them currently active */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UActActionComponent* GetOwningComponent() const;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();

public:

	void Initialize(UActActionComponent* NewActionComp);

	/* Immediately start when added to an action component */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UWorld* GetWorld() const override;

	virtual bool IsSupportedForNetworking() const override;

};
