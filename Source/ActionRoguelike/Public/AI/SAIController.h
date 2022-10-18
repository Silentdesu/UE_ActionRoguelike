// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAIController.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASAIController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Basic settings")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Blackboard keys")
	FName TargetKeyName = "MoveToLocation";

protected:

	virtual void BeginPlay() override;
};
