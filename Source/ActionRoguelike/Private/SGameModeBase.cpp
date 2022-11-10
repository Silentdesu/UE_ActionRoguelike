// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EngineUtils.h"
#include "SCharacter.h"
#include "TimerManager.h"
#include "SCreditSystem.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval = 2.0f;
	CreditsPerKill = 25;
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

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (Controller)
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(Victim);

	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;

		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	APawn* KillerPawn = Cast<APawn>(Killer);

	if (KillerPawn)
	{
		if (ASCreditSystem* CreditSystem = KillerPawn->GetPlayerState<ASCreditSystem>())
		{
			CreditSystem->AddCredits(CreditsPerKill);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKiled -> Victim: %s, Killer: %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimeHandler, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimeInterval, true);

	if (PowerupClasses.Num() > 0)
	{
		UEnvQueryInstanceBlueprintWrapper* PowerupSpawnQueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);

		if (PowerupSpawnQueryInstance)
		{
			PowerupSpawnQueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Bot spawning is disabled by cvar 'CVarSpawnBots'"));

		return;
	}

	int32 numberOfBots = 0;

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

void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		Locations.RemoveAt(RandomLocationIndex);

		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerupDistance)
			{
				bValidLocation = false;
				break;
			}
		}

		if (!bValidLocation)
		{
			continue;
		}

		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}
