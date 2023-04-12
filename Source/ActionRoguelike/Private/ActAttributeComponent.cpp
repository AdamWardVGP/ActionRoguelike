// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAttributeComponent.h"

#include "ActGameModeBase.h"
#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("act.DamageMultiplier"), 1.0f, TEXT("Global damage modifier for Attribute Component."), ECVF_Cheat);

UActAttributeComponent::UActAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100.f;
	Health = MaxHealth;

	Rage = 0.0f;
	MaxRage = 100.0f;

	//When setting bReplicates Unreal prefers this method if setting it in the constructor
	SetIsReplicatedByDefault(true);
}

UActAttributeComponent* UActAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<UActAttributeComponent>(FromActor->GetComponentByClass(UActAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UActAttributeComponent::IsActorAlive(AActor* FromActor)
{
	if(UActAttributeComponent* ActAttributeComponent = GetAttributes(FromActor))
	{
		return ActAttributeComponent->IsAlive();
	}

	return false;
}

bool UActAttributeComponent::IsLowHealth(AActor* FromActor, float ThresholdPercent)
{
	if (UActAttributeComponent* ActAttributeComponent = GetAttributes(FromActor))
	{
		return (ActAttributeComponent->Health / ActAttributeComponent->MaxHealth) <= ThresholdPercent;
	}

	return false;
}

bool UActAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{

	if(!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ignoring damage, god mode is enabled"))
		return false;
	}

	float OldHealth = Health;

	if(Delta < 0.0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);

	float ActualDelta = Health - OldHealth;

	//OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	if(ActualDelta != 0.0f)
	{

		//if called by a client this just calls locally, when called from the server
		//it will be broadcast to all clients
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	}

	if(ActualDelta <0.0f && Health == 0.0f)
	{
		AActGameModeBase* GM = GetWorld()->GetAuthGameMode<AActGameModeBase>();
		if(GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return ActualDelta != 0;
}

float UActAttributeComponent::GetMaxHealth()
{
	return MaxHealth;
}

bool UActAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UActAttributeComponent::IsAtMaxHealth() const
{
	return Health == MaxHealth;
}

bool UActAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -1 * GetMaxHealth());
}

float UActAttributeComponent::GetMaxRage()
{
	return MaxRage;
}

bool UActAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	const float OldRage = Rage;
	Rage = FMath::Clamp(Rage + Delta, 0, MaxRage);
	const float ActualDelta = Rage - OldRage;

	if (ActualDelta != 0.0f)
	{
		MulticastRageChanged(InstigatorActor, Rage, Delta);
	}

	return ActualDelta != 0;
}

void UActAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void UActAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void UActAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UActAttributeComponent, Health);
	DOREPLIFETIME(UActAttributeComponent, MaxHealth);
	DOREPLIFETIME(UActAttributeComponent, Rage);
	DOREPLIFETIME(UActAttributeComponent, MaxRage);
}


