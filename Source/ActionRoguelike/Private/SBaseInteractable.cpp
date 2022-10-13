// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseInteractable.h"
#include "SAttributeComponent.h"

ASBaseInteractable::ASBaseInteractable()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void ASBaseInteractable::Interact_Implementation(APawn* InstigatorPawn)
{
	USAttributeComponent* attributeComponent = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	if (attributeComponent != nullptr && !attributeComponent->IsHealthFull())
		OnInteract(InstigatorPawn);
}

void ASBaseInteractable::OnInteract(APawn* InstigatorPawn)
{
	MeshComponent->SetVisibility(false);

	GetWorldTimerManager().SetTimer(OnInteractDelayTime, this, &ASBaseInteractable::OnInteractTimeElapsed, VisiblityDelay);
}

void ASBaseInteractable::OnInteractTimeElapsed()
{
	GetWorldTimerManager().ClearTimer(OnInteractDelayTime);

	MeshComponent->SetVisibility(true);;
}


