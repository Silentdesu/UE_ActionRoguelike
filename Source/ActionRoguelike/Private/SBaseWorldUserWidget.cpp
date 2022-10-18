// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void USBaseWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D screenPosition;

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing this Widget"));
		return;
	}

	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, screenPosition))
	{
		float viewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
		
		screenPosition /= viewportScale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(screenPosition);
		}
	}
}
