// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UActInteractionComponent;
class UAnimMontage;
class UActAttributeComponent;
class UActActionComponent;

UCLASS()
class ACTIONROGUELIKE_API AActCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActCharacter();

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	void PrimaryAttack();

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FTimerHandle TimerHandle_PrimaryAttack;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PrimaryAttack_TimeElapsed();


	void SecondaryAttack();

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> SecondaryProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FTimerHandle TimerHandle_SecondaryAttack;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SecondaryAttack_TimeElapsed();

	void UltAttack();

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> UltProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FTimerHandle TimerHandle_UltAttack;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void UltAttack_TimeElapsed();

	void LaunchProjectileTowardCrosshair(TSubclassOf<AActor> Projectile);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UActInteractionComponent* InteractionComp;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void PrimaryInteract();

	void SprintStart();

	void SprintStop();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UActAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UActActionComponent* ActionComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Exec marks as a console command
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

	virtual FVector GetPawnViewLocation() const override;

};
