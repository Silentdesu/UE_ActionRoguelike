// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

public:

	USBTService_CheckAttackRange();

protected:

	UPROPERTY(EditAnywhere, Category = "Service settings")
	float AttackRange = 2000.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttackRangeKey;

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
