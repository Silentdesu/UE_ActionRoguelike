// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "SBaseWorldUserWidget.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attribute Component"));
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TimeToHitParameterName = "TimeToHit";
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeeing);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
			SetTargetActor(InstigatorActor);

		if (ActiveHealthWidget == nullptr)
		{
			ActiveHealthWidget = CreateWidget<USBaseWorldUserWidget>(GetWorld(), HealthWidget);

			if (ActiveHealthWidget)
			{
				ActiveHealthWidget->AttachedActor = this;
				ActiveHealthWidget->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParameterName, GetWorld()->TimeSeconds);

		if (NewHealth <= 0.0f)
		{
			AAIController* aiController = Cast<AAIController>(GetController());

			if (aiController)
			{
				aiController->GetBrainComponent()->StopLogic(TEXT("Killed"));
			}

			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetMesh()->SetAllBodiesSimulatePhysics(true);


			SetLifeSpan(10.0f);
		}
	}
}

void ASAICharacter::SetTargetActor(AActor* newTarget)
{
	AAIController* aiController = Cast<AAIController>(GetController());

	if (aiController)
	{
		aiController->GetBlackboardComponent()->SetValueAsObject(TargetKey, newTarget);
	}
}

void ASAICharacter::OnPawnSeeing(APawn* pawn)
{
	SetTargetActor(pawn);
}
