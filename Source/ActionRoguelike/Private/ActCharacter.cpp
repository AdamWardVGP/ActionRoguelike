// Fill out your copyright notice in the Description page of Project Settings.


#include "ActCharacter.h"

#include "ActActionComponent.h"
#include "ActAttributeComponent.h"
#include "ActInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

static TAutoConsoleVariable<bool> CVarDebugDrawCharacter(TEXT("act.CharacterDebugDraw"), false, TEXT("Enable debug geometry for character attacks"), ECVF_Cheat);

// Sets default values
AActCharacter::AActCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	InteractionComp = CreateDefaultSubobject<UActInteractionComponent>("InteractionComponent");

	AttributeComp = CreateDefaultSubobject<UActAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<UActActionComponent>("ActionComp");

	bUseControllerRotationYaw = false;
}

void AActCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &AActCharacter::OnHealthChanged);
}

// Called to bind functionality to input
void AActCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AActCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AActCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AActCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AActCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AActCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &AActCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("UltAttack", IE_Pressed, this, &AActCharacter::UltAttack);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AActCharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AActCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AActCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AActCharacter::SprintStop);
}

void AActCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void AActCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;

	//Movement to the right should be the right vector relative to the forward vector of the camera
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void AActCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void AActCharacter::SecondaryAttack()
{
	ActionComp->StartActionByName(this, "SecondaryAttack");
}

void AActCharacter::UltAttack()
{
	ActionComp->StartActionByName(this, "UltAttack");
}

void AActCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void AActCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void AActCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void AActCharacter::OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth,
                                    float Delta)
{
	if(Delta <0.0f)
	{

		AttributeComp->ApplyRageChange(this, -1 * Delta);
		
		if (NewHealth <= 0.0f)
		{
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			DisableInput(PlayerController);
		} else
		{
			GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		}
	}
}


void AActCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

FVector AActCharacter::GetPawnViewLocation() const
{
	return CameraComp->K2_GetComponentLocation();
}

