// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotionInteractable.h"
#include "../Public/SAttributeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SCreditSystem.h"

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
