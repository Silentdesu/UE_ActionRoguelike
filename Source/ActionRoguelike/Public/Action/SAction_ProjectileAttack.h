// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;
class ASCharacter;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

public:

	USAction_ProjectileAttack();

public:

	virtual void Start_Implementation(AActor* Instigator) override;

protected:

	UFUNCTION()
	void AttackDelayElapsed(ASCharacter* InstigatorCharacter);

protected:

	void OnAttack(ASCharacter* InstigatorCharacter);
	bool IsLineTraceHit(ASCharacter* InstigatorCharacter, FHitResult& outHitResult, FVector& outEndLocation);
	void SpawnProjectile(ASCharacter* InstigatorCharacter, TSubclassOf<AActor>& projectile, FVector& startLocation, const FVector& endLocation);

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimationDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float LineTraceLength;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* ProjectileSpawnVFX;
};
