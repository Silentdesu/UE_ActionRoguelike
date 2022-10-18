// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"

USBTTask_RangedAttack::USBTTask_RangedAttack()
{
	NodeName = TEXT("Ranged Attack");

	MaxBulletSpread = 2.0f;
}

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* aiController = OwnerComp.GetAIOwner();

	if (aiController)
	{
		ACharacter* aiCharacter = Cast<ACharacter>(aiController->GetPawn());
		
		if (aiCharacter == nullptr)
			return EBTNodeResult::Failed;

		FVector muzzleFlashLocation = aiCharacter->GetMesh()->GetSocketLocation(MuzzleFlashSocketName);
		AActor* targetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorName));

		if (targetActor == nullptr || !USAttributeComponent::IsActorAlive(targetActor))
			return EBTNodeResult::Failed;
		
		FVector direction = targetActor->GetActorLocation() - muzzleFlashLocation;
		FRotator muzzleFlashRotation = direction.Rotation();

		muzzleFlashRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
		muzzleFlashRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);
		
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		params.Instigator = aiCharacter;

		AActor* projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, muzzleFlashLocation, muzzleFlashRotation, params);

		return projectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}

