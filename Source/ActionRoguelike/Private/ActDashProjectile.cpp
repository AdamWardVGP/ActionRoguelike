// Fill out your copyright notice in the Description page of Project Settings.


#include "ActDashProjectile.h"

AActDashProjectile::AActDashProjectile()
{
	
}

void AActDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_Projectile, this, &AActDashProjectile::DurationElapsed, 2.f);
}

void AActDashProjectile::DurationElapsed()
{
	GetInstigator()->SetActorLocation(this->GetActorLocation());
	Destroy();
}