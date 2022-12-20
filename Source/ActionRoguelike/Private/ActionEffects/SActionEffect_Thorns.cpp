// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEffects/SActionEffect_Thorns.h"
#include "SAttributeComponent.h"
#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	ReflectFraction = 0.2f;

	Duration = 0.0f;
	Period = 0.0f;
}

void USActionEffect_Thorns::Start_Implementation(AActor* Instigator)
{
	Super::Start_Implementation(Instigator);

	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());

	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::Stop_Implementation(AActor* Instigator)
{
	Super::Stop_Implementation(Instigator);

	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());

	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::OnHealthChanged(AActor* Instigator, USAttributeComponent* OwingComponent, float NewHealth, float Delta)
{
	AActor* Actor = GetOwningComponent()->GetOwner();

	if (Delta < 0.0f && Actor != Instigator)
	{
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);

		if (ReflectedAmount == 0.0f)
		{
			return;
		}

		ReflectedAmount = FMath::Abs(ReflectedAmount);

		USGameplayFunctionLibrary::ApplyDamage(Actor, Instigator, ReflectFraction);
	}
}

