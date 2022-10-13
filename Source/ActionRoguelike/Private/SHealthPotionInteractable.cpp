// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotionInteractable.h"
#include "../Public/SAttributeComponent.h"

void ASHealthPotionInteractable::OnInteract(APawn* InstigatorPawn)
{
	USAttributeComponent* attributeComponent = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	if (attributeComponent != nullptr)
		attributeComponent->ApplyHealthChange(HealAmount);

	Super::OnInteract(InstigatorPawn);
}
