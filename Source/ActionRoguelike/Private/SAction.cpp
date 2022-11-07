// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

void USAction::Start_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
}

void USAction::Stop_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
}

UWorld* USAction::GetWorld() const
{
	//Outer is set when creating action via NewObject<T>

	UActorComponent* ActorComponent = Cast<UActorComponent>(GetOuter());

	return ActorComponent ? ActorComponent->GetWorld() : nullptr;
}
