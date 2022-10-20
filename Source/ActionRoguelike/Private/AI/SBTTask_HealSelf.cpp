// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"
#include "SAttributeComponent.h"
#include "AIController.h"

USBTTask_HealSelf::USBTTask_HealSelf()
{
	NodeName = TEXT("Heal Self");
}

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (Pawn == nullptr)
		return EBTNodeResult::Failed;

	USAttributeComponent* AttributeCompnent = USAttributeComponent::GetAttributes(Pawn);

	if (AttributeCompnent)
	{
		AttributeCompnent->ApplyHealthChange(Pawn, AttributeCompnent->GetMaxHealth());
	}

	return EBTNodeResult::Succeeded;
}

