// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
		return false;

	USActionComponent* ActionComponent = GetOwningComponent();

	return !ActionComponent->ActiveGameplayTags.HasAny(BlockedTags);
}

void USAction::Start_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantTags);

	bIsRunning = true;
}

void USAction::Stop_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stoped: %s"), *ActionName.ToString()), FColor::White);

	ensureAlways(bIsRunning);

	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantTags);

	bIsRunning = false;
}

UWorld* USAction::GetWorld() const
{
	//Outer is set when creating action via NewObject<T>

	AActor* Actor = Cast<AActor>(GetOuter());

	return Actor ? Actor->GetWorld() : nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return ActionComp;
}

void USAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		Start(nullptr);
	}
	else
	{
		Stop(nullptr);
	}
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, bIsRunning);
	DOREPLIFETIME(USAction, ActionComp);
}
