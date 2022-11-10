// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoinInteractable.h"
#include "SCreditSystem.h"

ASCoinInteractable::ASCoinInteractable()
{
	CreditsAmount = 50;
}

void ASCoinInteractable::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn == nullptr)
		return;

	if (ASCreditSystem* CreditSystem = InstigatorPawn->GetPlayerState<ASCreditSystem>())
	{
		CreditSystem->AddCredits(CreditsAmount);
		HideAndCooldown();
	}
}
