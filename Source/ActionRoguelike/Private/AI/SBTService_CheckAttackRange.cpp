// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

USBTService_CheckAttackRange::USBTService_CheckAttackRange()
{
	NodeName = TEXT("Check Attack Range");
}

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* blackboardComponent = OwnerComp.GetBlackboardComponent();

	if (blackboardComponent)
	{
		AActor* targetActor = Cast<AActor>(blackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName));

		if (targetActor)
		{
			AAIController* aiController = OwnerComp.GetAIOwner();

			if (aiController)
			{
				APawn* aiPawn = aiController->GetPawn();

				if (aiPawn)
				{
					float distance = FVector::Distance(aiPawn->GetActorLocation(), targetActor->GetActorLocation());

					bool bWithinRange = distance < AttackRange;
					
					bool bHasLineOfSight;
					
					if (bWithinRange)
						bHasLineOfSight = aiController->LineOfSightTo(targetActor);

					blackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLineOfSight));
				}
			}
		}
	}
}
