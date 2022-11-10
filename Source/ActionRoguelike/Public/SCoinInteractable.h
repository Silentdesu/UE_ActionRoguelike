// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseInteractable.h"
#include "SCoinInteractable.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASCoinInteractable : public ASBaseInteractable
{
	GENERATED_BODY()

public:
	
	ASCoinInteractable();

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;
};
