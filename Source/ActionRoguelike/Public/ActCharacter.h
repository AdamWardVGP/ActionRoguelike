// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UActInteractionComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API AActCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActCharacter();

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FTimerHandle TimerHandle_PrimaryAttack;

	void PrimaryAttack();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PrimaryAttack_TimeElapsed();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UActInteractionComponent* InteractionComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void PrimaryInteract();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
