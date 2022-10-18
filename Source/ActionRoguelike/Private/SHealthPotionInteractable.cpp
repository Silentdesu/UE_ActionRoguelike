// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotionInteractable.h"
#include "../Public/SAttributeComponent.h"
#include "Components/StaticMeshComponent.h"

ASHealthPotionInteractable::ASHealthPotionInteractable()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);
}

void ASHealthPotionInteractable::Interact_Implementation(APawn* instigatorPawn)
{
	if (!instigatorPawn)
		return;

	USAttributeComponent* attributeComponent = USAttributeComponent::GetAttributes(instigatorPawn);

	if (attributeComponent && !attributeComponent->IsFullHealth())
		if (attributeComponent->ApplyHealthChange(this, HealAmount))
			HideAndCooldown();

}
