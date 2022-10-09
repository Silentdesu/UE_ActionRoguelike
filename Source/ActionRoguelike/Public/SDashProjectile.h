// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "SDashProjectile.generated.h"

class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASBaseProjectile
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Basic settings")
	UParticleSystem* ExplodeEmitter;

	UPROPERTY(EditAnywhere, Category = "Time settings")
	float TeleportDelay = .2f;

	UPROPERTY(EditAnywhere, Category = "Time settings")
	float ExplodeDelay = .2f;

	FTimerHandle TimerHandle;

protected:

	virtual void BeginPlay() override;

	virtual void Explode_Implementation() override;
	void TeleportInstigator();
};
