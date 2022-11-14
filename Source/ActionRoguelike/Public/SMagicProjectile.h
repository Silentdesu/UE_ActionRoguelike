// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseProjectile.h"
#include "GameplayTagContainer.h"
#include "SMagicProjectile.generated.h"

class USActionEffect;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASBaseProjectile
{
	GENERATED_BODY()
	
	ASMagicProjectile();

protected:

	virtual void PostInitializeComponents() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere)
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSubclassOf<USActionEffect> ActionEffect;

protected:

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
