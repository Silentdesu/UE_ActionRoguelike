// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class USSaveGame;

/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASGameModeBase();

public:

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

public:

	UFUNCTION(Exec)
	void KillAllAI();

	UFUNCTION(BlueprintCallable)
	void OnActorKilled(AActor* Victim, AActor* Killer);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();
	void LoadSaveGame();

public:

	virtual void StartPlay() override;

protected:

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	FString SlotName;

	UPROPERTY()
	USSaveGame* CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Powerups")
	UEnvQuery* PowerupSpawnQuery;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Powerups")
	TArray<TSubclassOf<AActor>> PowerupClasses;	
	
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float RequiredPowerupDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 DesiredPowerupCount;

	UPROPERTY(EditAnywhere, Category = "AI")
	int BotLimit;

	UPROPERTY(EditAnywhere, Category = "AI")
	float SpawnTimeInterval;

	UPROPERTY(EditAnywhere, Category = "Timer settings", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	float RespawnDelay;

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditsPerKill;

	FTimerHandle TimeHandler;
};
