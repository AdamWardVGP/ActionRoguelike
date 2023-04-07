// Fill out your copyright notice in the Description page of Project Settings.


#include "ActThornsActionEffect.h"

#include "ActAttributeComponent.h"
#include "ActCharacter.h"

UActThornsActionEffect::UActThornsActionEffect()
{
	ThornsDamage = 5.f;
	bAutoStart = true;
}

void UActThornsActionEffect::StartAction_Implementation(AActor* Instigator)
{
	AActCharacter* OwningActor = Cast<AActCharacter>(GetOwningComponent()->GetOwner());
	if(!OwningActor)
	{
		return;
	}

	UActAttributeComponent* AttributeComponent = Cast<UActAttributeComponent>(OwningActor->GetComponentByClass(UActAttributeComponent::StaticClass()));

	if(AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &UActThornsActionEffect::OnHealthChanged);
	}
}

void UActThornsActionEffect::StopAction_Implementation(AActor* Instigator)
{
	AActCharacter* OwningActor = Cast<AActCharacter>(GetOwningComponent()->GetOwner());
	if (!OwningActor)
	{
		return;
	}

	UActAttributeComponent* AttributeComponent = Cast<UActAttributeComponent>(OwningActor->GetComponentByClass(UActAttributeComponent::StaticClass()));

	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.RemoveDynamic(this, &UActThornsActionEffect::OnHealthChanged);
	}
}

void UActThornsActionEffect::OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	UActAttributeComponent* AttributeComponent = Cast<UActAttributeComponent>(InstigatorActor->GetComponentByClass(UActAttributeComponent::StaticClass()));
	if(AttributeComponent)
	{
		//Could be worthwhile to add additional fields other than Instigator. We don't want infinite recursion
		//of thorns triggering thorns.
		AttributeComponent->ApplyHealthChange(InstigatorActor, -1 * ThornsDamage);
	}
}