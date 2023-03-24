// Fill out your copyright notice in the Description page of Project Settings.


#include "ActCharacter.h"

#include "ActInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AActCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	PlayAnimMontage(AttackAnim);

	//TODO change this to an animation event, for now we'll use a timer.
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AActCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void AActCharacter::PrimaryAttack_TimeElapsed_Implementation()
{

}

void AActCharacter::SecondaryAttack()
{
	PlayAnimMontage(AttackAnim);

	//TODO change this to an animation event, for now we'll use a timer.
	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &AActCharacter::SecondaryAttack_TimeElapsed, 0.2f);
}

void AActCharacter::SecondaryAttack_TimeElapsed_Implementation()
{
	LaunchProjectileTowardCrosshair(SecondaryProjectileClass);
}

void AActCharacter::UltAttack()
{
	PlayAnimMontage(AttackAnim);

	//TODO change this to an animation event, for now we'll use a timer.
	GetWorldTimerManager().SetTimer(TimerHandle_UltAttack, this, &AActCharacter::UltAttack_TimeElapsed, 0.2f);
}

void AActCharacter::UltAttack_TimeElapsed_Implementation()
{
	LaunchProjectileTowardCrosshair(UltProjectileClass);
}

void AActCharacter::LaunchProjectileTowardCrosshair(TSubclassOf<AActor> Projectile)
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector CameraLocation = CameraComp->K2_GetComponentLocation();
	FRotator CameraRotation = CameraComp->K2_GetComponentRotation();
	FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 1000.f);

	TArray<FHitResult> Hits;
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	QueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	GetWorld()->LineTraceMultiByObjectType(Hits, CameraLocation, TraceEnd, QueryParams);

	FVector Target;
	if (!Hits.IsEmpty())
	{
		Target = Hits[0].ImpactPoint;
	}
	else
	{
		Target = TraceEnd;
	}

	FRotator ProjectileRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, Target);

	FTransform SpawnTM = FTransform(ProjectileRot, HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(Projectile, SpawnTM, SpawnParams);
}


void AActCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

