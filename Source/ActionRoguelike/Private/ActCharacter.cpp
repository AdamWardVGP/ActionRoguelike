// Fill out your copyright notice in the Description page of Project Settings.


#include "ActCharacter.h"

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
	// ensure() is an unreal equivalent macro for "try" or assert. it will only throw the first time and lets you continue afterward
	// ensureAlways() can trigger it every time if you want to be more intrusive
	// ensure() also doesn't get included in packaged builds.
	// check() can be used too, but it will halt the entire execution if it's hit and you cant proceed.
	if(ensure(PrimaryProjectileClass))
	{
		LaunchProjectileTowardCrosshair(PrimaryProjectileClass);
	}
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
	FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 5000.f);

	TArray<FHitResult> Hits;
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	QueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	GetWorld()->LineTraceMultiByObjectType(Hits, CameraLocation, TraceEnd, QueryParams);

	float Radius = 30.f;
	//Sweep will instead scan across the vector with the given shape
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, CameraLocation, TraceEnd, FQuat::Identity, QueryParams, Shape);

	bool debugEnabled = CVarDebugDrawCharacter.GetValueOnGameThread();

	FVector Target;
	if (!Hits.IsEmpty())
	{
		Target = Hits[0].ImpactPoint;

		if (debugEnabled)
		{
			DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Green, false, 5.f, 0, 3.f);
			DrawDebugSphere(GetWorld(), Target, Radius, 32, FColor::Green, false, 5.f, 0, 3.f);
		}

	}
	else
	{
		Target = TraceEnd;

		if (debugEnabled)
		{
			DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Red, false, 5.f, 0, 3.f);
		}
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

void AActCharacter::OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta <0.0f)
	{
		
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

