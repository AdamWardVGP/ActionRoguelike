// Fill out your copyright notice in the Description page of Project Settings.


#include "ActProjectileBase.h"

#include "ActAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeSourceComponent.h"

// Sets default values
AActProjectileBase::AActProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	CastingComponent = CreateDefaultSubobject<UParticleSystemComponent>("CastingComp");
	CastingComponent->SetupAttachment(SphereComp);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjMoveComp");
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;

	InFlightAudioCue = CreateDefaultSubobject<UAudioComponent>("InFlightAudio");
	InFlightAudioCue->SetupAttachment(SphereComp);

	ImpactAudioCue = CreateDefaultSubobject<UAudioComponent>("ImpactAudio");

	CameraShakeComponent = CreateDefaultSubobject<UCameraShakeSourceComponent>("CameraComp");
	CameraShakeComponent->SetupAttachment(SphereComp);
}

void AActProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	SphereComp->OnComponentHit.AddDynamic(this, &AActProjectileBase::OnActorHit);
}

void AActProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	if (ensure(InFlightAudioCue))
	{
		InFlightAudioCue->Play(0);
	}
}

void AActProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AActProjectileBase::Explode_Implementation()
{
	if (ensure(InFlightAudioCue))
	{
		InFlightAudioCue->Stop();
	}

	if(const UWorld* World = GetWorld())
	{
		if(ensure(ImpactAudioCue))
		{
			UGameplayStatics::PlaySoundAtLocation(World, ImpactAudioCue->GetSound(), GetActorLocation(), GetActorRotation());
		}

		UGameplayStatics::PlayWorldCameraShake(World, CameraShakeComponent->CameraShake, GetActorLocation(), 10000.f, 1000000.f);
	}


	if(ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		Destroy();
	}
}