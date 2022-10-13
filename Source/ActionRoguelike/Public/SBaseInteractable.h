// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SBaseInteractable.generated.h"

class UStaticMeshComponent;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASBaseInteractable : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBaseInteractable();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

protected:

	UPROPERTY(EditAnywhere, Category = "Time settings")
	float VisiblityDelay = 10.f;

	FTimerHandle OnInteractDelayTime;

protected:

	virtual void OnInteract(APawn* InstigatorPawn);
	virtual void OnInteractTimeElapsed();

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;
};
