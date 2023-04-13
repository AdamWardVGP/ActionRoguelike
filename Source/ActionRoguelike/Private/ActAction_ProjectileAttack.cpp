// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAction_ProjectileAttack.h"

#include "ActCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


UActAction_ProjectileAttack::UActAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void UActAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	if(AActCharacter* Character = Cast<AActCharacter>(Instigator))
	{
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		if(Character->HasAuthority())
		{
			FTimerHandle TimerHandle_AtkDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AtkDelay, Delegate, AttackAnimDelay, false);
		}
	}
}

void UActAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatingCharacter)
{
	// ensure() is an unreal equivalent macro for "try" or assert. it will only throw the first time and lets you continue afterward
	// ensureAlways() can trigger it every time if you want to be more intrusive
	// ensure() also doesn't get included in packaged builds.
	// check() can be used too, but it will halt the entire execution if it's hit and you cant proceed.
	if (ensure(ProjectileClass))
	{
		FVector HandLocation = InstigatingCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatingCharacter;

		float Radius = 30.f;
		FCollisionShape Shape;
		Shape.SetSphere(Radius);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatingCharacter);

		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		QueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		QueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);


		FVector CameraLocation = InstigatingCharacter->GetPawnViewLocation();
		FRotator ControlRotation = InstigatingCharacter->GetControlRotation();
		FVector TraceEnd = CameraLocation + (ControlRotation.Vector() * 5000.f);

		FHitResult Hit;

		FVector Target;
		if (GetWorld()->SweepSingleByObjectType(Hit, CameraLocation, TraceEnd, FQuat::Identity, QueryParams, Shape))
		{
			Target = Hit.Location;
		}
		else
		{
			Target = TraceEnd;
		}

		FRotator ProjectileRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, Target);

		FTransform SpawnTM = FTransform(ProjectileRot, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatingCharacter);
}