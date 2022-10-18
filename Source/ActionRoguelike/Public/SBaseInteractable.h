// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SBaseInteractable.generated.h"

class USphereComponent;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASBaseInteractable : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBaseInteractable();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;

protected:

	UPROPERTY(EditAnywhere, Category = "Time settings")
	float RespawnTime = 10.f;

	FTimerHandle OnInteractDelayTimer;

protected:

	void Show();
	void HideAndCooldown();
	void SetInteractableActiveState(bool bActiveState);

public:

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
