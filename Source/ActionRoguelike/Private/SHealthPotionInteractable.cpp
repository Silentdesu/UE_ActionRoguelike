// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotionInteractable.h"
#include "../Public/SAttributeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SCreditSystem.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ASHealthPotionInteractable::ASHealthPotionInteractable()
{
	CreditsAmount = 50;
}

void ASHealthPotionInteractable::Interact_Implementation(APawn* instigatorPawn)
{
	if (!instigatorPawn)
		return;

	USAttributeComponent* attributeComponent = USAttributeComponent::GetAttributes(instigatorPawn);

	if (attributeComponent && !attributeComponent->IsFullHealth())
		if (ASCreditSystem* CreditSystem = instigatorPawn->GetPlayerState<ASCreditSystem>())
			if (CreditSystem->RemoveCredits(CreditsAmount) && attributeComponent->ApplyHealthChange(this, HealAmount))
				HideAndCooldown();

}

FText ASHealthPotionInteractable::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn);

	if (AttributeComponent && AttributeComponent->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}

	return FText::Format(LOCTEXT("HealthPotion_InteractableMessage", "Cost {0} credits. Restores health to maximum."), CreditsAmount);
}

#undef LOCTEXT_NAMESPACE
