// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAudioComponent;
class USoundBase;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASBaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBaseProjectile();

protected:

	UPROPERTY(EditAnywhere, Category = "Collision settings")
	FName CollisionProfileName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = "Impact settings")
	UParticleSystem* ImpactVFX;

	UPROPERTY(EditAnywhere, Category = "Impact settings")
	USoundBase* ImpactSFX;

	UPROPERTY(EditAnywhere, Category = "Impact settings")
	TSubclassOf<class UCameraShakeBase> CameraShakeBase;

	UPROPERTY(EditAnywhere, Category = "Impact settings")
	float CameraShakeInnerRadius;

	UPROPERTY(EditAnywhere, Category = "Impact settings")
	float CameraShakeOutsideRadius;

	UPROPERTY(EditAnywhere, Category = "Impact settings")
	bool bCameraFalloffEffect;

protected:

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();
};
