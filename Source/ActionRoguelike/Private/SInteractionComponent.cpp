// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "SBaseWorldUserWidget.h"

//TODO: Refactor

static TAutoConsoleVariable<bool> CVarDrawDebug(TEXT("su.DrawDebug"), false, TEXT("Draw Debug for Interaction Component."), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceLineLength = 1000.f;
	SweepSphereRadius = 30.f;
	CollisionChannel = ECC_WorldDynamic;
}

void USInteractionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDrawDebug = CVarDrawDebug.GetValueOnGameThread();

	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* owner = GetOwner();

	FVector StartLocation;
	FRotator OutRotator;
	owner->GetActorEyesViewPoint(StartLocation, OutRotator);

	FVector EndLocation = StartLocation + (OutRotator.Vector() * TraceLineLength);

	TArray<FHitResult> HitResults;
	bool bHit;
	FCollisionShape Shape;
	Shape.SetSphere(SweepSphereRadius);
	bHit = GetWorld()->SweepMultiByObjectType(HitResults, StartLocation, EndLocation, FQuat::Identity,
		CollisionObjectQueryParams, Shape);

	FColor Color = bHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;

	for (FHitResult HitResult : HitResults)
	{
		if (bDrawDebug)
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SweepSphereRadius, 16, Color, false, 2.f);

		AActor* HitActor = HitResult.GetActor();

		if (HitActor == nullptr)
			return;

		if (HitActor->Implements<USGameplayInterface>())
		{
			FocusedActor = HitActor;

			break;
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && DefaultWidgetClass)
			DefaultWidgetInstance = CreateWidget<USBaseWorldUserWidget>(GetWorld(), DefaultWidgetClass);

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
				DefaultWidgetInstance->AddToViewport();
		}
	}
	else
	{
		if (DefaultWidgetInstance)
			DefaultWidgetInstance->RemoveFromParent();
	}

	if (bDrawDebug)
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, false, 2.f, 0, 2.f);
}

void USInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	ISGameplayInterface::Execute_Interact(FocusedActor, OwnerPawn);
}
