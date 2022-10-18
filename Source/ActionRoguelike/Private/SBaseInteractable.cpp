// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseInteractable.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"

ASBaseInteractable::ASBaseInteractable()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("Interactable");
	RootComponent = SphereComponent;
}

void ASBaseInteractable::Interact_Implementation(APawn* InstigatorPawn)
{
	//Create implementation
}

void ASBaseInteractable::Show()
{
	SetInteractableActiveState(true);
}

void ASBaseInteractable::HideAndCooldown()
{
	SetInteractableActiveState(false);

	GetWorldTimerManager().SetTimer(OnInteractDelayTimer, this, &ASBaseInteractable::Show, RespawnTime);
}

void ASBaseInteractable::SetInteractableActiveState(bool bActiveState)
{
	GetWorldTimerManager().ClearTimer(OnInteractDelayTimer);

	SetActorEnableCollision(bActiveState);
	SphereComponent->SetVisibility(bActiveState, true);
}

