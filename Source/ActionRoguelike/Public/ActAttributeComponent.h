// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActAttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UActAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActAttributeComponent();

protected:

	// EditAnywhere - Edit in BP editor and per-instanc ein level
	// VisibleAnywhere - 'read-only' in editor and level. (Use for Components)
	// EditDefaultsOnly - Hide variable per instance, edit in BP editor only
	// VisibleDefaultsOnly - 'read only' access for variable, only in BP editor (uncommon)
	// EditInstanceOnly - Allow only editing of an instance (i.e. only when placed in a level)
	// --
	// BlueprintReadOnly - Only appears in blueprint for read (does not show up in instance detail panel)
	// BlueprintReadWrite - Appears in blueprint for read and write
	// --
	// Category - Friendly way to sort properties under a shared dropdown in the blueprint and instance detail panels.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attrributes")
	float Health;

	// HealthMax, Stamina, Strength

public:	

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);
		
};
