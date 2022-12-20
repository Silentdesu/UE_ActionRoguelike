// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ASItemChest();

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UFUNCTION()
	void OnRep_LidOpened();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(EditAnywhere, Category = "Basic settings")
	float TargetPitch;

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly)
	bool bLidOpened;
};
