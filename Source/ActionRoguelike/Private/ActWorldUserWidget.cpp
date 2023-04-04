// Fill out your copyright notice in the Description page of Project Settings.


#include "ActWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void UActWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	FVector2D ScreenPosition;

	if(!IsValid(AttachedActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("AttachedActor is no longer valid, removing Health widget."))
		return;
	}

	if(UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		//Results from ProjectWorldToScreen still aren't located properly in the viewport, we need to re-scale it to the correct position.
		float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
		ScreenPosition /= ViewportScale;

		if(ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
