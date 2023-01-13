// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseInteractable.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ASBaseInteractable::ASBaseInteractable()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("Interactable");
	RootComponent = SphereComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);

	SetReplicates(true);
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

void ASBaseInteractable::SetInteractableActiveState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;

	OnRep_IsActive();
}

void ASBaseInteractable::OnRep_IsActive()
{
	GetWorldTimerManager().ClearTimer(OnInteractDelayTimer);

	SetActorEnableCollision(bIsActive);
	SphereComponent->SetVisibility(bIsActive, true);
}

void ASBaseInteractable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASBaseInteractable, bIsActive);
}

