// Fill out your copyright notice in the Description page of Project Settings.


#include <BehaviorTree/BlackboardComponent.h>
#include "SBTService_CheckOwnHealth.h"
#include "SAttributeComponent.h"
#include "AIController.h"

USBTService_CheckOwnHealth::USBTService_CheckOwnHealth()
{
	NodeName = TEXT("Check Own Health");
	IsLowOwnHealthKey.SelectedKeyName = "IsLowOwnHealth";

	MinLowHealth = 0.5f;
}

void USBTService_CheckOwnHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (AIPawn)
	{
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(AIPawn);

		if (AttributeComponent)
		{
			bool bLowHealth = (AttributeComponent->GetCurrentHealth() / AttributeComponent->GetMaxHealth()) <= MinLowHealth;

			UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
			BlackboardComponent->SetValueAsBool(IsLowOwnHealthKey.SelectedKeyName, bLowHealth);
		}
	}
}
