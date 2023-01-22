// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseInteractable.h"
#include "SHealthPotionInteractable.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotionInteractable : public ASBaseInteractable
{
	GENERATED_BODY()

public:

	ASHealthPotionInteractable();

protected:

	UPROPERTY(EditAnywhere, Category = "Basic settings")
	float HealAmount = 50.f;

public:

	void Interact_Implementation(APawn* instigatorPawn) override;
	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
};
