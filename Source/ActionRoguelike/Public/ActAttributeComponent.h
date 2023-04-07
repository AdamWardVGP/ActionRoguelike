// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UActAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, UActAttributeComponent*, OwningComp, float, NewRage, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UActAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UActAttributeComponent* GetAttributes(AActor* FromActor);

#pragma region Health
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attrributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attrributes")
	float MaxHealth;

public:

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* FromActor);

	/**
	 * Determines if an actor's health is "low" based on a percentage of it's overall health.
	 */
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static bool IsLowHealth(AActor* FromActor, float ThresholdPercent);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsAtMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attrributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attrributes")
	float MaxRage;

public:

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxRage();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;
		
};
