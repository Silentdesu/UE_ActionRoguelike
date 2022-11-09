// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
		return false;

	USActionComponent* ActionComponent = GetOwningComponent();

	return !ActionComponent->ActiveGameplayTags.HasAny(BlockedTags);
}

void USAction::Start_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantTags);

	bIsRunning = true;
}

void USAction::Stop_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantTags);

	bIsRunning = false;
}

UWorld* USAction::GetWorld() const
{
	//Outer is set when creating action via NewObject<T>

	UActorComponent* ActorComponent = Cast<UActorComponent>(GetOuter());

	return ActorComponent ? ActorComponent->GetWorld() : nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}
