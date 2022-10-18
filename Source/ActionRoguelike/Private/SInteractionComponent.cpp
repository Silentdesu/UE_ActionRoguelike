// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	TraceLineLength = 1000.f;
	SweepSphereRadius = 30.f;
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams collisionObjectQueryParams;
	collisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* owner = GetOwner();

	FVector startLocation;
	FRotator outRotator;
	owner->GetActorEyesViewPoint(startLocation, outRotator);

	FVector endLocation = startLocation + (outRotator.Vector() * TraceLineLength);

	//FHitResult hitResult;
	//bHit = GetWorld()->LineTraceSingleByObjectType(hitResult, startLocation, endLocation, collisionObjectQueryParams);

	TArray<FHitResult> hitResults;
	bool bHit;
	FCollisionShape shape;
	shape.SetSphere(SweepSphereRadius);
	bHit = GetWorld()->SweepMultiByObjectType(hitResults, startLocation, endLocation, FQuat::Identity,
		collisionObjectQueryParams, shape);

	FColor color = bHit ? FColor::Green : FColor::Red;

	for (FHitResult hitResult : hitResults)
	{
		AActor* actor = hitResult.GetActor();

		if (actor == nullptr)
			return;

		if (actor->Implements<USGameplayInterface>())
		{
			APawn* pawn = Cast<APawn>(owner);

			ISGameplayInterface::Execute_Interact(actor, pawn);
		}
		
		DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, SweepSphereRadius, 16, color, false, 2.f);
	}

	DrawDebugLine(GetWorld(), startLocation, endLocation, color, false, 2.f, 0, 2.f);
}

