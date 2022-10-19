// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values
USAttributeComponent::USAttributeComponent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.f;
	Health = MaxHealth;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* fromActor)
{
	if (fromActor)
	{
		return Cast<USAttributeComponent>(fromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* fromActor)
{
	if (fromActor)
	{
		USAttributeComponent* attributeComponent = GetAttributes(fromActor);

		if (attributeComponent)
		{
			return attributeComponent->IsAlive();
		}
	}

	return false;
}

bool USAttributeComponent::Kill(AActor* instigatorActor)
{
	return ApplyHealthChange(instigatorActor, -MaxHealth);
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return (MaxHealth - Health) <= 0.f;
}

float USAttributeComponent::GetCurrentHealth() const
{
	return Health;
}

float USAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(AActor* instigatorActor, float delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	float oldHealth = Health;
	Health = FMath::Clamp(Health + delta, 0.f, MaxHealth);
	float actualDelta = Health - oldHealth;

	OnHealthChanged.Broadcast(instigatorActor, this, Health, delta);

	return actualDelta != 0;
}

