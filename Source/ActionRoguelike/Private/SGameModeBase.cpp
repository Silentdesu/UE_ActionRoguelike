// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EngineUtils.h"

ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval = 2.0f;
}

void ASGameModeBase::KillAllAI()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* bot = *It;

		USAttributeComponent* attributeComponent = USAttributeComponent::GetAttributes(bot);

		if (attributeComponent && attributeComponent->IsAlive())
		{
			attributeComponent->Kill(this);
		}
	}
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimeHandler, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimeInterval, true);
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	int numberOfBots = 0;

	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* bot = *It;

		USAttributeComponent* attributeComponent = USAttributeComponent::GetAttributes(bot);

		if (attributeComponent && attributeComponent->IsAlive())
		{
			numberOfBots++;
		}
	}

	if (DifficultyCurve)
	{
		BotLimit = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (numberOfBots >= BotLimit)
		return;

	UEnvQueryInstanceBlueprintWrapper* queryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (queryInstance == nullptr)
		return;

	queryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn bot EQS Query failed!"));
		return;
	}

	TArray<FVector> locations = QueryInstance->GetResultsAsLocations();

	if (locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, locations[0], FRotator::ZeroRotator);
	}
}
