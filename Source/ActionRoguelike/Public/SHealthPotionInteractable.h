// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseInteractable.h"
#include "SHealthPotionInteractable.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotionInteractable : public ASBaseInteractable
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Basic settings")
	float HealAmount = 50.f;

protected:

	virtual void OnInteract(APawn* InstigatorPawn) override;
};
