// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActAICharacter.generated.h"

class UPawnSensingComponent;
class UActAttributeComponent;
class UActActionComponent;
class UUserWidget;
class UActWorldUserWidget;

UCLASS()
class ACTIONROGUELIKE_API AActAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AActAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UActActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthbarWidgetClass;

	UActWorldUserWidget* ActiveHealthBar;

	void setTargetActor(AActor* NewTarget);

	virtual	void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UActAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth, float Delta);


};
