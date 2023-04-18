// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAnimInstance.h"

#include "ActActionComponent.h"
#include "GameplayTagContainer.h"

void UActAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if(AActor* OwningActor = GetOwningActor())
	{
		ActionComp = Cast<UActActionComponent>(GetOwningActor()->GetComponentByClass(UActActionComponent::StaticClass()));
	}
}

void UActAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if(ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}


